#!/bin/bash

STARPU_SILENT=1 MKL_NUM_THREADS=1 OMP_NUM_THREADS=1 ./potrf --threads=36 --gpus=1 --m=1024 --k=1024 --n=1024 --nb=256 --ib=64  --nowarmup
