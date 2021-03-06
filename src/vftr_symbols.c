/*
   This file is part of Vftrace.

   Vftrace is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Vftrace is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <elf.h>

#ifndef __ve__
#include "demangle.h"
#endif

#include "vftr_filewrite.h"
#include "vftr_symbols.h"

int        vftr_nsymbols;
symtab_t **vftr_symtab;

int vftr_cmpsym( const void *a, const void *b ) {
    symtab_t *s1 = *(symtab_t **) a;
    symtab_t *s2 = *(symtab_t **) b;
    if( s1->addr  < s2->addr ) return -1;
    if( s1->addr == s2->addr ) {
        if( s1->demangled  > s2->demangled ) return -1;
        if( s1->demangled  < s2->demangled ) return  1;
        else                                 return  0;
    } else
        return 1;
}

void vftr_print_symbol_table (FILE *f) {
    int i;
    fprintf (f, "SYMBOL TABLE: %d\n", vftr_nsymbols );
    for (i = 0; i < vftr_nsymbols; i++) {
	fprintf( f, "%5d %p %04x %d %s", 
        i, vftr_symtab[i]->addr,
           vftr_symtab[i]->index,
           vftr_symtab[i]->demangled,  vftr_symtab[i]->name );
	if( vftr_symtab[i]->demangled )
	    fprintf( f, " [%s]", vftr_symtab[i]->full );
	fprintf( f, "\n" );
    }
    fprintf( f, "-----------------------------------------------------------------\n" );
}

/*
** vftr_get_library_symtab - retrieve part of the symbol table for a libary or executable.
** Arguments:
**    char   *target    Path to the library or executable
**    off_t  base       Library's base address
**    int    pass       0: count only, 1: save symbols
*/
#define vftr_get_library_symtab vftr_get_library_symtab_linux

void vftr_get_library_symtab_linux( char *target, off_t base, int pass ) {
    void           *addr;
    char           *headerStringTable = NULL;
    char           *symbolStringTable = NULL;
    char           *padding = NULL;
    FILE           *exe;
    int             i, j, n, nst, nsym;
    int             symbolCount;
    int             symtabIndex = -1;
    int             symstrIndex = -1;
    int             nehdr = sizeof(Elf64_Ehdr);
    Elf64_Ehdr      ehdr;                /* ELF header */
    Elf64_Shdr     *shdr = NULL;         /* ELF section header */
    Elf64_Sym      *symbolTable = NULL;  /* ELF symbol table */
    symtab_t       *found;
    
    if ((exe = fopen( target, "r" )) == NULL) {
	fprintf (vftr_log, "opening %s", target);
        perror (target); abort();
    }
    
    if (fread (&ehdr, 1, nehdr, exe ) != nehdr) {
	perror ("reading ELF header from executable");
	abort();
    }

    n = ehdr.e_shnum * sizeof(Elf64_Shdr);
    shdr = (Elf64_Shdr *) malloc( n );
    fseek( exe, (long)ehdr.e_shoff, SEEK_SET );
    if (fread( shdr, 1, n, exe ) != n) {
	perror ("reading section headers from executable");
	abort();
    }

    nst = shdr[ehdr.e_shstrndx].sh_size;
    if (nst == 0) return; /* No headers in this section */

    headerStringTable = (char *)malloc(nst);
    memset (headerStringTable, 0, nst);
    fseek (exe, (long)shdr[ehdr.e_shstrndx].sh_offset, SEEK_SET);
    if (fread( headerStringTable, 1, nst, exe ) != nst)  {
	perror( "reading string table from executable" );
	abort();
    }

    for (i = 0; i < ehdr.e_shnum; i++) {
        char *name = &headerStringTable[shdr[i].sh_name];
        if (!strcmp(name,".strtab")) {
		symstrIndex = i;
	} else if (!strcmp(name,".symtab")) {
		symtabIndex = i;
	}
    }

    if (symstrIndex == -1) {
        fprintf( vftr_log, "No symbol string table in %s\n", target );
        return;
    } else {
	nsym = shdr[symstrIndex].sh_size;
	symbolStringTable = (char *)malloc(nsym);
        memset (symbolStringTable, 0, nsym);
	fseek (exe, (long)shdr[symstrIndex].sh_offset, SEEK_SET); 
	if ((i = fread (symbolStringTable, 1, nsym, exe)) != nsym ) {
	    perror( "reading symbol string table from executable" );
	    abort();
	}
    }

    if( symtabIndex == -1 ) {
        fprintf (vftr_log, "No symbol table in %s\n", target);
        return;
    } else {
	nsym = shdr[symtabIndex].sh_size;
	symbolTable = (Elf64_Sym *)malloc(nsym);
	memset (symbolTable, 0, nsym);
	symbolCount = nsym / sizeof(Elf64_Sym);
	fseek (exe, (long)shdr[symtabIndex].sh_offset, SEEK_SET); 
	if ((i = fread( symbolTable, 1, nsym, exe )) != nsym) {
	    perror( "reading symbol table from executable" );
	    abort();
	}
    }

    n = 0;
    for (i = 0; i < symbolCount; i++) {
        Elf64_Sym s = symbolTable[i];
	if (ELF64_ST_TYPE(s.st_info) == STT_FUNC && s.st_value) {
	    if (pass) {
                j = vftr_nsymbols + n;
                vftr_symtab[j] = (symtab_t *) malloc( sizeof(symtab_t) );
#if defined(__ve__)
/* All absolute */
                vftr_symtab[j]->addr = (void *) s.st_value;
#else
                vftr_symtab[j]->addr = (void *)(base + s.st_value);
#endif
                /* Copy symbol name and demangle C++ generated names */
#ifdef __cplusplus
                vftr_symtab[j]->demangled =
                       demangle (&symbolStringTable[s.st_name],
                                 &(vftr_symtab[j]->name),
                                 &(vftr_symtab[j]->full));
#else
                vftr_symtab[j]->demangled = 0;
                vftr_symtab[j]->name = strdup(&symbolStringTable[s.st_name]);
                vftr_symtab[j]->full = NULL;
                vftr_symtab[j]->index = s.st_shndx;
#endif
            }
            n++;
	}
    }

    vftr_nsymbols += n;

    free (symbolStringTable);
    free (symbolTable);
    free (headerStringTable);
    free (shdr);
    free (padding);
    
    if (fclose(exe )) {
        perror ("fclose");
    }

}

FILE *get_fmap (char *target) {
    char maps[80];
    FILE *fmap;
    if (target) {
      /* Standalone testing use: inspect executable or library */
      if( !strstr( target, "/maps" ) ) {
        /* Executable or library */
        vftr_nsymbols = 0;
        vftr_get_library_symtab( target, 0L, 0 );
        vftr_symtab = (symtab_t **) malloc( vftr_nsymbols * sizeof(symtab_t *) );
        vftr_nsymbols = 0;
        vftr_get_library_symtab( target, 0L, 1 );
        return NULL;
      }
      strcpy (maps, target);
    } else {
      /* Normal use: from vftr_initialize() */
      sprintf (maps, "/proc/%d/maps", getpid());
    }

    if ((fmap = fopen (maps, "r")) == NULL) {
        fprintf (vftr_log, "Opening %s", maps);
	perror (" ");
	abort();
    }
    return fmap;
}

/* This function parses the lines in the /proc/pid/maps file. Each line consists of:
	- an address range [base-top]. We split this at the "-" sign, since we
	  we have no use for the value of the upper end of the address range.
	- the file permissions. We skip non-executable entries.
	- the offset.
	- the device and inode value. We do not need them.
	- the path to the file the memory range was mapped from, i.e. the
          libraries we want to process.
*/
void parse_fmap_line (char *line, pathList_t **library, pathList_t **head) {
    	char *base, *top, *permissions, *offset, *device, *inode, *path;
        pathList_t *newlib;

        base = strtok(line,"-");
        top = strtok(NULL," ");
        permissions = strtok(NULL," ");
        if (permissions[2] != 'x') {
		return; /* Ignore if not executable */
	}
        offset = strtok(NULL," ");
#ifndef __VMAP_OFFSET
        if (strcmp(offset,"00000000")) {
		return;
	}
#endif
        device = strtok(NULL," ");
        inode = strtok(NULL," ");
        path = strtok(NULL," \n");
/* We filter out devices and system libraries (they are not instrumented anyway).
   For this, we test for some common places in their path names
*/
        if (!path ) return; /* Ignore if no path */
        if (!strncmp (path, "/dev/", 5)) return;
        if (!strncmp (path, "/usr/lib", 8)) return;
        if (!strncmp (path, "/usr/lib64", 10)) return;
        if (!strncmp (path, "/lib", 4)) return;
#ifdef __ve__
        if (!strncmp (path, "/opt/nec/ve/veos/lib", 20)) return;
#endif
        if (*path == '[') return; /* Ignore [lib] */
        newlib = (pathList_t  *) malloc( sizeof(pathList_t) );
        if (*library) {
		(*library)->next = newlib;
        } else {
		*head = newlib;
	}
        *library = newlib;
        sscanf (base, "%lx", &(*library)->base);  /* Save library base */
        (*library)->path = strdup( path );
        (*library)->next = NULL;
#ifdef __VMAP_OFFSET
	(*library)->offset = strtoul(offset, NULL, 16);
        vftr_get_library_symtab ((*library)->path, (*library)->base - (*library)->offset, 0 ); /* First pass, counting only */
#else
        vftr_get_library_symtab ((*library)->path, 0L, 0 ); /* First pass, counting only */
	(*library)->offset = 0L;
#endif

}

/*
** vftr_create_symbol_table - two-pass retrieval of symbol table: in the first pass
** the symbols are counted, in the second pass the symbols are saved.
**
** Arguments:
**    char *    target   Path to the maps info: /proc/<pid>/maps of an arbitrary
**                       process or path to a specific library or executable.
**
** Vftrace calls vftr_create_symbol_table with NULL argument, which means that the
** symbol table is constructed for the current executable and all the shared
** libraries it uses.
*/
int vftr_create_symbol_table (int rank, char *target) {
    char line[LINESIZE];
    int symCount = 0, pass;
    pathList_t *head, *library, *next;

    FILE *fmap = get_fmap (target);
    if (!fmap) return 1;
    /* Read the application libraries in the maps info */
    library = NULL;
    head = NULL;
    while (fgets (line, LINESIZE, fmap)) {
	parse_fmap_line (line, &library, &head);
    }
    if (head == NULL) {
	return 1;
    }
    // What to do if vftr_nsymbols == 0?


    /* Allocate application's symbol table */
    vftr_symtab = (symtab_t **) malloc (vftr_nsymbols * sizeof(symtab_t *));

    /* Second pass, this time saving the symbols */
    vftr_nsymbols = 0;

    for (library = head; library; library = next) {
#ifdef __VMAP_OFFSET
        vftr_get_library_symtab (library->path, library->base - library->offset, 1);
#else
        /* FIXME Need to understand why base has to be set this way, if at all correct */
        off_t base = strstr(library->path, ".so") ? library->base : 0L;
        vftr_get_library_symtab( library->path, base, 1 );
#endif
        next = library->next;
        free (library);
    }

    qsort (vftr_symtab, vftr_nsymbols, sizeof(symtab_t *), vftr_cmpsym);
    return 0;

}

symtab_t **vftr_find_nearest(symtab_t **table, void *addr, int count) {
  int imid = 0;
  int imin = 0;
  int imax = count - 1;

  if (table[imin]->addr <= addr && addr <= table[imax]->addr) {
    while (imax >= imin) {
        imid = (imin + imax) / 2; /* Divide interval in two */
        if (table[imid]->addr <= addr &&
           table[imid+1]->addr >  addr) {
		break; /* Done: nearest match */
        } else if (table[imid]->addr < addr) {
          imin = imid + 1;    /* Next search in upper half */
        } else {
          imax = imid - 1;    /* Next search in lower half */
	}
    }
    return &table[imid]; /* Return nearest match */
  } else {
    return NULL;
  }
}

char *vftr_find_symbol (void *addr, int line, char **full) {
    symtab_t **found;
    size_t offset;
    char *name, *newname;
    void *addr_found;
    found = vftr_find_nearest (vftr_symtab, addr, vftr_nsymbols);
    if (found) {
      addr_found = (*found)->addr;
      if ((*found)->demangled) *full = (*found)->full;
      name = (*found)->name;
      if (addr_found == addr) return name; /* Exact match (automatic instrumentation) */
      if (line > 0) {
        int len = strlen(name);
        newname = (char *) malloc (sizeof(char) * (16 + len));
        /* Chop Fortran trailing underscore */
        if (name[len-1] == '_' && name[len-2] != '_') name[len-1] = 0;
        sprintf (newname, "%s:%d-endline", name, line); /* "endline" is replaced later */
      } else {
        int len = 30 + strlen(name);
        offset = (size_t)addr - (size_t) addr_found;
        newname = (char *) malloc (sizeof(char) * len);
        memset (newname, 0, len);
        sprintf (newname, "%s+0x%lx", name, offset);
      }
    } else {
      /* Address not in symbol table */
        newname = (char *) malloc (sizeof(char) * 24);
        memset (newname, 0, 24);
        sprintf (newname,"0x%p", addr);
    }
    return newname;
}
