#####
 #
 #
 # @copyright 2018 King Abdullah University of Science and Technology (KAUST).
 #                     All rights reserved.
 #####
###
#
#  @project AL4SAN
#  AL4SAN is a software package provided by:
#     King Abdullah Univesity of Science and Technology
#
#  @version 0.1.0
#  @Eduardo Gonzalez 
#  @author Rabab Alomairy
#
#  @date 2018-05-19
#
###
PROGRAM=cholesky

TARGETS=$(PROGRAM)-p $(PROGRAM)-i $(PROGRAM)-d

#BASE_DIR=../..
#include $(BASE_DIR)/common-files/Makefile

#CC = g++
#CC = clang
#CC = icpc
CC = gcc
#CFLAGS = -g -fopenmp #-S -emit-llv
GFLAGS=
#CFLAGS_P =
#CFLAGS_I = --instrument
#CFLAGS_D = --debug

#LLVM_MODULES = core jit native

#CPPFLAGS = `llvm-config --cppflags` 
LDFLAGS = #`llvm-config --ldflags` 
#LIBS = `llvm-config --libs

MKL_CHECK=$(MKLROOT)/lib/intel64_lin/libmkl_sequential.so

LIBS  =  -L$(MKLROOT)/lib/intel64_lin/ -lm -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -L/home/omairyrm/al4san-dev/build-quark/installdir/lib -lal4san -lal4san_quark -L/home/omairyrm/quark -lquark -lhwloc -lpthread #`llvm-config --libs`
INCS  = -I$(MKLROOT)/include -I/home/omairyrm/al4san-dev/build-quark/installdir/include -I/home/omairyrm/quark/

#EXTRA = -std=c99 -O3 -Wall -Wno-unused 

$(PROGRAM)-p: $(PROGRAM).c $(MKL_CHECK)
	$(CC) $(GFLAGS) $(CFLAGS) $(LDFLAGS) $(INCS) -o $@ $< $(LIBS)

$(PROGRAM)-i: $(PROGRAM).c $(MKL_CHECK)
	$(CC) $(GFLAGS) $(CFLAGS) $(LDFLAGS) $(INCS) -o $@ $< $(LIBS)

$(PROGRAM)-d: $(PROGRAM).c $(MKL_CHECK)
	$(CC) $(GFLAGS) $(CFLAGS) $(LDFLAGS) $(INCS) -o $@ $< $(LIBS)


$(MKL_CHECK):
	@echo "=================================================="
	@echo "===           MKL LIBRARY NEEDED               ==="
	@echo "=================================================="
	@false

clean:
	rm -f $(CC)_* *.o *~ $(TARGETS) 


