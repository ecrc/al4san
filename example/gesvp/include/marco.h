EIG_USE_MPI
EIG_USE_CUDA



//added param

            iparam[IPARAM_NCUDAS],
#if defined(EIG_USE_MPI)
            iparam[IPARAM_NMPI],
            iparam[IPARAM_P], iparam[IPARAM_Q],
#endif


IPARAM_NMPI