# -*- Makefile -*-
include Make.inc

CSHOBJS = timing/timing.o\
timing/time_dsygv.o\
coreblas/core_dlarfy.o\
coreblas/core_dsbtype1cb.o\
coreblas/core_dsbtype2cb.o\
coreblas/core_dsbtype3cb.o\
coreblas/core_dtrdalg1.o\
compute/Second_Stage.o\
compute/eig_dpotrf.o\
compute/eig_dplgsy.o\
compute/eig_dtrsm.o\
compute/eig_dtrmm.o\
compute/eig_dsymm.o\
compute/eig_dsyr2k.o\
compute/First_Stage.o\
compute/eig_pdsbcpy_t2bl.o\
compute/eig_pdsytrd_hb2st.o\
compute/cham_eig_pdsygst.o\
compute/eig_pdsytrd_sy2sb.o\
codelets/al4san/codelet_dplgsy.o\
codelets/al4san/codelet_dlacpy.o\
codelets/al4san/codelet_dlacpy1.o\
codelets/al4san/codelet_dlatro.o\
codelets/al4san/codelet_dlatro1.o\
codelets/al4san/codelet_dtrdalg1.o\
codelets/al4san/codelet_dpotrf.o\
codelets/al4san/codelet_dtrsm.o\
codelets/al4san/codelet_dgemm.o\
codelets/al4san/codelet_dsyrk.o\
codelets/al4san/codelet_dtrmm.o\
codelets/al4san/codelet_dsymm.o\
codelets/al4san/codelet_dsyr2k.o\
codelets/al4san/codelet_dhe2ge.o\
codelets/al4san/codelet_dgeqrt.o\
codelets/al4san/codelet_dsyrfb.o\
codelets/al4san/codelet_dormqr.o\
codelets/al4san/codelet_dlaset.o\
codelets/al4san/codelet_dtpmqrt.o\
codelets/al4san/codelet_dtsmqr_hetra1.o\
codelets/al4san/codelet_dtpqrt.o\
codelets/al4san/codelet_dsygst.o

all: potrf

lib:
	(cd lib; make all)

potrf: $(CSHOBJS)
	$(LINK) $(LDFLAGS) $(CSHOBJS) -o $@ $(LIBS)

.PHONY: clean all timing/timing

clean:
	(rm -f potrf *.o timing/*.o compute/*.o codelets/*.o codelets/quark/*.o codelets/al4san/*.o *~)

