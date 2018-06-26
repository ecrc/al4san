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
import shutil;
from optparse import OptionParser,OptionGroup;

from fileConversion import *;
from fileCopy import *;

def main(argv):

    # Create the options parser for detecting options on the command line.
    parser = OptionParser(usage="Usage: %prog [options]");
    group = OptionGroup(parser,"Printing Options","These options control generate_altanalSrc.py.");
    group.add_option("-y","--force"     , help=': Force overwrite all files'             , action='store_true', dest='force'    , default=False);
    group.add_option("-o","--prefix"    , help=': install files in PREFIX.'              , action='store'     , dest='prefix'   , type='string', default=False);
    group.add_option("-p","--plasma-dir", help=': Specify the top directory of PLASMA.'  , action='store'     , dest='pdir'     , type='string', default=False);
    group.add_option("-m","--altanal-dir" , help=': Specify the top directory of ALTANAL.'   , action='store'     , dest='mdir'     , type='string', default=False);
    group.add_option("-f","--file"      , help=': Specify a file(s) on which to operate.', action='store'     , dest='flist'    , type='string', default=False);
    parser.add_option_group(group);
    (options, args) = parser.parse_args();

    # Create files from PLASMA
    if options.prefix and options.pdir and options.flist:
        print '\n#####################'
        print '\nCONVERT PLASMA FILES '
        print '\n#####################'
        for file in options.flist.split():
            convert_file(file, options.pdir, options.prefix, options.force)

    # Copy file from ALTANAL
    if options.prefix and options.mdir:
        print '\n#####################'
        print '\nCOPY NEEDED SVN ALTANAL'
        print '\n#####################'
        copy_file(options.mdir, options.prefix, options.force)

    # Exit
    return 0

if "__main__" == __name__:
    sys.exit(main(sys.argv))
