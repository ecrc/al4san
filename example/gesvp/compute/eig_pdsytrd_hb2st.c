//#include "common.h"
#include <al4san.h>
#include <runtime/al4san_quark.h>
/***************************************************************************/
/**
 *  Parallel bulge chasing column-wise - dynamic scheduling
 **/
/***************************************************************************/
void eig_pdsytrd_hb2st_v1_quark(int uplo, int N, int NB, int Vblksiz,
                                double *A, int LDA,
                                double *V, double *TAU,
                                double *D, double *E, int WANTZ,
                                AL4SAN_sequence_t *sequence, AL4SAN_request_t *request)
{
    AL4SAN_context_t *al4sanctxt;
    AL4SAN_option_t options;
    /*===========================
     *   local variables
     *===========================*/
    int INgrsiz, INthgrsiz;
    int myid, grsiz, shift=3, stt, st, ed, stind, edind;
    int blklastind, colpt, PCOL, ACOL, MCOL;
    int stepercol, mylastid, grnb, grid;
    int *DEP,*MAXID;
    int i, sweepid, m;
    int thgrsiz, thgrnb, thgrid, thed;
     size_t ws_worker = 0;
    size_t ws_host = 0;
    /* variables used when this function is standalone */
#ifdef COMPLEX
    int standalonework = 0 ;
    static double zone  = (double) 1.0;
#endif
    al4sanctxt = al4san_context_self();
    if (al4sanctxt == NULL) {
        al4san_fatal_error("eig_pdsytrd_hb2st_v1_quark", "AL4SAN not initialized");
        return AL4SAN_ERR_NOT_INITIALIZED;
    }

    if (sequence->status != AL4SAN_SUCCESS) 
        {al4san_fatal_error("eig_pdsytrd_hb2st_v1_quark", "AL4SAN not initialized");
        return;}

    if ( uplo != Al4sanLower ) {
        al4san_request_fail(sequence, request, AL4SAN_ERR_NOT_SUPPORTED);
        return;
    }
    /* Quick return */
    if (N == 0) {
        return;
    }
    ws_worker = NB;
    ws_worker *= sizeof(double);
    ws_host   *= sizeof(double);

   AL4SAN_Options_Init(&options, sequence, request);
  AL4SAN_Options_Workspace_Alloc(&options, ws_worker, ws_host );
    /*
     * The matrix is diagonal. We just copy it (abs for complex) and store it in D
     * sequential code here is better so only core 0 will work
     */
    if ( NB == 0 ) {
        memset(E, 0, (N-1)*sizeof(double));
#ifdef COMPLEX
        for (i=0; i<N; i++){
            D[i] = fabs(A[LDA*i]); /* A is band storage*/
        }
#else
        for (i=0; i<N; i++){
            D[i] = A[LDA*i];
        }
#endif
        return;
    }

    /*
     * The matrix is already Tridiagonal. We do not need to apply the bulge chasing.
     * We make diagonal and superdiagonal elements real, and store them in
     * D and E.
     * For Q, PT: ZSCAL should be done if WANTQ == 1.
     */
    /* =======================================================
     * NOTE in CASE USED STANDALONE FUNCTION
     * =======================================================*/
    /* in case where the first stage has been done we are sure
     * that all element of E are real so no need to go through
     * making the off diagonal element real just copy them to D and E.
     * However in case where this function is used for only
     * band matrices meaning only stage2 want to be called then it requires
     * to make all off-diagonal elements real and so remove this portion below
     * and uncomment the NB=1 case below.*/

    /* sequential code here so only core 0 will work */
    if ( NB == 1 ) {
        /* Case used when this function is standalone and so the off diagonal
         * element require to be real. (Only for complex) */
#ifdef COMPLEX
        if( standalonework != 0 ) {
            /*
             * PlasmaLower
             */
            if( uplo == Al4sanLower ) {
                double ztmp, absztmp;

                TAU[0] = zone;
                for (i=0; i<N-1; i++) {
                    D[i]       = (A[LDA*i]);
                    ztmp       = A[LDA*i+1];
                    absztmp    = fabs(ztmp);
                    A[LDA*i+1] = absztmp;
                    E[i]       = absztmp;
                    if( absztmp != 0. )
                        ztmp = (double) (ztmp / (double) absztmp);
                    else
                        ztmp = (double)1.;
                    if( i < N-2 ) {
                        A[LDA*(i+1)+1] *= ztmp;
                    }
                    // for Q: ZSCAL should be done in case of WANTQ
                    TAU[i+1] = ztmp;
                }
                D[N-1] = (A[LDA*(N-1)]);
            }
            /*
             * PlasmaUpper
             */
            else {
                al4san_error("pdsytrd_hb2st_v1", "Al4san Upper is not supported\n");
                al4san_request_fail(sequence, request, AL4SAN_ERR_NOT_SUPPORTED);
                return;
            }
        }
        else
#endif
        {
            /* Case used when first stage has been applied or all real cases */
            if( uplo == Al4sanLower ){
                for (i=0; i < N-1; i++) {
                    D[i] = (A[i*LDA]);
                    E[i] = (A[i*LDA+1]);
                }
                D[N-1] = (A[(N-1)*LDA]);
            }
            else {
                for (i=0; i < N-1; i++) {
                    D[i] = (A[i*LDA+NB]);
                    E[i] = (A[i*LDA+NB-1]);
                }
                D[N-1] = (A[(N-1)*LDA+NB]);
                E[N-1] = 0.;
            }
        }
        return;
    }

    /*
     * General case:
     *
     * As I store V in the V vector there are overlap between
     * tasks so shift is now 4 where group need to be always
     * multiple of 2 (or shift=5 if not multiple of 2),
     * because as example if grs=1 task 2 from
     * sweep 2 can run with task 6 sweep 1., but task 2 sweep 2
     * will overwrite the V of tasks 5 sweep 1 which are used by
     * task 6, so keep in mind that group need to be multiple of 2,
     * and thus tasks 2 sweep 2 will never run with task 6 sweep 1.
     * OR if we allocate V as V(N,2) and we switch between the storing of
     * sweep's like odd in V(N,1) and even in V(N,2) then no overlap and so
     * shift is 3.
     * when storing V in matrix style, shift could be back to 3.
     * */
    /***************************************************************************
     *                       START BULGE CHASING CODE
     **************************************************************************/
    /* General case NB > 1 && N > NB */
    DEP   = (int *)malloc((N+1)*sizeof(int));
    MAXID = (int *)malloc( (N+1)*sizeof(int));
    memset(MAXID,0,(N+1)*sizeof(int));
     AL4SAN_Barrier();
 //   QUARK_Barrier((Quark*)(al4sanctxt->schedopt));
    /*
     * Initialisation of local parameter. those parameter should be
     * input or tuned parameter.
     */
    INgrsiz = 1;
    if( NB > 160 ) {
        INgrsiz = 2;
    }
    else if( NB > 100 ) {
        if( N < 5000 )
            INgrsiz = 2;
        else
            INgrsiz = 4;
    } else {
        INgrsiz = 6;
    }
    INthgrsiz = N;

    grsiz   = INgrsiz;
    thgrsiz = INthgrsiz;
    if( grsiz   == 0 ) grsiz   = 6;
    if( thgrsiz == 0 ) thgrsiz = N;

    i = shift/grsiz;
    stepercol =  i*grsiz == shift ? i:i+1;

    i       = (N-1)/thgrsiz;
    thgrnb  = i*thgrsiz == (N-1) ? i:i+1;

    #if defined (ENABLE_DEBUG)
    printf("  Dynamic bulgechasing version v9_9col threads  %4d   threads_used  %4d   N %5d      NB %5d    grs %4d thgrsiz %4d  WANTZ %4d\n",al4sanctxt->world_size, al4sanctxt->world_size, N, NB, grsiz,thgrsiz,WANTZ);
    #endif

    for (thgrid = 1; thgrid<=thgrnb; thgrid++){
        stt  = (thgrid-1)*thgrsiz+1;
        thed = min( (stt + thgrsiz -1), (N-1));
        for (i = stt; i <= N-1; i++){
            ed=min(i,thed);
            if(stt>ed)break;
            for (m = 1; m <=stepercol; m++){
                st=stt;
                for (sweepid = st; sweepid <=ed; sweepid++){
                    /* PCOL:  dependency on the ID of the master of the group of the previous column.  (Previous Column:PCOL). */
                    /* ACOL:  dependency on the ID of the master of the previous group of my column.   (Acctual  Column:ACOL). (it is 0(NULL) for myid=1) */
                    /* MCOL:  OUTPUT dependency on the my ID, to be used by the next ID. (My Column: MCOL). I am the master of this group. */
                    myid     = (i-sweepid)*(stepercol*grsiz) +(m-1)*grsiz + 1;
                    mylastid = myid+grsiz-1;
                    PCOL     = mylastid+shift-1;  /* to know the dependent ID of the previous column. need to know the master of its group*/
                    MAXID[sweepid] = myid;
                    PCOL     = min(PCOL,MAXID[sweepid-1]); /* for the last columns, we might do only 1 or 2 kernel, so the PCOL will be wrong. this is to force it to the last ID of the previous col.*/
                    grnb     = PCOL/grsiz;
                    grid     = grnb*grsiz == PCOL ? grnb:grnb+1;
                    PCOL     = (grid-1)*grsiz +1; /* give me the ID of the master of the group of the previous column.*/
                    ACOL     = myid-grsiz;
                    if(myid==1)ACOL=0;
                    MCOL     = myid;

                    EIG_AL4SAN_CORE_dtrdalg1(
                        &options,
                        N, NB, A, LDA,
                        V, TAU,
                        Vblksiz, WANTZ,
                        i, sweepid, m, grsiz,
                        &DEP[PCOL], &DEP[ACOL], &DEP[MCOL] );

                    if(mylastid%2 ==0){
                        blklastind      = (mylastid/2)*NB+1+sweepid-1;
                    }else{
                        colpt      = ((mylastid+1)/2)*NB + 1 +sweepid -1 ;
                        stind      = colpt-NB+1;
                        edind      = min(colpt,N);
                        if( (stind>=edind-1) && (edind==N) )
                            blklastind=N;
                        else
                            blklastind=0;
                    }
                    if(blklastind >= (N-1))  stt=stt+1;
                } /* END for sweepid=st:ed    */
            } /* END for m=1:stepercol */
        } /* END for i=1:N-2      */
    } /* END for thgrid=1:thgrnb     */

     /*
     * Barrier used only for now, to be sure that everything
     * is done before copying the D and E and free workspace.
     * this will be removed later when D and E are directly filled
     * during the bulge process.
     */
    AL4SAN_Barrier();
    free( DEP);
    free( MAXID);
    /*================================================
     *  store resulting diag and lower diag D and E
     *  note that D and E are always real
     *================================================*/
    /*
     * STORE THE RESULTING diagonal/off-diagonal in D AND E
     */
    /* Make diagonal and superdiagonal elements real,
     * storing them in D and E
     */
    /* In complex case, the off diagonal element are
     * not necessary real. we have to make off-diagonal
     * elements real and copy them to E.
     * When using HouseHolder elimination,
     * the ZLARFG give us a real as output so, all the
     * diagonal/off-diagonal element except the last one are already
     * real and thus we need only to take the abs of the last
     * one.
     *  */
    /* sequential code here so only core 0 will work */
    if( uplo == Al4sanLower ) {
        for (i=0; i < N-1 ; i++) {
            D[i] = (A[i*LDA]);
            E[i] = (A[i*LDA+1]);
        }
        D[N-1] = (A[(N-1)*LDA]);
    } else { /* PlasmaUpper not tested yet */
        for (i=0; i<N-1; i++) {
            D[i] = (A[i*LDA+NB]);
            E[i] = (A[i*LDA+NB-1]);
        }
        D[N-1] = (A[(N-1)*LDA+NB]);
    } /* end PlasmaUpper */
  AL4SAN_Options_Workspace_Free(&options);
  AL4SAN_Options_Finalize(&options);
    return;
}
/***************************************************************************/
