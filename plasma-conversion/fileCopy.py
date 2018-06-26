#!/usr/bin/env python
# -*- coding: utf-8 -*-
###
#
# -- Inria
# -- (C) Copyright 2012
#
# This software is a computer program whose purpose is to process
# Matrices Over Runtime Systems @ Exascale (ALTANAL). More information
# can be found on the following website: http://www.inria.fr/en/teams/altanal.
# 
# This software is governed by the CeCILL-C license under French law and
# abiding by the rules of distribution of free software.  You can  use, 
# modify and/ or redistribute the software under the terms of the CeCILL-C
# license as circulated by CEA, CNRS and INRIA at the following URL
# "http://www.cecill.info". 
# 
# As a counterpart to the access to the source code and  rights to copy,
# modify and redistribute granted by the license, users are provided only
# with a limited warranty  and the software's author,  the holder of the
# economic rights,  and the successive licensors  have only  limited
# liability. 
# 
# In this respect, the user's attention is drawn to the risks associated
# with loading,  using,  modifying and/or developing or reproducing the
# software by the user in light of its specific status of free software,
# that may mean  that it is complicated to manipulate,  and  that  also
# therefore means  that it is reserved for developers  and  experienced
# professionals having in-depth computer knowledge. Users are therefore
# encouraged to load and test the software's suitability as regards their
# requirements in conditions enabling the security of their systems and/or 
# data to be ensured and,  more generally, to use and operate it in the 
# same conditions as regards security. 
# 
# The fact that you are presently reading this means that you have had
# knowledge of the CeCILL-C license and that you accept its terms.
#
###
#
#  @project MORSE
#  MORSE is a software package provided by:
#     Inria Bordeaux - Sud-Ouest,
#     Univ. of Tennessee,
#     King Abdullah Univesity of Science and Technology
#     Univ. of California Berkeley,
#     Univ. of Colorado Denver. 
# 
#  @author Cedric Castagnede
#  @author Emmanuel Agullo
#  @author Mathieu Faverge
#  @date 2012-07-13
#   
###

import sys;
import os;
import shutil;
import string;


def copyDirectoryTree(directory, destination, mode):
    pattern = shutil.ignore_patterns('.svn', '*~', 'plasma-conversion', 'insert-licence')
    for entry in os.listdir(directory):
        entryPath = os.path.join(directory, entry)
        destPath  = os.path.join(destination, entry)
        if os.path.isdir(entryPath):
            if entry == '.svn'             or \
               entry == 'insert-licence'   or \
               entry == 'plasma-conversion':
                continue
            else:
                if os.path.exists(destPath):
                    copyDirectoryTree(entryPath, destPath, mode)

                else:
                    print 'write:',destPath
                    shutil.copytree(entryPath, destPath, ignore=pattern)

        else:
            #if not os.path.exists(destPath):
                if entry == 'plasma_2.6.0.tar.gz':
                    continue
                else:
                    print 'write:',destPath
                    shutil.copy(entryPath, destination)
            #else:
                #print 'not write:',destPath
                #print 'already exists:',destPath
                #if mode is True:
                #    ask = "y"

                #else:
                #    ask = ""

                #while (ask != "y") and (ask != "n"):
                #    ask = raw_input('overwrite (y/n)? ')

                #if ask == "y":
                #    print 'overwrite:',destPath
                #    shutil.copy(entryPath, destination)

                #if ask == "n":
                #    print 'stop overwrite:',destPath
                #    sys.exit(0)


def copy_file(srcpath, dstpath, mode):

    if os.path.exists(srcpath) is True:
        dstpath2  = os.path.join(dstpath, '')
        copyDirectoryTree(srcpath, dstpath2, mode)

    else:
        print 'error:',srcpath,'does not exist'
        sys.exit(1)

    return;
