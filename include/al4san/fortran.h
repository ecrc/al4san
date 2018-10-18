!**
!
! -- Inria
! -- (C) Copyright 2012
!
! This software is a computer program whose purpose is to process
! Matrices Over Runtime Systems @ Exascale (CHAMELEON). More information
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
!**
!
! @brief Chameleon Fortran77 header
! @version 1.0.0
! @author Bilel Hadri
! @author Mathieu Faverge
! @author Cedric Castagnede
! @date 2010-11-15
!
!**

!********************************************************************
!   AL4SAN constants - precisions
!
      integer  Al4sanByte, Al4sanInteger, Al4sanRealFloat
      integer  Al4sanRealDouble, Al4sanComplexFloat, Al4sanComplexDouble
      parameter ( Al4sanByte          = 0 )
      parameter ( Al4sanInteger       = 1 )
      parameter ( Al4sanRealFloat     = 2 )
      parameter ( Al4sanRealDouble    = 3 )
      parameter ( Al4sanComplexFloat  = 4 )
      parameter ( Al4sanComplexDouble = 5 )

!********************************************************************
!   AL4SAN constants - boolean
!
      integer AL4SAN_FALSE, AL4SAN_TRUE
      parameter ( AL4SAN_FALSE = 0 )
      parameter ( AL4SAN_TRUE  = 1 )

!********************************************************************
!   State machine switches
!
      integer AL4SAN_WARNINGS, AL4SAN_ERRORS, AL4SAN_AUTOTUNING
      integer AL4SAN_DAG, AL4SAN_PROFILING_MODE, AL4SAN_PARALLEL_MODE
      integer AL4SAN_BOUND
      parameter ( AL4SAN_WARNINGS       = 1 )
      parameter ( AL4SAN_ERRORS         = 2 )
      parameter ( AL4SAN_AUTOTUNING     = 3 )
      parameter ( AL4SAN_DAG            = 4 )
      parameter ( AL4SAN_PROFILING_MODE = 5 )
      parameter ( AL4SAN_PARALLEL_MODE  = 6 )
      parameter ( AL4SAN_BOUND          = 7 )

!********************************************************************
!   AL4SAN constants - configuration  parameters
!
      integer AL4SAN_CONCURRENCY
      parameter ( AL4SAN_CONCURRENCY      = 1 )


!********************************************************************
!   AL4SAN constants - success & error codes
!
      integer AL4SAN_SUCCESS, AL4SAN_ERR_NOT_INITIALIZED
      integer AL4SAN_ERR_REINITIALIZED, AL4SAN_ERR_NOT_SUPPORTED
      integer AL4SAN_ERR_ILLEGAL_VALUE, AL4SAN_ERR_NOT_FOUND
      integer AL4SAN_ERR_OUT_OF_MEMORY, AL4SAN_ERR_INTERNAL_LIMIT
      integer AL4SAN_ERR_UNALLOCATED, AL4SAN_ERR_FILESYSTEM
      integer AL4SAN_ERR_UNEXPECTED, AL4SAN_ERR_SEQUENCE_FLUSHED
      parameter ( AL4SAN_SUCCESS             =    0 )
      parameter ( AL4SAN_ERR_NOT_INITIALIZED = -101 )
      parameter ( AL4SAN_ERR_REINITIALIZED   = -102 )
      parameter ( AL4SAN_ERR_NOT_SUPPORTED   = -103 )
      parameter ( AL4SAN_ERR_ILLEGAL_VALUE   = -104 )
      parameter ( AL4SAN_ERR_NOT_FOUND       = -105 )
      parameter ( AL4SAN_ERR_OUT_OF_MEMORY   = -106 )
      parameter ( AL4SAN_ERR_INTERNAL_LIMIT  = -107 )
      parameter ( AL4SAN_ERR_UNALLOCATED     = -108 )
      parameter ( AL4SAN_ERR_FILESYSTEM      = -109 )
      parameter ( AL4SAN_ERR_UNEXPECTED      = -110 )
      parameter ( AL4SAN_ERR_SEQUENCE_FLUSHED= -111 )

!********************************************************************
!   AL4SAN constants - kernels options
!
      integer AL4SAN_PRIORITY_MIN, AL4SAN_PRIORITY_MAX
      parameter ( AL4SAN_PRIORITY_MIN = 0     )
      parameter ( AL4SAN_PRIORITY_MAX = 10000 )

!********************************************************************
!   AL4SAN constants - scheduler properties
!
      integer PRIORITY, CALLBACK, REDUX
      parameter ( PRIORITY = 16 )
      parameter ( CALLBACK = 17 )
      parameter ( REDUX    = 18 )
