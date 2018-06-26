!**
!
! -- Inria
! -- (C) Copyright 2012
!
! This software is a computer program whose purpose is to process
! Matrices Over Runtime Systems @ Exascale (CHAMELEON). More information
! can be found on the following website: http://www.inria.fr/en/teams/altanal.
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
!   ALTANAL constants - precisions
!
      integer  AltanalByte, AltanalInteger, AltanalRealFloat
      integer  AltanalRealDouble, AltanalComplexFloat, AltanalComplexDouble
      parameter ( AltanalByte          = 0 )
      parameter ( AltanalInteger       = 1 )
      parameter ( AltanalRealFloat     = 2 )
      parameter ( AltanalRealDouble    = 3 )
      parameter ( AltanalComplexFloat  = 4 )
      parameter ( AltanalComplexDouble = 5 )

!********************************************************************
!   ALTANAL constants - boolean
!
      integer ALTANAL_FALSE, ALTANAL_TRUE
      parameter ( ALTANAL_FALSE = 0 )
      parameter ( ALTANAL_TRUE  = 1 )

!********************************************************************
!   State machine switches
!
      integer ALTANAL_WARNINGS, ALTANAL_ERRORS, ALTANAL_AUTOTUNING
      integer ALTANAL_DAG, ALTANAL_PROFILING_MODE, ALTANAL_PARALLEL_MODE
      integer ALTANAL_BOUND
      parameter ( ALTANAL_WARNINGS       = 1 )
      parameter ( ALTANAL_ERRORS         = 2 )
      parameter ( ALTANAL_AUTOTUNING     = 3 )
      parameter ( ALTANAL_DAG            = 4 )
      parameter ( ALTANAL_PROFILING_MODE = 5 )
      parameter ( ALTANAL_PARALLEL_MODE  = 6 )
      parameter ( ALTANAL_BOUND          = 7 )

!********************************************************************
!   ALTANAL constants - configuration  parameters
!
      integer ALTANAL_CONCURRENCY
      parameter ( ALTANAL_CONCURRENCY      = 1 )


!********************************************************************
!   ALTANAL constants - success & error codes
!
      integer ALTANAL_SUCCESS, ALTANAL_ERR_NOT_INITIALIZED
      integer ALTANAL_ERR_REINITIALIZED, ALTANAL_ERR_NOT_SUPPORTED
      integer ALTANAL_ERR_ILLEGAL_VALUE, ALTANAL_ERR_NOT_FOUND
      integer ALTANAL_ERR_OUT_OF_MEMORY, ALTANAL_ERR_INTERNAL_LIMIT
      integer ALTANAL_ERR_UNALLOCATED, ALTANAL_ERR_FILESYSTEM
      integer ALTANAL_ERR_UNEXPECTED, ALTANAL_ERR_SEQUENCE_FLUSHED
      parameter ( ALTANAL_SUCCESS             =    0 )
      parameter ( ALTANAL_ERR_NOT_INITIALIZED = -101 )
      parameter ( ALTANAL_ERR_REINITIALIZED   = -102 )
      parameter ( ALTANAL_ERR_NOT_SUPPORTED   = -103 )
      parameter ( ALTANAL_ERR_ILLEGAL_VALUE   = -104 )
      parameter ( ALTANAL_ERR_NOT_FOUND       = -105 )
      parameter ( ALTANAL_ERR_OUT_OF_MEMORY   = -106 )
      parameter ( ALTANAL_ERR_INTERNAL_LIMIT  = -107 )
      parameter ( ALTANAL_ERR_UNALLOCATED     = -108 )
      parameter ( ALTANAL_ERR_FILESYSTEM      = -109 )
      parameter ( ALTANAL_ERR_UNEXPECTED      = -110 )
      parameter ( ALTANAL_ERR_SEQUENCE_FLUSHED= -111 )

!********************************************************************
!   ALTANAL constants - kernels options
!
      integer ALTANAL_PRIORITY_MIN, ALTANAL_PRIORITY_MAX
      parameter ( ALTANAL_PRIORITY_MIN = 0     )
      parameter ( ALTANAL_PRIORITY_MAX = 10000 )

!********************************************************************
!   ALTANAL constants - scheduler properties
!
      integer PRIORITY, CALLBACK, REDUX
      parameter ( PRIORITY = 16 )
      parameter ( CALLBACK = 17 )
      parameter ( REDUX    = 18 )
