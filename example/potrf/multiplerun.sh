#!/bin/bash

PROGRAM=potrf

export IFS=";"

#THREADS="72"
#MSIZES="2048"
#BSIZES="256"

#for MS in $MSIZES; do
  #for BS in $BSIZES; do
   # for thread in $THREADS; do
   #   NX_THREADS=$thread ./$PROGRAM $MS $BS 0
  #  done
 # done
#done

#MSIZES="1024;2048;4096;6144;8192;10240;12288;14336;16384;18432;20480;22528;24576;26624;28672;30720;32768;34816;36864;38912;40960"
MSIZES2="51200;61440;71680;81920;102400;122880;143360"
MSIZES1="20480;30720;40960"
BSIZES1="256;512;768;1024"
BSIZES2="512;768;1024;1280;1536;1792"
REPEAT="1;2;3"
MSIZES3="153600;163840;174080;184320;194560;204800"
MSIZES4="163840"


for MS in $MSIZES1; do
      for (( BS=256; BS<=1024; BS=BS+256)); do
       for RP in $REPEAT ;do
            ./$PROGRAM --n=$MS --nrhs=$MS --nb=$BS --threads=55 --gpus=0 --p=1 --q=1 --runtime="Parsec"
        done 
    done
done


for MS in $MSIZES2; do
      for (( BS=512; BS<=2048; BS=BS+256)); do
       for RP in $REPEAT ;do
            ./$PROGRAM --n=$MS --nrhs=$MS --nb=$BS --threads=55 --gpus=0 --p=1 --q=1 --runtime="Parsec"
        done 
    done
done
