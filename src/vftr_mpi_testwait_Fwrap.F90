! This file is part of Vftrace.
!
! Vftrace is free software; you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation; either version 2 of the License, or
! (at your option) any later version.
!
! Vftrace is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License along
! with this program; if not, write to the Free Software Foundation, Inc.,
! 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifdef _MPI

SUBROUTINE MPI_Barrier(COMM, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Barrier_F
   IMPLICIT NONE
   INTEGER COMM
   INTEGER ERROR

   CALL vftr_MPI_Barrier_F(COMM, ERROR)

END SUBROUTINE MPI_Barrier

SUBROUTINE MPI_Test(REQUEST, FLAG, STATUS, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Test_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER REQUEST
   LOGICAL FLAG
   INTEGER STATUS(MPI_STATUS_SIZE)
   INTEGER ERROR

   CALL vftr_MPI_Test_F(REQUEST, FLAG, STATUS, ERROR)

END SUBROUTINE MPI_Test

SUBROUTINE MPI_Testany(COUNT, ARRAY_OREQUESTS, INDEX, FLAG, STATUS, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Testany_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER COUNT
   INTEGER ARRAY_OREQUESTS(*)
   INTEGER INDEX
   LOGICAL FLAG
   INTEGER STATUS(MPI_STATUS_SIZE)
   INTEGER ERROR

   CALL vftr_MPI_Testany_F(COUNT, ARRAY_OREQUESTS, INDEX, FLAG, STATUS, ERROR)

END SUBROUTINE MPI_Testany

SUBROUTINE MPI_Testsome(INCOUNT, ARRAY_OREQUESTS, OUTCOUNT, &
                        ARRAY_OINDICES, ARRAY_OSTATUSES, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Testsome_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER INCOUNT
   INTEGER ARRAY_OREQUESTS(*)
   INTEGER OUTCOUNT
   INTEGER ARRAY_OINDICES(*)
   INTEGER ARRAY_OSTATUSES(MPI_STATUS_SIZE,*)
   INTEGER ERROR

   CALL vftr_MPI_Testsome_F(INCOUNT, ARRAY_OREQUESTS, OUTCOUNT, &
                            ARRAY_OINDICES, ARRAY_OSTATUSES, ERROR)

END SUBROUTINE MPI_Testsome

SUBROUTINE MPI_Testall(COUNT, ARRAY_OREQUESTS, FLAG, ARRAY_OSTATUSES, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Testall_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER COUNT
   INTEGER ARRAY_OREQUESTS(*)
   LOGICAL FLAG
   INTEGER ARRAY_OSTATUSES(MPI_STATUS_SIZE,*)
   INTEGER ERROR

   CALL vftr_MPI_Testall_F(COUNT, ARRAY_OREQUESTS, FLAG, ARRAY_OSTATUSES, ERROR)

END SUBROUTINE MPI_Testall

SUBROUTINE MPI_Wait(REQUEST, STATUS, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Wait_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER REQUEST
   INTEGER STATUS(MPI_STATUS_SIZE)
   INTEGER ERROR

   CALL vftr_MPI_Wait_F(REQUEST, STATUS, ERROR)

END SUBROUTINE MPI_Wait

SUBROUTINE MPI_Waitany(COUNT, ARRAY_OREQUESTS, INDEX, STATUS, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Waitany_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER COUNT
   INTEGER ARRAY_OREQUESTS(*)
   INTEGER INDEX
   INTEGER STATUS(MPI_STATUS_SIZE)
   INTEGER ERROR

   CALL vftr_MPI_Waitany_F(COUNT, ARRAY_OREQUESTS, INDEX, STATUS, ERROR)

END SUBROUTINE MPI_Waitany

SUBROUTINE MPI_Waitsome(INCOUNT, ARRAY_OREQUESTS, OUTCOUNT, &
                        ARRAY_OINDICES, ARRAY_OSTATUSES, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Waitsome_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER INCOUNT
   INTEGER ARRAY_OREQUESTS(*)
   INTEGER OUTCOUNT
   INTEGER ARRAY_OINDICES(*)
   INTEGER ARRAY_OSTATUSES(MPI_STATUS_SIZE,*)
   INTEGER ERROR

   CALL vftr_MPI_Waitsome_F(INCOUNT, ARRAY_OREQUESTS, OUTCOUNT, &
                            ARRAY_OINDICES, ARRAY_OSTATUSES, ERROR)

END SUBROUTINE MPI_Waitsome

SUBROUTINE MPI_Waitall(COUNT, ARRAY_OREQUESTS, ARRAY_OSTATUSES, ERROR)
   USE vftr_mpi_testwait_c2F, &
      ONLY : vftr_MPI_Waitall_F
   USE mpi, ONLY: MPI_STATUS_SIZE
   IMPLICIT NONE
   INTEGER COUNT
   INTEGER ARRAY_OREQUESTS(*)
   INTEGER ARRAY_OSTATUSES(MPI_STATUS_SIZE,*)
   INTEGER ERROR

   CALL vftr_MPI_Waitall_F(COUNT, ARRAY_OREQUESTS, ARRAY_OSTATUSES, ERROR)

END SUBROUTINE MPI_Waitall

#endif 

