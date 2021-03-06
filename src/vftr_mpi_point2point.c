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

#ifdef _MPI
#include <mpi.h>

#include "vftr_timer.h"
#include "vftr_sync_messages.h"
#include "vftr_async_messages.h"
#include "vftr_mpi_environment.h"

int vftr_MPI_Send(const void *buf, int count, MPI_Datatype datatype,
                  int dest, int tag, MPI_Comm comm) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Send(buf, count, datatype, dest, tag, comm);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Send(buf, count, datatype, dest, tag, comm);
      long long tend = vftr_get_runtime_usec();

      vftr_store_sync_message_info(send, count, datatype, dest, tag, comm, tstart, tend);

      return retVal;
   }
}

int vftr_MPI_Bsend(const void *buf, int count, MPI_Datatype datatype,
                   int dest, int tag, MPI_Comm comm) {
   
   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Bsend(buf, count, datatype, dest, tag, comm);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Bsend(buf, count, datatype, dest, tag, comm);
      long long tend = vftr_get_runtime_usec();

      vftr_store_sync_message_info(send, count, datatype, dest, tag, comm, tstart, tend);

      return retVal;
   }
}

int vftr_MPI_Bsend_init(const void *buf, int count,
                        MPI_Datatype datatype, int dest, int tag,
                        MPI_Comm comm, MPI_Request *request) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
   
      vftr_register_request(send, count, datatype, dest, tag, comm, *request, tstart);
   
      return retVal;
   }
}

int vftr_MPI_Isend(const void *buf, int count, MPI_Datatype datatype,
                   int dest, int tag, MPI_Comm comm,
                   MPI_Request *request) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);

      vftr_register_request(send, count, datatype, dest, tag, comm, *request, tstart);

      return retVal;
   }
}

int vftr_MPI_Ibsend(const void *buf, int count, MPI_Datatype datatype,
                    int dest, int tag, MPI_Comm comm, MPI_Request *request) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);

      vftr_register_request(send, count, datatype, dest, tag, comm, *request, tstart);

      return retVal;
   }
}

int vftr_MPI_Ssend(const void *buf, int count, MPI_Datatype datatype,
                   int dest, int tag, MPI_Comm comm) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Ssend(buf, count, datatype, dest, tag, comm);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Ssend(buf, count, datatype, dest, tag, comm);
      long long tend = vftr_get_runtime_usec();
  
      vftr_store_sync_message_info(send, count, datatype, dest, tag, comm, tstart, tend);
  
      return retVal;
   }
}

int vftr_MPI_Issend(const void *buf, int count, MPI_Datatype datatype,
                    int dest, int tag, MPI_Comm comm, MPI_Request *request) {
 
   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
 
      vftr_register_request(send, count, datatype, dest, tag, comm, *request, tstart);
 
      return retVal;
   }
}

int vftr_MPI_Rsend(const void *buf, int count, MPI_Datatype datatype,
                   int dest, int tag, MPI_Comm comm) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Rsend(buf, count, datatype, dest, tag, comm);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Rsend(buf, count, datatype, dest, tag, comm);
      long long tend = vftr_get_runtime_usec();
  
      vftr_store_sync_message_info(send, count, datatype, dest, tag, comm, tstart, tend);
  
      return retVal;
   }
}

int vftr_MPI_Irsend(const void *buf, int count, MPI_Datatype datatype,
                    int dest, int tag, MPI_Comm comm, MPI_Request *request) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
  
      vftr_register_request(send, count, datatype, dest, tag, comm, *request, tstart);
  
      return retVal;
   }
}

int vftr_MPI_Recv(void *buf, int count, MPI_Datatype datatype,
                  int source, int tag, MPI_Comm comm, MPI_Status *status) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Recv(buf, count, datatype, source, tag, comm, status);
   } else {

      MPI_Status tmpstatus;
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Recv(buf, count, datatype, source, tag, comm, &tmpstatus);
      long long tend = vftr_get_runtime_usec();
   
      vftr_store_sync_message_info(recv, count, datatype, tmpstatus.MPI_SOURCE,
         tmpstatus.MPI_TAG, comm, tstart, tend);
   
      // handle the special case of MPI_STATUS_IGNORE
      if (status != MPI_STATUS_IGNORE) {
         status->MPI_SOURCE = tmpstatus.MPI_SOURCE;
         status->MPI_TAG = tmpstatus.MPI_TAG;
         status->MPI_ERROR = tmpstatus.MPI_ERROR;
      }
   
      return retVal;
   }
}

int vftr_MPI_Irecv(void *buf, int count, MPI_Datatype datatype,
                   int source, int tag, MPI_Comm comm, MPI_Request *request) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
   } else {
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);

      vftr_register_request(recv, count, datatype, source, tag, comm, *request, tstart);

      return retVal;
   }
}

int vftr_MPI_Sendrecv(const void *sendbuf, int sendcount,
                      MPI_Datatype sendtype, int dest, int sendtag,
                      void *recvbuf, int recvcount, MPI_Datatype recvtype,
                      int source, int recvtag, MPI_Comm comm,
                      MPI_Status *status) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag,
                           recvbuf, recvcount, recvtype, source, recvtag,
                           comm, status);
   } else {
      MPI_Status tmpstatus;
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag,
                                 recvbuf, recvcount, recvtype, source, recvtag,
                                 comm, &tmpstatus);
      long long tend = vftr_get_runtime_usec();
  
      int rank;
      PMPI_Comm_rank(comm, &rank);
      if (rank == source) {
         vftr_store_sync_message_info(send, sendcount, sendtype, dest,
                                      sendtag, comm, tstart, tend);
      } else {
         vftr_store_sync_message_info(recv, recvcount, recvtype,
                                      tmpstatus.MPI_SOURCE, tmpstatus.MPI_TAG,
                                      comm, tstart, tend);
      }
  
      // handle the special case of MPI_STATUS_IGNORE
      if (status != MPI_STATUS_IGNORE) {
         status->MPI_SOURCE = tmpstatus.MPI_SOURCE;
         status->MPI_TAG = tmpstatus.MPI_TAG;
         status->MPI_ERROR = tmpstatus.MPI_ERROR;
      }
  
      return retVal;
   }
}

int vftr_MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype,
                              int dest, int sendtag, int source, int recvtag,
                              MPI_Comm comm, MPI_Status *status) {

   // disable profiling based on the Pcontrol level
   if (vftrace_Pcontrol_level == 0) {
      return PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag,
                                   source, recvtag, comm, status);
   } else {
      MPI_Status tmpstatus;
      long long tstart = vftr_get_runtime_usec();
      int retVal = PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag,
                                         source, recvtag, comm, &tmpstatus);
      long long tend = vftr_get_runtime_usec();

      int rank;
      PMPI_Comm_rank(comm, &rank);
      if (rank == source) {
         vftr_store_sync_message_info(send, count, datatype, dest,
                                      sendtag, comm, tstart, tend);
      } else {
        vftr_store_sync_message_info(recv, count, datatype,
                                     tmpstatus.MPI_SOURCE, tmpstatus.MPI_TAG,
                                     comm, tstart, tend);
      }

      // handle the special case of MPI_STATUS_IGNORE
      if (status != MPI_STATUS_IGNORE) {
         status->MPI_SOURCE = tmpstatus.MPI_SOURCE;
         status->MPI_TAG = tmpstatus.MPI_TAG;
         status->MPI_ERROR = tmpstatus.MPI_ERROR;
      }

      return retVal;
   }
}

#endif
