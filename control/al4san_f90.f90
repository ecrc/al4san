!!!
!
! -- Inria
! -- (C) Copyright 2012
!
! This software is a computer program whose purpose is to process
! Matrices Over Runtime Systems @ Exascale (AL4SAN). More information
! can be found on the following website: http://www.inria.fr/en/teams/al4san.
!
! This software is governed by the CeCILL-B license under French law and
! abiding by the rules of distribution of free software.  You can  use,
! modify and/ or redistribute the software under the terms of the CeCILL-B
! license as circulated by CEA, CNRS and INRIA at the following URL
! "http://www.cecill.info".
!
! As a counterpart to the access to the source code and  rights to copy,
! modify and redistribute granted by the license, users are provided only
! with a limited warranty  and the software's author,  the holder of the
! economic rights,  and the successive licensors  have only  limited
! liability.
!
! In this respect, the user's attention is drawn to the risks associated
! with loading,  using,  modifying and/or developing or reproducing the
! software by the user in light of its specific status of free software,
! that may mean  that it is complicated to manipulate,  and  that  also
! therefore means  that it is reserved for developers  and  experienced
! professionals having in-depth computer knowledge. Users are therefore
! encouraged to load and test the software's suitability as regards their
! requirements in conditions enabling the security of their systems and/or
! data to be ensured and,  more generally, to use and operate it in the
! same conditions as regards security.
!
! The fact that you are presently reading this means that you have had
! knowledge of the CeCILL-B license and that you accept its terms.
!
!!!

!
! Copyright © 2011 The Numerical Algorithms Group Ltd. All rights reserved.
!
! Redistribution and use in source and binary forms, with or without
! modification, are permitted provided that the following conditions are
! met:
! - Redistributions of source code must retain the above copyright notice,
!   this list of conditions, and the following disclaimer.
! - Redistributions in binary form must reproduce the above copyright
!   notice, this list of conditions and the following disclaimer listed in
!   this license in the documentation and/or other materials provided with
!   the distribution.
! - Neither the name of the copyright holders nor the names of its
!   contributors may be used to endorse or promote products derived from
!   this software without specific prior written permission.
!
! This software is provided by the copyright holders and contributors "as
! is" and any express or implied warranties, including, but not limited
! to, the implied warranties of merchantability and fitness for a
! particular purpose are disclaimed. in no event shall the copyright owner
! or contributors be liable for any direct, indirect, incidental, special,
! exemplary, or consequential damages (including, but not limited to,
! procurement of substitute goods or services; loss of use, data, or
! profits; or business interruption) however caused and on any theory of
! liability, whether in contract, strict liability, or tort (including
! negligence or otherwise) arising in any way out of the use of this
! software, even if advised of the possibility of such damage.
!
!
!
!  AL4SAN fortran 90 interface
!  AL4SAN is a software package provided by Univ. of Tennessee,
!  Univ. of California Berkeley and Univ. of Colorado Denver
!
! @version 1.1.0
! @author Numerical Algorithm Group
! @date 2011-09-15
! @precisions normal z -> c d s
!
module al4san

      use al4san_s
      use al4san_d
      use al4san_ds
      use al4san_c
      use al4san_z
      use al4san_zc
      include 'fortran.h'

      logical :: al4san_initialized = .false.
      integer, parameter :: sp = kind(0.0)
      integer, parameter :: dp = kind(0.0d0)

      interface
         function AL4SAN_Init_c(cpus, gpus) &
          & bind(c, name='AL4SAN_Init')
            use iso_c_binding
            implicit none
            integer(kind=c_int) :: AL4SAN_Init_c
            integer(kind=c_int), value :: cpus, gpus
         end function AL4SAN_Init_c
      end interface

      interface
         function AL4SAN_Finalize_c() &
          & bind(c, name='AL4SAN_Finalize')
            use iso_c_binding
            implicit none
            integer(kind=c_int) :: AL4SAN_Finalize_c
         end function AL4SAN_Finalize_c
      end interface

      interface
         function AL4SAN_Set_c(param, pval) &
          & bind(c, name='AL4SAN_Set')
            use iso_c_binding
            implicit none
            integer(kind=c_int) :: AL4SAN_Set_c
            integer(kind=c_int), value :: param
            integer(kind=c_int), value :: pval
         end function AL4SAN_Set_c
      end interface

      interface
         function AL4SAN_Get_c(param, pval) &
          & bind(c, name='AL4SAN_Get')
            use iso_c_binding
            implicit none
            integer(kind=c_int) :: AL4SAN_Get_c
            integer(kind=c_int), value :: param
            type(c_ptr), value :: pval
         end function AL4SAN_Get_c
      end interface

      interface
         function AL4SAN_Enable_c(param) &
          & bind(c, name='AL4SAN_Enable')
            use iso_c_binding
            implicit none
            integer(kind=c_int) :: AL4SAN_Enable_c
            integer(kind=c_int), value :: param
         end function AL4SAN_Enable_c
      end interface

      interface
         function AL4SAN_Disable_c(param) &
          & bind(c, name='AL4SAN_Disable')
            use iso_c_binding
            implicit none
            integer(kind=c_int) :: AL4SAN_Disable_c
            integer(kind=c_int), value :: param
         end function AL4SAN_Disable_c
      end interface

      interface
         subroutine free_c(ptr) bind(c, name='free')
            use iso_c_binding
            implicit none
            type(c_ptr), value :: ptr
         end subroutine free_c
      end interface

      interface
         function AL4SAN_Version_c(maj,min,mic) &
          & bind(c, name='AL4SAN_Version')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Version_c
            type(c_ptr), value ::  maj,min,mic
         end function AL4SAN_Version_c
      end interface

      interface
         function AL4SAN_Init_Affinity_c(cores,gpus,bindtab) &
          & bind(c, name='AL4SAN_Init_Affinity')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Init_Affinity_c
            integer(kind=c_int), value ::  cores, gpus
            type(c_ptr), value :: bindtab
         end function AL4SAN_Init_Affinity_c
      end interface

      interface
         function AL4SAN_Dealloc_Handle_c(handle) &
          & bind(c, name='AL4SAN_Dealloc_Handle')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Dealloc_Handle_c
            type(c_ptr) :: handle
         end function AL4SAN_Dealloc_Handle_c
      end interface

      interface
         function AL4SAN_Sequence_Create_c(seq) &
          & bind(c, name='AL4SAN_Sequence_Create')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Sequence_Create_c
            type(c_ptr) :: seq
         end function AL4SAN_Sequence_Create_c
      end interface

      interface
         function AL4SAN_Sequence_Destroy_c(seq) &
          & bind(c, name='AL4SAN_Sequence_Destroy')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Sequence_Destroy_c
            type(c_ptr), value :: seq
         end function AL4SAN_Sequence_Destroy_c
      end interface

      interface
         function AL4SAN_Sequence_Wait_c(seq) &
          & bind(c, name='AL4SAN_Sequence_Wait')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Sequence_Wait_c
            type(c_ptr), value :: seq
         end function AL4SAN_Sequence_Wait_c
      end interface

      interface
         function AL4SAN_Sequence_Flush_c(seq,req) &
          & bind(c, name='AL4SAN_Sequence_Flush')
            use iso_c_binding
            integer(kind=c_int) :: AL4SAN_Sequence_Flush_c
            type(c_ptr), value :: seq
            type(c_ptr), value :: req
         end function AL4SAN_Sequence_Flush_c
      end interface

   contains

   subroutine al4san_init(cores,gpus,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(in) :: cores, gpus
      integer(kind=c_int), intent(out) :: info
      info = al4san_init_c(cores,gpus)
      al4san_initialized = .true.
   end subroutine al4san_init

   subroutine al4san_finalize(info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(out) :: info
      info = al4san_finalize_c()
      al4san_initialized = .false.
   end subroutine al4san_finalize

   subroutine al4san_set(param,pval,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(in) :: param
      integer(kind=c_int), intent(in) :: pval
      integer(kind=c_int), intent(out) :: info
      info = al4san_set_c(param,pval)
   end subroutine al4san_set

   subroutine al4san_get(param,pval,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(in) :: param
      integer(kind=c_int), intent(out), target :: pval
      integer(kind=c_int), intent(out) :: info
      info = al4san_get_c(param,c_loc(pval))
   end subroutine al4san_get

   subroutine al4san_enable(param,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(in) :: param
      integer(kind=c_int), intent(out) :: info
      info = al4san_enable_c(param)
   end subroutine al4san_enable

   subroutine al4san_disable(param,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(in) :: param
      integer(kind=c_int), intent(out) :: info
      info = al4san_disable_c(param)
   end subroutine al4san_disable


   subroutine al4san_version(ver_major,ver_minor,ver_micro,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(out), target :: ver_major,ver_minor,ver_micro
      integer(kind=c_int), intent(out) :: info
      info = al4san_version_c(c_loc(ver_major),c_loc(ver_minor),c_loc(ver_micro))
   end subroutine al4san_version

   subroutine al4san_init_affinity(cores,bindtab,info)
      use iso_c_binding
      implicit none
      integer(kind=c_int), intent(in) :: cores
      integer(kind=c_int), intent(out), target :: bindtab
      integer(kind=c_int), intent(out) :: info
      info = al4san_init_affinity_c(cores,c_loc(bindtab))
   end subroutine al4san_init_affinity

   subroutine al4san_sequence_create(sequence,info)
      use iso_c_binding
      implicit none
      type(c_ptr), intent(out) :: sequence
      integer(kind=c_int), intent(out) :: info
      info = al4san_sequence_create_c(sequence)
   end subroutine al4san_sequence_create

   subroutine al4san_sequence_destroy(sequence,info)
      use iso_c_binding
      implicit none
      type(c_ptr), intent(in) :: sequence
      integer(kind=c_int), intent(out) :: info
      info = al4san_sequence_destroy_c(sequence)
   end subroutine al4san_sequence_destroy

   subroutine al4san_sequence_wait(sequence,info)
      use iso_c_binding
      implicit none
      type(c_ptr), intent(in) :: sequence
      integer(kind=c_int), intent(out) :: info
      info = al4san_sequence_wait_c(sequence)
   end subroutine al4san_sequence_wait

   subroutine al4san_sequence_flush(sequence,request,info)
      use iso_c_binding
      implicit none
      type(c_ptr), intent(in) :: sequence, request
      integer(kind=c_int), intent(out) :: info
      info = al4san_sequence_flush_c(sequence,request)
   end subroutine al4san_sequence_flush

end module al4san
