#!/bin/sh
set -x

SRCDIR_TO_ANALYZE="build compute control coreblas example include runtime testing timing"

echo $PWD
rm -f filelist.txt
for dir in ${SRCDIR_TO_ANALYZE}
do
    find $dir -name '*\.[ch]' >> filelist.txt
done

# Remove all CMakeFiles generated file
sed -i '/CMakeFiles/d' filelist.txt

# Remove installed files
sed -i '/build\/install.*/d' filelist.txt

# Remove original files used for precision generation
for file in `git grep "@precisions" | awk -F ":" '{ print $1 }'`
do
    sed -i "\:^$file.*:d" filelist.txt
done

# Remove external header files
for file in coreblas/include/coreblas/cblas.h coreblas/include/coreblas/lapacke.h coreblas/include/coreblas/lapacke_config.h coreblas/include/coreblas/lapacke_mangling.h
do
    sed -i "\:^$file.*:d" filelist.txt
done
