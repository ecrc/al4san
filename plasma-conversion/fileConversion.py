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
import string;

import re;
import shutil;
from rulesSubstitutions import subs;

def read_file(file):
    fd = open(file, 'r')
    filedata = fd.read()
    fd.close()
    return filedata


def write_file(file, filedata):
    fd = open(file, 'w')
    fd.write(filedata)
    fd.close()


def update_file(file, filedata):
    # read previous data
    fd = open(file, 'r')
    data = fd.read()
    fd.close()

    # write new + previous data
    fd = open(file, 'w')
    fd.write(filedata + data)
    fd.close()


def create_path(path):
    path = os.path.abspath(path)
    if os.path.exists(path) is False:
        print 'create:',path
        os.makedirs(path)

def substitute_file(srcfilepath, dstfilepath, filetype):
    print 'substitute : apply >>>',filetype,'<<< dictionnary'
    print '             for',dstfilepath

    # Initialize filedata
    filedata = read_file(srcfilepath)

    # Subsitute in data
    work = subs[filetype]
    for pattern in work:
        condition = pattern[0]
        search    = pattern[1]
        replace   = pattern[2]
        if not search:
            continue
        if condition == 'u':
            filedata = re.sub(re.compile(search, re.M), replace, filedata)
        if condition == 'r':
            data_p = re.sub(re.compile(search, re.M), replace, filedata)
            while data_p != filedata:
                filedata = data_p
                data_p = re.sub(re.compile(search, re.M), replace, filedata)
            filedata = data_p

    # calll the last treatment to remove tabulation
    filedata = re.sub(re.compile('\t', re.M), '    ', filedata)

    # Write file
    write_file(dstfilepath, filedata)



def change_file(srcfile, dstfile, filetype, mode):

    if os.path.exists(srcfile):
        create_path(os.path.dirname(dstfile))

        if os.path.exists(dstfile):
            print 'already exists:',dstfile
            if mode is True:
                ask = "y"
            else:
                ask = ""
            while (ask != "y") and (ask != "n"):
                ask = raw_input('overwrite (y/n)? ')

            if ask == "y":
                print 'overwrite:',dstfile
                substitute_file(srcfile, dstfile, filetype)

            if ask == "n":
                print 'stop overwrite::',dstfile
                sys.exit(0)

        else:
            print 'write: from',srcfile
            print '       into',dstfile
            substitute_file(srcfile, dstfile, filetype)

    else:
        print 'error:',srcfile,'does not exist'
        sys.exit(1)

    return;


def convert_file(file, plasma_src, prefix, mode):
    srcpath = os.path.abspath(plasma_src)
    dstpath = os.path.join(os.path.abspath(prefix),'')

    filename = os.path.basename(file)
    filetype = file[:- len(filename)-1]

    srcfilepath = os.path.join(srcpath,file)
    dstfilepath = os.path.join(dstpath,filetype)

    if filetype == 'core_blas':
        # coreblas
        dstpath_core = os.path.join(dstpath, 'coreblas/compute')
        dstfilepath_core = os.path.join(dstpath_core, filename)
        change_file(srcfilepath,      dstfilepath_core, 'coreblas', mode)
        change_file(dstfilepath_core, dstfilepath_core, 'all_end',  True)

    elif filetype == 'core_blas-qwrapper':
        
        # coreblas path
        dstpath_core = os.path.join(dstpath, 'coreblas/compute')
        dstfilepath_core = os.path.join(dstpath_core, filename)

        if filename == 'qwrapper_zgetf2_nopiv.c'  or \
           filename == 'qwrapper_zpemv.c'         or \
           filename == 'qwrapper_zparfb.c':
            print 'CODELET:', filename, 'do not need to be generated'

        else:
            # codelet for quark
            filename = filename.replace('qwrapper', 'codelet')
            dstpath_quark  = os.path.join(dstpath, 'runtime/quark/codelets')
            dstfilepath_quark  = os.path.join(dstpath_quark, filename)
            change_file(srcfilepath, dstfilepath_quark, 'codelet_quark', mode)

            # codelet for starpu
            filename = filename.replace('core', 'codelet')
            dstpath_starpu = os.path.join(dstpath, 'runtime/starpu/codelets')
            dstfilepath_starpu = os.path.join(dstpath_starpu, filename)
            change_file(dstfilepath_quark, dstfilepath_starpu, 'codelet_starpu', mode)

            # codelet for starpu with WS
            if filename == 'codelet_zgelqt.c' or \
               filename == 'codelet_zgeqrt.c' or \
               filename == 'codelet_ztslqt.c' or \
               filename == 'codelet_ztsmlq.c' or \
               filename == 'codelet_ztsmqr.c' or \
               filename == 'codelet_ztsqrt.c' or \
               filename == 'codelet_ztstrf.c' or \
               filename == 'codelet_zttlqt.c' or \
               filename == 'codelet_zttmlq.c' or \
               filename == 'codelet_zttmqr.c' or \
               filename == 'codelet_zttqrt.c' or \
               filename == 'codelet_zunmlq.c' or \
               filename == 'codelet_zunmqr.c':
                change_file(dstfilepath_starpu, dstfilepath_starpu, 'codelet_starpu_ws', True)

            # codelet for starpu with cuda
            if filename == 'codelet_zgemm.c'  or \
               filename == 'codelet_zhemm.c'  or \
               filename == 'codelet_zherk.c'  or \
               filename == 'codelet_zher2k.c' or \
               filename == 'codelet_zsymm.c'  or \
               filename == 'codelet_zsyrk.c'  or \
               filename == 'codelet_zsyr2k.c' or \
               filename == 'codelet_ztrsm.c'  or \
               filename == 'codelet_ztrmm.c'  or \
               filename == 'codelet_zlauum.c' or \
               filename == 'codelet_ztrtri.c' or \
               filename == 'codelet_zpotrf.c':
                change_file(dstfilepath_starpu, dstfilepath_starpu, 'codelet_starpu_cuda', True)

            change_file(dstfilepath_quark,  dstfilepath_quark,  'all_end', True)
            change_file(dstfilepath_starpu, dstfilepath_starpu, 'all_end', True)

    elif filetype == 'compute':
        # drivers (2 treatments is necessary)
        dstfilepath = os.path.join(dstfilepath,filename)
        change_file(srcfilepath, dstfilepath, 'all_compute', mode)

        if filename[0:2] == 'pz':
            change_file(dstfilepath, dstfilepath, 'pcompute',    True)

            # specific transformation
            if filename == 'pzgebrd_tb2bd.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzgetrf_reclap.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzgelqfrh.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzgeqrfrh.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzunglq.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzunglqrh.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzungqr.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzungqrrh.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzunmlqrh.c':
                change_file(dstfilepath, dstfilepath, filename,  True)
            if filename == 'pzunmqrrh.c':
                change_file(dstfilepath, dstfilepath, filename,  True)

            # specific transformation to add workspace
            if filename == 'pzgelqf.c'        or \
               filename == 'pzgelqfrh.c'      or \
               filename == 'pzgeqrf.c'        or \
               filename == 'pzgeqrfrh.c'      or \
               filename == 'pzgetmi2.c'       or \
               filename == 'pzgetrf_incpiv.c' or \
               filename == 'pzhetrd_hb2st.c'  or \
               filename == 'pzlange.c'        or \
               filename == 'pzlanhe.c'        or \
               filename == 'pzlansy.c'        or \
               filename == 'pzpack.c'         or \
               filename == 'pzshift.c'        or \
               filename == 'pzunglq.c'        or \
               filename == 'pzunglqrh.c'      or \
               filename == 'pzungqr.c'        or \
               filename == 'pzungqrrh.c'      or \
               filename == 'pzunmlq.c'        or \
               filename == 'pzunmlqrh.c'      or \
               filename == 'pzunmqr_blgtrd.c' or \
               filename == 'pzunmqr.c'        or \
               filename == 'pzunmqrrh.c':
                # codelet for starpu
                change_file(dstfilepath, dstfilepath, 'pcompute_ws', True)

        else:
            change_file(dstfilepath, dstfilepath, 'compute', True)

        change_file(dstfilepath, dstfilepath, 'all_end', True)

    elif filetype == 'include':
        if filename == 'core_zblas.h' or filename == 'core_zcblas.h':
            # include for coreblas
            filetype = 'include_coreblas'
            dstfilename = filename.replace('core_', 'coreblas_')
            dstfilename = dstfilename.replace('blas.h', '.h')
            dstfilepath = os.path.join(dstpath,'coreblas/include')
            dstfilepath = os.path.join(dstfilepath,dstfilename)
            change_file(srcfilepath, dstfilepath, filetype, mode)
            change_file(dstfilepath, dstfilepath, 'all_end', True)

            # include for runtime (insert_task)
            filetype = 'include_runtime'
            dstfilepath = os.path.join(dstpath,'include')
            newfile     = re.sub('core', 'runtime', filename)
            newfile     = re.sub('blas', '',        newfile)
            dstfilepath = os.path.join(dstfilepath,newfile)
            change_file(srcfilepath, dstfilepath, filetype, mode)
            change_file(dstfilepath, dstfilepath, 'all_end', True)

            # include quark_blas
            filetype = 'include_quarkblas'
            dstfilepath = os.path.join(dstpath,'runtime/quark/include')
            newfile     = re.sub('core', 'quark', filename)
            dstfilepath = os.path.join(dstfilepath,newfile)
            change_file(srcfilepath, dstfilepath, filetype, mode)
            change_file(dstfilepath, dstfilepath, 'all_end', True)

        elif filename == 'plasma_z.h' or filename == 'plasma_zc.h':
            # include for drivers
            filetype = 'include_altanal'
            dstfilepath = os.path.join(dstpath,'include')
            newfile     = re.sub('plasma', 'altanal', filename)
            dstfilepath = os.path.join(dstfilepath,newfile)
            change_file(srcfilepath, dstfilepath, filetype, mode)
            change_file(dstfilepath, dstfilepath, 'all_end', True)

        else:
            print '% >>>>>>>>>>>>>>>>>>>>>>>>>>>'
            print '% What do you think you do...'
            print '% <<<<<<<<<<<<<<<<<<<<<<<<<<<'
            sys.exit(1)

    elif filetype == 'control':
        newfile     = re.sub('plasma', 'altanal', filename)
        dstfilepath = os.path.join(dstfilepath,newfile)
        change_file(srcfilepath, dstfilepath, filetype, mode)
        change_file(dstfilepath, dstfilepath, 'all_end', True)

    else:
        # others files (timing, testing...)
        dstfilepath = os.path.join(dstfilepath,filename)
        change_file(srcfilepath, dstfilepath, filetype, mode)
        change_file(dstfilepath, dstfilepath, 'all_end', True)

    return;
