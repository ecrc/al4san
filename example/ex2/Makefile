# -*- Makefile -*-
include Make.inc

CSHOBJS = potrf.o codelets/codelet_dgemm.o codelets/codelet_dpotrf.o codelets/codelet_dtrsm.o codelets/codelet_dsyrk.o

all: potrf 

lib:
	(cd lib; make all)

potrf: $(CSHOBJS)
	$(LINK) $(LDFLAGS) $(CSHOBJS) -o $@ $(LIBS)

.PHONY: clean all potrf

clean:
	(rm -f *.o codelets/*.o potrf *~)

