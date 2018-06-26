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
#  @version 1.0.0
#  @author Cedric Castagnede
#  @author Emmanuel Agullo
#  @author Mathieu Faverge
#  @date 2012-07-13
#   
###

subs = {
  # --------------------------------------------------------
  # replacements applied to ALL files first.
  'all_begin' : [
    ( 'u', None, None ),
  ],

  # ------------------------------------------------------------
  # replacements applied to compute files.
  'all_compute' : [
    # Get information in static function to allocate workspace
    ( 'r', '#include([\s\S]*)plasma_private_alloc\(([^,]*),([\s\S]*?),([^)]*)\)',         '//WS_ALLOC : \\3\n#include\\1'                                ),
    # end get

    ( 'u', 'plasma_static_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([^;]*);',               'altanal_\\3(,\\4;'                                              ),
    ( 'u', 'plasma_dynamic_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([^;]*);',              'altanal_\\3(,\\4;'                                              ),
    ( 'u', 'plasma_parallel_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([^;]*);',             'altanal_\\3(,\\4;'                                              ),
    ( 'u', 'plasma_static_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([\s\S]*?)request\)',    'altanal_\\3(,\\4)'                                              ),
    ( 'u', 'plasma_dynamic_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([\s\S]*?)request\)',   'altanal_\\3(,\\4)'                                              ),
    ( 'u', 'plasma_parallel_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([\s\S]*?)request\)',  'altanal_\\3(,\\4)'                                              ),
    # Dirty replacement to put the correct call of 'altanal_pz***` by removing all types
    # The 8 first lines are called n times more to be sure to change all `plasma_desc_mat_free(&desc` to `ALTANAL_Runtime_desc_getoncpu(`
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)PLASMA_enum([ \w\*]*),([^;]*);',             'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)PLASMA_desc([ \w\*]*),([^;]*);',             'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)PLASMA_Complex64_t([ \w\*]*),([^;]*);',      'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)PLASMA_sequence([ \w\*]*),([^;]*);',         'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)PLASMA_request([ \w\*]*),([^;]*);',          'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)int([ \w\*]*),([^;]*);',                     'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)float([ \w\*]*),([^;]*);',                   'altanal_p\\1(\\2,\\5;'                                          ),
    ( 'r', 'altanal_p([\w]*)\(([^;]*),([\s\w]*)double([ \w\*]*),([^;]*);',                  'altanal_p\\1(\\2,\\5;'                                          ),

    ( 'u', 'altanal_p([\w]*)\(([, ]*)',                                                     'altanal_p\\1('                                                  ),
  ],
  #
  #
  'compute' : [
    # Check the 2 next lines when plasma const will be right
    ( 'u', 'OUTOFPLACE([^}]+)plasma_zooptile2lap\(([\s]*)([^,]+),([^}]+)plasma_dynamic_sync\(\);([\s]*)plasma_desc_mat_free([^}]+)}',
           'OUTOFPLACE\\1altanal_zooptile2lap(\\3,\\4ALTANAL_Runtime_barrier(altanal);\\5ALTANAL_Runtime_desc_getoncpu(&\\3);\\5plasma_desc_mat_free\\6}'                    ),
    ( 'u', 'OUTOFPLACE([^}]+)ALTANAL_Runtime_desc_getoncpu([^;]+);([\s]*)([^}]+)}([\s\S]*)_Tile\(([\s\S]*)plasma_dynamic_sync\(\);([\s]*)status = sequence->status',
           'OUTOFPLACE\\1ALTANAL_Runtime_desc_getoncpu\\2;\\4}\\5_Tile(\\6ALTANAL_Runtime_barrier(altanal);\\7\\4\\7status = sequence->status'                             ),
    # Dirty replacement for ALTANAL_z*_Tile to put ALTANAL_Runtime_desc_getoncpu
    # The two first lines are called 10 times more to be sure to change all `plasma_desc_mat_free(&desc` to `ALTANAL_Runtime_desc_getoncpu(`
    ( 'r', '_Tile\(([\s\S]*)ALTANAL_Runtime_barrier\(altanal\);([^}]*)plasma_desc_mat_free\(&desc([^}]*)status = sequence->status',
           '_Tile(\\1ALTANAL_Runtime_barrier(altanal);\\2ALTANAL_Runtime_desc_getoncpu(\\3status = sequence->status'                                                       ),

    # Specific change for zplghe.c, zplgsy.c, zplrnt.c
    # TODO: it works because it is the last call in the function
    #       we need to find better delimiters
    ( 'u', '_zplghe\(([\s\S]*)\n([ \t]*)plasma_ziptile2lap\(([^;]*);',
           '_zplghe(\\1\n\\2ALTANAL_Runtime_barrier(altanal);\n\\2altanal_zooptile2lap(\\3;' ),
    ( 'u', '_zplgsy\(([\s\S]*)\n([ \t]*)plasma_ziptile2lap\(([^;]*);',
           '_zplgsy(\\1\n\\2ALTANAL_Runtime_barrier(altanal);\n\\2altanal_zooptile2lap(\\3;' ),
    ( 'u', '_zplrnt\(([\s\S]*)\n([ \t]*)plasma_ziptile2lap\(([^;]*);',
           '_zplrnt(\\1\n\\2ALTANAL_Runtime_barrier(altanal);\n\\2altanal_zooptile2lap(\\3;' ),
    # end specific

    # Remove INPLACE / OUTOFPLACE
    ( 'u', '\n([^\n]*)OUTOFPLACE([^\n]*)\n([\s\S]*?)\n([^\n]*)else([^\n]*)\n([\s\S]*?)\n([^\n]*)}([^\n]*)\n',
           '\n/*\\1OUTOFPLACE\\2*/\n\\3\n/*\\4else\\5*/\n\\6\n/*\\7}\\8*/\n'                                                                             ),
    ( 'r', '\n([^\n]*?)OUTOFPLACE([^}]*?)}([\s]*)else([^}]*?)\n([ ])([^\n]*?)\n([^}]*?)}',
           '\n\\1OUTOFPLACE\\2} else\\4\n/*\\5\\6*/\n\\7}'                                                                                               ),
    ( 'u', '\n([ ]+)([\s]*)plasma_ziptile2lap([^;]*);([ \t]*)\n',                         '\n/*\\1\\2plasma_ziptile2lap\\3;\\4*/\n'                      ),
    ( 'u', '\n([ ]+)([\s]*)plasma_ziplap2tile([^;]*);([ \t]*)\n',                         '\n/*\\1\\2plasma_ziplap2tile\\3;\\4*/\n'                      ),
    # end remove

    # Change plasma_desc_init into altanal_zdesc_alloc
    ( 'u', 'desc([\w]*)([ \t]*)=([ \t]*)plasma_desc_init\(([^,]*),([^;]*);([\s]*)([^;]*);', 'altanal_zdesc_alloc(desc\\1,\\5;'                             ),
    ( 'u', 'altanal_zdesc_alloc\(([^;]*),([\w\s]*)\*([\w\s]*),([^;]*);',                      'altanal_zdesc_alloc(\\1,\\4;'                                 ),
    ( 'u', 'altanal_zdesc_alloc\(([^;]*)\n([ \t]*)([^;]*);',                                  'altanal_zdesc_alloc(\\1 \\3;'                                 ),
    ( 'u', 'altanal_zdesc_alloc\(desc([\w]*)([^;]*)\);',                                   'altanal_zdesc_alloc(desc\\1\\2, altanal_desc_mat_free(&desc\\1));' ),
    # end chhange

    # Remove desc in Async
    ( 'u', 'desc([\w]*)\.d',                                                              '\\1->d'                                                       ),
    ( 'u', 'desc([\w]*)\.i',                                                              '\\1->i'                                                       ),
    ( 'u', 'desc([\w]*)\.j',                                                              '\\1->j'                                                       ),
    ( 'u', 'desc([\w]*)\.l',                                                              '\\1->l'                                                       ),
    ( 'u', 'desc([\w]*)\.m',                                                              '\\1->m'                                                       ),
    ( 'u', 'desc([\w]*)\.n',                                                              '\\1->n'                                                       ),
    ( 'r', '_Tile_Async\(([\s\S]*)\n([ \t]*)PLASMA_desc([ \t]*)([\w]*);([ \t]*)\n([\s\S]*)\n}\n',
           '_Tile_Async(\\1\n\\6\n}\n'                                                                                                                   ),
    ( 'r', '_Tile_Async\(([\s\S]*)\n([ \t]*)desc([\w]*)([ \t]*)=([ \t]*)\*\\3;([ \t]*)\n([\s\S]*)\n}\n',
           '_Tile_Async(\\1\n\\7\n}\n'                                                                                                                   ),
    ( 'r', '_Tile_Async\(([\s\S]*)\n([ \t]*)}([ \t]*)else([ \t]*){([\s]*)}([ \t]*)\n([\s\S]*)\n}\n' ,
           '_Tile_Async(\\1\n\\2}\n\\7\n}\n'                                                                                                             ),
    ( 'r', '_Tile_Async\(([\s\S]*)altanal_p([\w]*)\(([^;]*)desc([a-zA-Z0-9]+)([^;]*);([\s\S]*)\n}\n' ,
           '_Tile_Async(\\1altanal_p\\2(\\3\\4\\5;\\6\n}\n'                                                                                                ),
    # end remove

    # Patch for altanal_desc_submatrix (this will not work with 2-sided and LU inversion)
    ( 'r', '_Tile_Async\(([\s\S]*)^([\s]*)altanal_p([\w]*)\(([^;]*)plasma_desc_submatrix\(([\s]*)([a-zA-Z0-9]+),([\s\S]*)\),([^;]*);',
           '_Tile_Async(\\1\\2sub\\6 = altanal_desc_submatrix(\\5\\6,\\7);\n\\2altanal_p\\3(\\4sub\\6,\\8;\n\\2free(sub\\6);'                                ),
    ( 'r', '_Tile_Async\(([^)]*)\)([\s]*){([\s\S]*)free\(sub([\w]*)\)',
           '_Tile_Async(\\1)\\2{\n\tALTANAL_desc_t *sub\\4;\\3FLAGFREE(sub\\4)'                                                                            ),
    ( 'r', '_Tile_Async\(([^)]*)\)([\s]*){([\s\S]*)ALTANAL_desc_t \*sub([\w]*);([\s\S]*)\n^([\s]*)ALTANAL_desc_t \*sub\\4;',
           '_Tile_Async(\\1)\\2{\\3ALTANAL_desc_t *sub\\4;\\5'                                                                                             ),
    ( 'u', 'FLAGFREE',                                                                    'free'                                                         ),
    # end patch

    ( 'u', 'PLASMA_Dealloc_Handle_Tile',                                                  'ALTANAL_Dealloc_Workspace'                                      ),
    ( 'u', 'plasma_dynamic_sync\(\)',                                                     'ALTANAL_Runtime_barrier(altanal)'                                       ),
    ( 'u', 'QUARK_Barrier\(plasma->quark\)',                                              'ALTANAL_Runtime_barrier(altanal)'                                       ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'plasma_context_t',                                                            'ALTANAL_context_t'                                              ),
    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),

    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),

    # Fix for zgels et zgelqs
    ( 'u', 'ALTANAL_zgels_Tile_Async([\s\S]*)sub([\w]*) = ([\s\S]*?)altanal_pztile_zero([\s\S]*?)free([^;]*);',
           'ALTANAL_zgels_Tile_Async\\1/* sub\\2 = \\3altanal_pztile_zero\\4free\\5; */'                                                                     ),
    ( 'u', 'ALTANAL_zgelqs_Tile_Async([\s\S]*)sub([\w]*) = ([\s\S]*?)altanal_pztile_zero([\s\S]*?)free([^;]*);',
           'ALTANAL_zgelqs_Tile_Async\\1/* sub\\2 = \\3altanal_pztile_zero\\4free\\5; */'                                                                    ),

  ],

  # ------------------------------------------------------------
  # replacements applied to pcompute files.
  'pcompute' : [
    ( 'u', '#if 0([\s\S]*?)#endif',                                                       ''                                                             ),
    ( 'u', 'plasma_([\w]*)_quark\(',                                                      'altanal_\\1('                                                   ),
    ( 'u', '\*\*/([\s]*?)void([\s]*?)plasma_([\w]*?)\(([\s\S]*)}([\s]*?)/\*\*',           '**/\\1\n/**'                                                  ),
    ( 'u', 'static scheduling([\s\S]*)dynamic scheduling',                                'dynamic scheduling'                                           ),

    ( 'u', 'Quark_Task_Flags task_flags([\w]*) = Quark_Task_Flags_Initializer;',          'ALTANAL_option_t options\\1;'                                   ),
    ( 'u', 'QUARK_Task_Flag_Set\(([\s\S]*?)task_flags([\w]*)([\s]*),([^\n]*)\);',         'ALTANAL_Runtime_options_init(&options\\2, altanal, sequence, request);' ),
    ( 'u', 'plasma->quark, &task_flags([\w]*)',                                           '&options\\1'                                                  ),
    ( 'u', 'ALTANAL_Runtime_options_init\(&options([\w]*),([\s\S]*)}',
           'ALTANAL_Runtime_options_init(&options\\1,\\2\tALTANAL_Runtime_options_finalize(&options\\1, altanal);\n}'                                                      ),

    ( 'u', 'plasma_dynamic_sync\(\)',                                                     'ALTANAL_Runtime_barrier(altanal)'                                       ),
    ( 'u', 'QUARK_Barrier\(plasma->quark\)',                                              'ALTANAL_Runtime_barrier(altanal)'                                       ),

    ( 'u', 'PLASMA_desc([ \t]*)',                                                         'ALTANAL_desc_t\\1*'                                             ),
    ( 'u', 'plasma_context_t',                                                            'ALTANAL_context_t'                                              ),
    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),

    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'QUARK_CORE',                                                                  'ALTANAL_TASK'                                                   ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),
    ( 'u', '_quark',                                                                      ''                                                             ),

    ( 'u', 'ALTANAL_TASK([\w]*)\(([^;]*),([ \n\t]*)sequence([^;]*);',                       'ALTANAL_TASK\\1(\\2\\4;'                                        ),
    ( 'u', 'ALTANAL_TASK([\w]*)\(([^;]*),([ \n\t]*)request([^;]*);',                        'ALTANAL_TASK\\1(\\2\\4;'                                        ),
    ( 'u', '#define([\w\s]*)\(([\w\s]*),([\w\s]*)\)([ \t]*)BLKADDR\(([\S\s]*?),([\S\s]*?),([\S\s]*?),([\S\s]*?)\)\n',
           '#define\\1(\\2,\\3) \\5, \\7, \\8\n'                                                                                                         ),

    ( 'u', '([\w]*)\.d',                                                                  '\\1->d'                                                       ),
    ( 'u', '([\w]*)\.i',                                                                  '\\1->i'                                                       ),
    ( 'u', '([\w]*)\.j',                                                                  '\\1->j'                                                       ),
    ( 'u', '([\w]*)\.l',                                                                  '\\1->l'                                                       ),
    ( 'u', '([\w]*)\.m',                                                                  '\\1->m'                                                       ),
    ( 'u', '([\w]*)\.n',                                                                  '\\1->n'                                                       ),
  ],
  #
  #
  # specific patch because of dirty source code
  'pzgebrd_tb2bd.c' : [
    ( 'u', '#define A\(_m, _n\) \(ALTANAL_Complex64_t \*\)altanal_geteltaddr\(&A, \(_m\), \(_n\), eltsize\)',
           '#define A(_m,_n) BLKADDR(&dA, ALTANAL_Complex64_t, _m, _n)'                                                                                    ),
  ],
  'pzgetrf_reclap.c' : [
    ( 'u', '#define BLKLDD\(&dA, k\) \(A\)d.lm',                                          '#define BLKLDD(dA, k) (dA).lm'                                ),
    ( 'u', 'BLKLDD\(&dA',                                                                 'BLKLDD(dA'                                                    ),
  ],
  #
  #
  # Need to add specific information - not static implementation
  'pzgelqfrh.c' : [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  A->nb + ib*T->nb\n#include "common.h"'          ),
  ],
  'pzgeqrfrh.c' : [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  A->nb + ib*T->nb\n#include "common.h"'          ),
  ],
  'pzunglq.c': [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  ib*T->nb\n#include "common.h"'                  ),
  ],
  'pzunglqrh.c': [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  ib*T->nb\n#include "common.h"'                  ),
  ],
  'pzungqr.c': [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  ib*T->nb\n#include "common.h"'                  ),
  ],
  'pzungqrrh.c': [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  ib*T->nb\n#include "common.h"'                  ),
  ],
  'pzunmlqrh.c': [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  ib*T->nb\n#include "common.h"'                  ),
  ],
  'pzunmqrrh.c': [
    ( 'u', '#include "common.h"',                                                         '//WS_ALLOC :  ib*T->nb\n#include "common.h"'                  ),
  ],

  # end specific patch

  # ------------------------------------------------------------
  # replacements applied to pcompute files - (workspace)
  'pcompute_ws' : [
    # Compute the size of the workspace
    ( 'u', '#include "common.h"',                                     '//WS_ADD : \n#include "common.h"'                                                 ),
    ( 'u', '//WS_ALLOC : ([^\n]*)\n([\s\S]*?)//WS_ADD : ([^\n]*)\n',  '//ALLOC_WS : \\1\n\\2//WS_ADD : \\3\\1\n'                                         ),
    ( 'r', '//WS_ALLOC : ([^\n]*)\n([\s\S]*?)//WS_ADD : ([^\n]*)\n',  '//ALLOC_WS : \\1\n\\2//WS_ADD : \\3 +\\1\n'                                       ),
    # end compute
    ( 'u', '([\s\S]*?)WS_ADD : ([^\n]*)\n([\s\S]*?)^([\s]*)ib([\s]*)=([\s]*)ALTANAL_IB([\s]*);',
           '\\1WS_ADD : \\2\n\\3\\4ib\\5=\\6ALTANAL_IB\\7;\\4h_work_size  = sizeof(ALTANAL_Complex64_t)*(\\2 );\\4d_work_size  = 0;\\4ALTANAL_Runtime_options_ws_alloc( &options, h_work_size, d_work_size );\n' ),
    ( 'u', 'ALTANAL_Runtime_options_finalize\(&options, altanal\);',            'ALTANAL_Runtime_options_ws_free(&options);\n\tALTANAL_Runtime_options_finalize(&options, altanal);' ),
    ( 'u', 'ALTANAL_option_t options;',                                 'ALTANAL_option_t options;\n\tsize_t h_work_size, d_work_size;'                      ),
  ],

  # ------------------------------------------------------------
  # replacements applied to coreblas files.
  'coreblas' : [
    ( 'u', '#include "common.h"',                                                         '#include "coreblas.h"'                                        ),
    ( 'u', '#include "quark.h"',                                                          ''                                                             ),
    ( 'u', '#if defined\(PLASMA_HAVE_WEAK\)([\s\S]*?)#endif',                             ''                                                             ),
    ( 'u', 'int([\s]*)QUARK_CORE_([\w]*)\(([^)]*)\)([\s]*){([\s\S]*?)\n}',                ''                                                             ),
    ( 'u', 'void([\s]*)QUARK_CORE_([\w]*)\(([^)]*)\)([\s]*){([\s\S]*?)\n}',               ''                                                             ),
    ( 'u', 'void([\s]*)CORE([\w]*)_quark\(([^)]*)\)([\s]*){([\s\S]*?)\n}',                ''                                                             ),
    ( 'u', 'BLKADDR\(([ ]*)([\w]*),([^\n]*)',                                             'BLKADDR(&\\2,\\3'                                             ),
    ( 'u', 'BLKLDD\(([ ]*)([\w]*)([^\n]*)',                                               'BLKLDD(&\\2\\3'                                               ),

    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'PLASMA_Complex64_t',                                                          'ALTANAL_Complex64_t'                                            ),
    ( 'u', 'PLASMA_Complex32_t',                                                          'ALTANAL_Complex32_t'                                            ),
    ( 'u', 'PLASMA_enum',                                                                 'ALTANAL_enum'                                                   ),
    ( 'u', 'PLASMA_CORE',                                                                 'ALTANAL_CORE'                                                   ),
    ( 'u', 'PLASMA_SUCCESS',                                                              'ALTANAL_SUCCESS'                                                ),
    ( 'u', 'PLASMA_ERR_NOT_SUPPORTED',                                                    'ALTANAL_ERR_NOT_SUPPORTED'                                      ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),

    ( 'u', '/([\s\\*]*)/\n\n',                                                            ''                                                             ),
  ],

  # ------------------------------------------------------------
  # replacements applied to codelet_quark files.
  'codelet_quark' : [
    ( 'u', '#include "common.h"',                                                         '#include "altanal_quark.h"'                                     ),
    ( 'u', '#if defined\(PLASMA_HAVE_WEAK\)([\s\S]*?)#endif',                             ''                                                             ),

    ( 'u', '\n([\s\w]*)void([\s]*)CORE_([a-zA-Z0-9]*)\(([^)]*)\)([\s]*){([\s\S]*?)\n}',   ''                                                             ),
    ( 'u', '\n([\s\w]*)int([\s]*)CORE_([a-zA-Z0-9]*)\(([^)]*)\)([\s]*){([\s\S]*?)\n}',    ''                                                             ),
    ( 'u', '\n([\s\w]*)void([\s]*)CORE_([\w]*)_([^q])([a-zA-Z0-9]*)\(([^)]*)\)([\s]*){([\s\S]*?)\n}', ''                                                 ),
    ( 'u', '\n([\s\w]*)int([\s]*)CORE_([\w]*)_([^q])([a-zA-Z0-9]*)\(([^)]*)\)([\s]*){([\s\S]*?)\n}',  ''                                                 ),
    ( 'u', '\n([\s\w]*)void([\s]*)CORE_([a-zA-Z0-9]*)\(([^)]*)\);',                       ''                                                             ),
    ( 'u', '\n([\s\w]*)int([\s]*)CORE_([a-zA-Z0-9]*)\(([^)]*)\);',                        ''                                                             ),
    ( 'u', '\n([\s\w]*)void([\s]*)CORE_([\w]*)_([^q])([a-zA-Z0-9]*)\(([^)]*)\);',         ''                                                             ),
    ( 'u', '\n([\s\w]*)int([\s]*)CORE_([\w]*)_([^q])([a-zA-Z0-9]*)\(([^)]*)\);',          ''                                                             ),

    ( 'u', 'Quark([\s]*)\*quark,([\s]*)Quark_Task_Flags([\s]*)\*task_flags,',             'ALTANAL_option_t *options,'                                     ),
    ( 'u', 'plasma_sequence_flush',                                                       'ALTANAL_Runtime_sequence_flush'                                       ),
    ( 'u', 'QUARK_Insert_Task\(([ \t\n]*)quark',                                          'QUARK_Insert_Task(\\1options->quark'                          ),
    ( 'u', 'QUARK_Insert_Task\(([^)]*)task_flags',                                        'QUARK_Insert_Task(\\1options->task_flags'                     ),
    ( 'u', '&sequence,',                                                                  '&(options->sequence),'                                        ),
    ( 'u', '&request,',                                                                   '&(options->request),'                                         ),

    ( 'u', '\(([\s\S]*),([\s]*)PLASMA_sequence([^,]*)sequence',                           '(\\1'                                                         ),
    ( 'u', '\(([\s\S]*),([\s]*)PLASMA_request([^,]*)request',                             '(\\1'                                                         ),
    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),

    ( 'u', 'static inline \n',                                                            ''                                                             ),
    ( 'u', 'static \n',                                                                   ''                                                             ),

    ( 'r', 'QUARK_CORE([\w]*)\(([\s\S]*)const ([ \t\w]*)\*([\w]*),' ,                     'QUARK_CORE\\1(\\2\\3*\\4,'                                    ),
    ( 'r', 'QUARK_CORE([\w]*)\(([\s\S]*)PLASMA_Complex64_t([ \t]*)\*([\w]*),' ,           'QUARK_CORE\\1(\\2ALTANAL_desc_t *\\4, int \\4m, int \\4n,'      ),
    ( 'r', 'QUARK_CORE([\w]*)\(([\s\S]*)PLASMA_Complex32_t([ \t]*)\*([\w]*),' ,           'QUARK_CORE\\1(\\2ALTANAL_desc_t *\\4, int \\4m, int \\4n,'      ),
    ( 'r', 'QUARK_Insert_Task\(([^;]*)sizeof\(PLASMA_Complex64_t\)\*([\s\S]*?),([\s]*)([\w]*),',
           'QUARK_Insert_Task(\\1sizeof(ALTANAL_Complex64_t)*\\2,\\3RTBLKADR(\\4, ALTANAL_Complex64_t, \\4m, \\4n),'                                        ),
    ( 'r', 'QUARK_Insert_Task\(([^;]*)sizeof\(PLASMA_Complex32_t\)\*([\s\S]*?),([\s]*)([\w]*),',
           'QUARK_Insert_Task(\\1sizeof(ALTANAL_Complex32_t)*\\2,\\3RTBLKADR(\\4, ALTANAL_Complex32_t, \\4m, \\4n),'                                        ),
    ( 'u', 'RTBLKADR\(NULL, ALTANAL_Complex64_t, NULLm, NULLn\)' ,                         'NULL'                                                         ),

    ( 'u', 'QUARK_CORE',                                                                  'ALTANAL_TASK'                                                   ),
    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),

    ( 'u', 'qwrapper_([\w]*).c',                                                          'codelet_\\1.c'                                                ),
    ( 'u', 'core_blas quark wrapper',                                                     'codelets kernel'                                              ),

    # Add patch to remove REGION_D and REGION_U in codelet_zttlqt.c + codelet_zttqrt.c
    ( 'r', 'QUARK_Insert_Task\(([^;]*)CORE_zttqrt_quark([^;]*)\|([\s]*)QUARK_REGION_D([\s]*)\|([\s]*)QUARK_REGION_U',
           'QUARK_Insert_Task(\\1CORE_zttqrt_quark\\2' ),
    ( 'r', 'QUARK_Insert_Task\(([^;]*)CORE_zttlqt_quark([^;]*)\|([\s]*)QUARK_REGION_D([\s]*)\|([\s]*)QUARK_REGION_L',
           'QUARK_Insert_Task(\\1CORE_zttlqt_quark\\2' ),
    # end patch

    # Suppress additional functions (ex: gemm2, gemm_f2...)
    ( 'u', '\n([\s\w]*)([\w]*)([\s]*)CORE_zgemm_([\w]+)_quark\(([^)]*)\)([\s]*){([\s\S]*?)\n}' , ''                                                      ),
    ( 'u', '\n([\s\w]*)([\w]*)([\s]*)ALTANAL_TASK_zgemm_([\w]+)\(([^)]*)\)([\s]*){([\s\S]*?)\n}' , ''                                                      ),
    ( 'u', '\n([\s\w]*)([\w]*)([\s]*)CORE_zgemm([0-9]+)_quark\(([^)]*)\)([\s]*){([\s\S]*?)\n}' , ''                                                      ),
    ( 'u', '\n([\s\w]*)([\w]*)([\s]*)ALTANAL_TASK_zgemm([0-9]+)\(([^)]*)\)([\s]*){([\s\S]*?)\n}' , ''                                                      ),
    ( 'u', '\n([\s\w]*)([\w]*)([\s]*)CORE_ztrmm_([\w]+)_quark\(([^)]*)\)([\s]*){([\s\S]*?)\n}' , ''                                                      ),
    ( 'u', '\n([\s\w]*)([\w]*)([\s]*)ALTANAL_TASK_ztrmm_([\w]+)\(([^)]*)\)([\s]*){([\s\S]*?)\n}' , ''                                                      ),
    ( 'u', '/\*([\s\*]*)\*/([\s]*)/\*([\s\*]*)\*/\n' ,                                           ''                                                      ),
    # end suppress

    # Special remove of Rnd64_jump
    ( 'u', '#define COMPLEX([\s\S]*)static unsigned long long int([\s]*)Rnd64_jump([\s\S]*)return([\s\w]*);([\s]*)}', ''                                 ),
    # end remove
  ],

  # ------------------------------------------------------------
  # replacements applied to codelet_starpu files.
  'codelet_starpu' : [
    # Transformation for cl_***_cpu_func
    ( 'u', '#include "altanal_quark.h"',                                                    '#include "altanal_starpu.h"'                                    ),
    ( 'u', 'void([ \t]*)CORE_([\w]*)_quark\(([^)]*)\)',                                   'static void cl_\\2_cpu_func(void *descr[], void *cl_arg)'     ),
    ( 'u', '\n([ \t]*)ALTANAL_sequence_t([ \t]*)\*sequence;([ \t]*)\n',                     '\n'                                                           ),
    ( 'u', '\n([ \t]*)ALTANAL_request_t([ \t]*)\*request;([ \t]*)\n',                       '\n'                                                           ),
    ( 'u', '\n([ \t]*)if([\s\S]*?)ALTANAL_Runtime_sequence_flush([^;]*);([ \t]*)\n',              '\n'                                                           ),
    ( 'u', 'int info;',                                                                   'int info = 0;'                                                ),
    ( 'u', 'quark_unpack_args_([\w]*)\(([\s]*)quark([\s]*),',                             'starpu_codelet_unpack_args(cl_arg,'                           ),
    ( 'u', 'starpu_codelet_unpack_args\(([^;]*),([\s]*)sequence([^)]*)',                  'starpu_codelet_unpack_args(\\1\\3'                            ),
    ( 'u', 'starpu_codelet_unpack_args\(([^;]*),([\s]*)request([^;]*)',                   'starpu_codelet_unpack_args(\\1\\3'                            ),
    ( 'r', 'starpu_codelet_unpack_args\(([^;]*),([\s]*)([^&\s]+)([^;]*);',                'starpu_codelet_unpack_args(\\1,\\2&\\3\\4;'                   ),
    ( 'r', 'RTBLKADR\(([ \t]*)([\w]+)([\s\S]*)ALTANAL_Complex64_t([ \t]*)\*\\2;([\s\S]*)\n([ \t]*)starpu_codelet_unpack_args([^;]*),([\s]*)&\\2([,\\)]+)',
      'RTBLKADR(\\1\\2\\3ALTANAL_Complex64_t\\4*\\2;\\5\n\\6\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[0]);\n\\6starpu_codelet_unpack_args\\7\\9'),

    # repeat: We need to repeat manually to increase the index of descr
    ( 'u', 'descr\[0\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[0\]',
           'descr[0]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[1]'                                                                      ),
    ( 'u', 'descr\[1\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[0\]',
           'descr[1]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[2]'                                                                      ),
    ( 'u', 'descr\[2\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[1\]',
           'descr[2]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[3]'                                                                      ),
    ( 'u', 'descr\[3\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[2\]',
           'descr[3]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[4]'                                                                      ),
    ( 'u', 'descr\[4\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[3\]',
           'descr[4]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[5]'                                                                      ),
    ( 'u', 'descr\[5\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[4\]',
           'descr[5]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[6]'                                                                      ),
    ( 'u', 'descr\[6\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[5\]',
           'descr[6]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[7]'                                                                      ),
    ( 'u', 'descr\[7\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[6\]',
           'descr[7]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[8]'                                                                      ),
    ( 'u', 'descr\[8\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[7\]',
           'descr[8]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[9]'                                                                      ),
    ( 'u', 'descr\[9\]\);([ \t\n]*)([\w]*) = \(ALTANAL_Complex64_t \*\)STARPU_MATRIX_GET_PTR\(descr\[8\]',
           'descr[9]);\\1\\2 = (ALTANAL_Complex64_t *)STARPU_MATRIX_GET_PTR(descr[9]'                                                                      ),
    # end repeat
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[0\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[0]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 1, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[1\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[1]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 2, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[2\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[2]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 3, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[3\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[3]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 4, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[4\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[4]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 5, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[5\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[5]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 6, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[6\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[6]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 7, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[7\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[7]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 8, cl_\\1_cpu_func)\n' ),
    ( 'r', 'cl_([\w]*)_cpu_func\(([\s\S]*?)STARPU_MATRIX_GET_PTR\(descr\[8\]\);([\s]*)starpu_codelet_unpack_args([\s\S]*)$',
      'TREATED_\\1_cpu_func(\\2STARPU_MATRIX_GET_PTR(descr[8]);\\3starpu_codelet_unpack_args\\4/*\n * Codelet definition\n */\nCODELETS_CPU(\\1, 9, cl_\\1_cpu_func)\n' ),
    ( 'u', 'TREATED',                                                                     'cl'                                                           ),
    # end Transformation

    # Transformation for ALTANAL_TASK
    ( 'u', '\n([ \t]*)DAG_CORE_([\w]*);\n',                                               '\n'                                                           ),
    ( 'u', 'QUARK_Insert_Task',                                                           'starpu_insert_task'                                           ),
    ( 'u', 'options->quark([\s\S]*?)options->task_flags,',                                'codelet,'                                                     ),
    ( 'u', 'ALTANAL_TASK_([\w]*)\(([^)]*)\)([\s]*){([\s]*)([\w])',
           'ALTANAL_TASK_\\1(\\2)\\3{\\4(void)nb;\\4struct starpu_codelet *codelet = &cl_\\1;\\4void (*callback)(void*) = options->profiling ? cl_\\1_callback : NULL;\\4\\5' ),
    ( 'r', 'starpu_insert_task\(([^;]*)\|([\s]*)LOCALITY([^;]*?)',              'starpu_insert_task(\\1\\3'                                              ),
    ( 'r', 'starpu_insert_task\(([^;]*)\|([\s]*)QUARK_REGION_D([^;]*?)',        'starpu_insert_task(\\1\\3'                                              ),
    ( 'r', 'starpu_insert_task\(([^;]*)\|([\s]*)QUARK_REGION_U([^;]*?)',        'starpu_insert_task(\\1\\3'                                              ),
    ( 'r', 'starpu_insert_task\(([^;]*)\|([\s]*)QUARK_REGION_L([^;]*?)',        'starpu_insert_task(\\1\\3'                                              ),
    ( 'r', 'starpu_insert_task\(([^;]*)sizeof\(ALTANAL_request_t([ \t]*)\*\)([\s\S]*?),([\s\S]*?),([\s\S]*?),([ \t]*)\n([ \t]*)sizeof',
           'starpu_insert_task(\\1sizeof'                                                                                                                ),
    ( 'r', 'starpu_insert_task\(([^;]*)sizeof\(ALTANAL_sequence_t([ \t]*)\*\)([\s\S]*?),([\s\S]*?),([\s\S]*?),([ \t]*)\n([ \t]*)sizeof',
           'starpu_insert_task(\\1sizeof'                                                                                                                ),
    ( 'r', 'starpu_insert_task\(([^;]*)sizeof\(([^,]*),([\s]*)RTBLKADR\(([^)]*)\),([\s]*)([\S]*)([\s]*),',
           'starpu_insert_task(\\1\\6,\\5RTBLKADR(\\4),'                                                                                                ),
    ( 'r', 'starpu_insert_task\(([^;]*)sizeof\(([^,]*),([\s]*)([\S]*)([\s]*),([\s]*)([\S]*)([\s]*),',
           'starpu_insert_task(\\1\\7,\\6\\4\\5,\\3CONV_BACKUP(\\2,'                                                                                     ),

    ( 'r', 'starpu_insert_task\(([^;]*)CONV_BACKUP([^;]*)',                               'starpu_insert_task(\\1sizeof\\2'                              ),
    ( 'r', 'starpu_insert_task\(([^;]*)VALUE([^;]*)',                                     'starpu_insert_task(\\1STVAL\\2'                               ),
    ( 'r', 'starpu_insert_task\(([^;]*)STVAL([^;]*)',                                     'starpu_insert_task(\\1STARPU_VALUE\\2'                        ),
    ( 'r', 'starpu_insert_task\(([^;]*)INOUT([^;]*)',                                     'starpu_insert_task(\\1STARPU_RW\\2'                           ),
    ( 'r', 'starpu_insert_task\(([^;]*)INPUT([^;]*)',                                     'starpu_insert_task(\\1STARPU_R\\2'                            ),
    ( 'r', 'starpu_insert_task\(([^;]*)OUTPUT([^;]*)',                                    'starpu_insert_task(\\1STARPU_W\\2'                            ),
    ( 'r', 'starpu_insert_task\(([^;]*)SCRATCH([^;]*)',                                   'starpu_insert_task(\\1STARPU_TREATED\\2'                      ),
    ( 'u', 'TREATED',                                                                     'SCRATCH'                                                      ),
    ( 'u', 'starpu_insert_task\(([^;]*),([\s]*) 0\);',
           'starpu_insert_task(\\1,\\2 STARPU_PRIORITY,\toptions->priority,\\2 STARPU_CALLBACK,\tcallback,\\2 0);'                                       ),
    # end Transformation

    # Special transformation for IPIV
    ( 'u', 'STARPU_([\w]*),([\s]*)IPIV,([\s]*)sizeof\(int\)([^,]*)',                      'STARPU_VALUE,\\2&IPIV,\\3sizeof(int*)'                        ),

    # Special remove
    ( 'u', 'ALTANAL_TASK_zlaset2\(([^)]*)\)([\s]*){([\s]*)\(void\)nb;',                     'ALTANAL_TASK_zlaset2(\\1)\\2{\\3'                               ),
    ( 'u', 'ALTANAL_TASK_zlaset\(([^)]*)\)([\s]*){([\s]*)\(void\)nb;',                      'ALTANAL_TASK_zlaset(\\1)\\2{\\3'                                ),
    ( 'u', 'ALTANAL_TASK_zplghe\(([^)]*)\)([\s]*){([\s]*)\(void\)nb;',                      'ALTANAL_TASK_zplghe(\\1)\\2{\\3'                                ),
    ( 'u', 'ALTANAL_TASK_zplrnt\(([^)]*)\)([\s]*){([\s]*)\(void\)nb;',                      'ALTANAL_TASK_zplrnt(\\1)\\2{\\3'                                ),
    ( 'u', 'ALTANAL_TASK_zplgsy\(([^)]*)\)([\s]*){([\s]*)\(void\)nb;',                      'ALTANAL_TASK_zplgsy(\\1)\\2{\\3'                                ),
    # end remove
    ( 'u', '/([\s\\*\\/]*?)/',                                                            ''                                                             ),
  ],

  # ------------------------------------------------------------
  # replacements applied to codelet_starpu files (workspace).
  'codelet_starpu_ws' : [
    # Suppress multiple SCRATCH
    ( 'r', 'starpu_insert_task\(([^;]*)\n^([\s]*)STARPU_SCRATCH,([\s]*)NULL,([^,]*),([^;]*)^([\s]*)STARPU_SCRATCH,([\s]*)NULL,([^,]*),',
           'starpu_insert_task(\\1\\5\\6STARPU_SCRATCH,\\7NULL,\\8,'                                                                                     ),
    ( 'u', '^([\s]*)STARPU_SCRATCH,([\s]*)NULL,([^,]*),',
           '\\1STARPU_VALUE,\\2&h_work, sizeof(ALTANAL_starpu_ws_t *),\n\\1STARPU_VALUE,\\2&d_work, sizeof(ALTANAL_starpu_ws_t *),'                          ),
    ( 'u', '^([ \t]*)starpu_insert_task', '\\1ALTANAL_starpu_ws_t *h_work = (ALTANAL_starpu_ws_t*)(options->ws_host);\n\\1ALTANAL_starpu_ws_t *d_work = (ALTANAL_starpu_ws_t*)(options->ws_device);\n\n\\1starpu_insert_task' ),
    # Modify cl_***_cpu_func
    ( 'u', 'static void cl_([\w]*)([^{]*?){', 'static void cl_\\1\\2{\n\tALTANAL_starpu_ws_t *h_work;\n\tALTANAL_starpu_ws_t *d_work;'                       ),
    ( 'r', 'ALTANAL_Complex64_t([\s]*)\*([\w]*);([\s\S]*?)^([\s]*)starpu_codelet_unpack_args\(([^;]*)\&\\2([,\)])([^;]*);',
      'ALTANAL_Complex64_tDONE\\1*\\2;\\3\\4starpu_codelet_unpack_args(\\5&\\2\\6\\7;\n\\4\\2 = (ALTANAL_Complex64_t*)ALTANAL_Runtime_starpu_ws_getlocal(h_work);'   ),
    ( 'r', 'ALTANAL_Complex64_tDONE([\s]*)\*([\w]*);([\s\S]*?)^([\s]*)starpu_codelet_unpack_args\(([^;]*)\&\\2([,\)])',
           'ALTANAL_Complex64_tDONE\\1*\\2;\\3\\4starpu_codelet_unpack_args(\\5CLSCRATCH\\6'                                                               ),
    ( 'r', 'starpu_codelet_unpack_args\(([^;]*)CLSCRATCH([,\)])([^;]*)CLSCRATCH([,\)])',
           'starpu_codelet_unpack_args(\\1\\3CLSCRATCH\\4'                                                                                               ),
    ( 'u', 'starpu_codelet_unpack_args\(([^;]*)CLSCRATCH',                                'starpu_codelet_unpack_args(\\1&h_work, &d_work'               ),
    ( 'u', 'ALTANAL_Complex64_tDONE',                                                       'ALTANAL_Complex64_t'                                            ),
    # Specifc transformation - change order of WORK and TAU
    ( 'u', 'WORK([^;]*)ALTANAL_Runtime_starpu_ws_getlocal([^;]*);([\s]*)TAU([^;]*)ALTANAL_Runtime_starpu_ws_getlocal([^;]*);',
           'TAU\\4ALTANAL_Runtime_starpu_ws_getlocal\\5;\\3WORK = TAU + max( m, n );'                                                                            ),
  ],

  # ------------------------------------------------------------
  # replacements applied to codelet_starpu files (cuda).
  'codelet_starpu_cuda' : [
    # Transformation for cl_***_cuda_func (cublas)
    ( 'u', 'static void cl_([\w]*)_cpu_func\(([^)]*)\)([\s]*){([\s\S]*?)}',
           'static void cl_\\1_cpu_func(\\2)\\3{\\4}\n\n#ifdef ALTANAL_USE_CUDA\nstatic void cl_\\1_cuda_func(\\2)\\3{\\4}\n#endif\n'                      ),
    ( 'u', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)return([\s]*);([\s\S]*?)}',         'cl_\\1_cuda_func(\\2)\\3{\\4\\6}'                      ),
    ( 'u', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)}',                'cl_\\1_cuda_func(\\2)\\3{\\4\n\tcudaThreadSynchronize();\n\treturn;\n}' ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)cblas_z([\s\S]*?)}',                'cl_\\1_cuda_func(\\2)\\3{\\4cublasZ\\5}'               ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)ALTANAL_Complex64_t([\s\S]*?)}',      'cl_\\1_cuda_func(\\2)\\3{\\4cuDoubleComplex\\5}'       ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)CBLAS_SADDR\(([\w]*)\)([\s\S]*?)}', 'cl_\\1_cuda_func(\\2)\\3{\\4\\5\\6}'                   ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)([\s]*)CblasColMajor,([\s\S]*?)}',  'cl_\\1_cuda_func(\\2)\\3{\\4\\6}'                      ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)\(CBLAS_([A-Z]*)\)([\w]*),([\s\S]*?)}',
           'cl_\\1_cuda_func(\\2)\\3{\\4lapack_const(\\6),\\7}'                                                                                          ),
    # end Transformation

    # Transformation for cl_***_cuda_func (geadd)
    ( 'u', 'cl_zgeadd_cuda_func\(([^)]*)\)([\s]*){([\s]*)int([\s\S]*?)}',                        'cl_zgeadd_cuda_func(\\1)\\2{\\3int j;\n\tint\\4}'      ),
    ( 'u', 'cl_zgeadd_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)CORE_zgeadd\(M, N, alpha, A, LDA, B, LDB\);([\s\S]*?)}',
           'cl_zgeadd_cuda_func(\\1)\\2{\\3if (M == LDA && M == LDB)\n\t\tcublasZaxpy(M*N, alpha, A, 1, B, 1);\n\telse {\n\t\tfor (j = 0; j < N; j++)\n\t\t\tcublasZaxpy(M, alpha, A + j*LDA, 1, B + j*LDB, 1);\n\t}\n\\4}' ),
    # end Transformation

    # Transformation for cl_***_cuda_func (magma)
    ( 'u', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)int info = 0;([\s\S]*?)}',     'cl_\\1_cuda_func(\\2)\\3{\\4int ret;\n\tint info = 0;\\5}'  ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)([\s]*)LAPACK_COL_MAJOR,([\s\S]*?)}', 'cl_\\1_cuda_func(\\2)\\3{\\4\\6}'                    ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)info = LAPACKE_([\w]*)_work([\s\S]*?)}',
           'cl_\\1_cuda_func(\\2)\\3{\\4ret = magma_\\5_gpu\\6}'                                                                                         ),
    ( 'r', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)int([\s\S]*?)LAPACKE_([\w]*)_work([\s\S]*?)}',
           'cl_\\1_cuda_func(\\2)\\3{\\4int ret;\n\tint\\5ret = magma_\\6_gpu\\7}'                                                                       ),
    ( 'u', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)ret = magma_([^;]*)\);([\s\S]*?)}',
           'cl_\\1_cuda_func(\\2)\\3{\\4ret = magma_\\5, &info);\\6}' ),
    ( 'u', 'cl_([\w]*)_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)ret = magma_([^;]*);([\s\S]*?)}',
      'cl_\\1_cuda_func(\\2)\\3{\\4ret = magma_\\5;\n\t if (ret != MAGMA_SUCCESS) {\n\t\tfprintf(stderr, "Error in MAGMA: %d\\\\n", ret);\n\t\texit(-1);\n\t}\\6}' ),
    # end Transformation

    # Transformation for cl_***_cuda_func (magmablas)
    ( 'u', 'cl_zlaset_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)magma_zlaset_gpu([\s\S]*?)}', 'cl_zlaset_cuda_func(\\1)\\2{\\3magmablas_zlaset\\4}'          ),
    # end Transformation

    # Speccific add
    ( 'u', 'cl_zlauum_cuda_func\(([^)]*)\)([\s]*){([\s\S]*?)int ret;([\s\S]*?)}',        'cl_zlauum_cuda_func(\\1)\\2{\\3int ret;\n\tint info = 0;\\4}'  ),
    #end add

    ( 'u', 'CODELETS_CPU\(([\w]*), ([\w]*), cl_([\w]*)_cpu_func\)',                       'CODELETS(\\1, \\2, cl_\\3_cpu_func, cl_\\3_cuda_func)'        ),
  ],

  # ------------------------------------------------------------
  # replacements applied to codelet_starpu files (opencl).
  'codelet_starpu_opencl' : [
    # Transformation for cl_***_opencl_func
    # end Transformation
  ],
  # ------------------------------------------------------------
  # replacements applied to specific headers
  'include_coreblas' : [
    ( 'u', 'void([ \t]*)QUARK_CORE_([^)]*)\);([^\n]*)\n',                                 ''                                                             ),
    ( 'u', 'int([ \t]*)QUARK_CORE_([^)]*)\);([^\n]*)\n',                                  ''                                                             ),
    ( 'u', 'void([ \t]*)CORE_([\w]*)_quark([^;]*);([^\n]*)\n',                            ''                                                             ),
    ( 'u', '/([^/]*)called by PLASMA([^/]*)/\n',                                          ''                                                             ),
    ( 'u', '/([^/]*)called by QUARK([^/]*)/\n',                                           ''                                                             ),
    ( 'u', '#ifdef COMPLEX([ \t]*)\n#endif([ \t]*)\n',                                    ''                                                             ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'PLASMA_Complex64_t',                                                          'ALTANAL_Complex64_t'                                            ),
    ( 'u', 'PLASMA_Complex32_t',                                                          'ALTANAL_Complex32_t'                                            ),
    ( 'u', 'PLASMA_enum',                                                                 'ALTANAL_enum'                                                   ),
    ( 'u', 'PLASMA_CORE',                                                                 'ALTANAL_CORE'                                                   ),
  ],
  #
  #
  'include_runtime' : [
    ( 'u', 'core_zblas.h',                                                                'runtime_z.h'                                                  ),
    ( 'u', 'core_zcblas.h',                                                               'runtime_zc.h'                                                 ),
    ( 'u', '_PLASMA_CORE_',                                                               '_ALTANAL_Runtime_'                                                    ),
    ( 'u', 'void([ \t]*)CORE_([^)]*)\);([^\n]*)\n',                                       ''                                                             ),
    ( 'u', 'int([ \t]*)CORE_([^)]*)\);([^\n]*)\n',                                        ''                                                             ),
    ( 'u', '#ifdef COMPLEX([ \t]*)\n#endif([ \t]*)\n',                                    ''                                                             ),
    ( 'u', '/([^/]*)serial kernels([^/]*)/\n',                                            ''                                                             ),
    ( 'u', '/([^/]*)called by QUARK([^/]*)/\n',                                           ''                                                             ),

    ( 'u', 'Quark([\s]*)\*quark,([\s]*)Quark_Task_Flags([\s]*)\*task_flags,',             'ALTANAL_option_t *options,'                                     ),
    ( 'u', 'PLASMA_sequence([^,]*)sequence\)',                                            ')'                                                            ),
    ( 'u', 'PLASMA_request([^,]*)request\)',                                              ')'                                                            ),
    ( 'u', 'PLASMA_sequence([^,]*)sequence,',                                             ''                                                             ),
    ( 'u', 'PLASMA_request([^,]*)request,',                                               ''                                                             ),
    ( 'u', 'void([ \t]*)QUARK_CORE_([^)]*),([\s]*)\);',                                   'void\\1QUARK_CORE_\\2);'                                      ),
    ( 'u', 'int([ \t]*)QUARK_CORE_([^)]*),([\s]*)\);',                                    'int\\1QUARK_CORE_\\2);'                                       ),
    ( 'r', 'QUARK_CORE([\w]*)\(([\s\S]*)const ([ \t\w]*)\*([\w]*),' ,                     'QUARK_CORE\\1(\\2\\3*\\4,'                                    ),
    ( 'r', 'QUARK_CORE([\w]*)\(([\s\S]*)PLASMA_Complex64_t([ \t]*)\*([\w]*),' ,           'QUARK_CORE\\1(\\2ALTANAL_desc_t *\\4, int \\4m, int \\4n,'      ),
    ( 'r', 'QUARK_CORE([\w]*)\(([\s\S]*)PLASMA_Complex32_t([ \t]*)\*([\w]*),' ,           'QUARK_CORE\\1(\\2ALTANAL_desc_t *\\4, int \\4m, int \\4n,'      ),

    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),

    ( 'u', 'QUARK_CORE',                                                                  'ALTANAL_TASK'                                                   ),
    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
  ],
  #
  #
  'include_quarkblas' : [
    ( 'u', 'core_zblas.h',                                                                'quark_zblas.h'                                                ),
    ( 'u', 'core_zcblas.h',                                                               'quark_zcblas.h'                                               ),
    ( 'u', '_PLASMA_CORE_',                                                               '_QUARK_'                                                      ),
    ( 'u', 'void([ \t]*)CORE_([a-zA-Z0-9]*)\(([^)]*)\);([^\n]*)\n',                       ''                                                             ),
    ( 'u', 'int([ \t]*)CORE_([a-zA-Z0-9]*)\(([^)]*)\);([^\n]*)\n',                        ''                                                             ),
    ( 'u', 'void([ \t]*)CORE_([\w]*)_(^q])([a-zA-Z0-9]*)\(([^)]*)\);([^\n]*)\n',          ''                                                             ),
    ( 'u', 'int([ \t]*)CORE_([\w]*)_([^q])([a-zA-Z0-9]*)\(([^)]*)\);([^\n]*)\n',          ''                                                             ),
    ( 'u', 'void([ \t]*)QUARK_CORE_([^)]*)\);([^\n]*)\n',                                 ''                                                             ),
    ( 'u', 'int([ \t]*)QUARK_CORE_([^)]*)\);([^\n]*)\n',                                  ''                                                             ),
    ( 'u', '/([^/]*)called by PLASMA([^/]*)/\n',                                          ''                                                             ),
    ( 'u', '/([^/]*) serial kernels([^/]*)/\n',                                           ''                                                             ),
    ( 'u', '#ifdef COMPLEX([ \t]*)\n#endif([ \t]*)\n',                                    ''                                                             ),

    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
  ],
  #
  #
  'include_altanal' : [
    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),
  ],

  # ------------------------------------------------------------
  # replacements applied to control files.
  'control' : [
    ( 'u', 'plasma_alloc_ipiv\(([\w]*), ([\w]*), PLASMA_FUNC_ZGESV, \(void([ ]*)\*\*\)IPIV\)',
           'altanal_alloc_ipiv(\\1, \\2, ALTANAL_FUNC_ZGESV, AltanalComplexDouble, descL, (void**)IPIV)'                                                       ),
    ( 'u', 'plasma_shared_alloc',                                                         'altanal_desc_mat_alloc'                                         ),
    ( 'u', 'Declarations of parallel functions \(static scheduling\)([\s\S]*?)Declarations of internal sequential functions',
           'Declarations of internal sequential functions'                                                                                               ),
    ( 'u', 'plasma_parallel_call_([\w]*)\(([\s\\\]*)plasma_([\w]*),([^;]*);',             'altanal_\\3(\\4;'                                               ),
    ( 'u', 'altanal_pzlapack_to_tile\(([^;]*?);',                                           'altanal_pzlapack_to_tile(A, lm, &descA, seq, req);'             ),
    ( 'u', 'altanal_pztile_to_lapack\(([^;]*?);',                                           'altanal_pztile_to_lapack(&descA, A, lm, seq, req);'             ),
    ( 'u', 'PLASMA_Dealloc_Handle_Tile',                                                  'ALTANAL_Dealloc_Workspace'                                      ),

    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),
    ( 'u', 'PLASMA_desc([ \t]*)([\w])',                                                   'ALTANAL_desc_t\\1*\\2'                                          ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'plasma_context_t',                                                            'ALTANAL_context_t'                                              ),
    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),
    ( 'u', '_quark',                                                                      ''                                                             ),

    # Add altanal_zdesc_alloc in compute_z.h
    ( 'u', '#define altanal_zdesc_alloc',
           '#define altanal_zdesc_alloc2(descA, mb, nb, lm, ln, i, j, m, n)         \\\n\tdescA = altanal_desc_init(                                          \\\n\t\tAltanalComplexDouble, (mb), (nb), ((mb)*(nb)),                  \\\n\t\t(m), (n), (i), (j), (m), (n));                                \\\n\taltanal_desc_mat_alloc( &(descA) );\n\n#define altanal_zdesc_alloc' ),
    # end add
  ],

  # ------------------------------------------------------------
  # replacements applied to timing files.
  'timing' : [
    ( 'u', 'PLASMA_Dealloc_Handle_Tile',                                                  'ALTANAL_Dealloc_Workspace'                                      ),
    ( 'u', 'PLASMA_sequence',                                                             'ALTANAL_sequence_t'                                             ),
    ( 'u', 'PLASMA_request',                                                              'ALTANAL_request_t'                                              ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'real_Double_t',                                                               'altanal_time_t'                                                 ),
    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),

    # Add dirty getoncpu( descA ), will need to handle that within the sequence for exemple
    ( 'u', '(ALTANAL_Sequence_Wait\([^;]*\);\n)([\s]*)STOP_TIMING',                         '\\1\\2ALTANAL_Desc_getoncpu( descA );\n\\2STOP_TIMING'          ),
  ],

  # ------------------------------------------------------------
  # replacements applied to timing files.
  'testing' : [
    ( 'u', 'core_blas.h',                                                                 'coreblas.h'                                                   ),
    ( 'u', 'testing_zmain.h',                                                             'testing_zauxiliary.h'                                         ),
    ( 'u', 'real_Double_t',                                                               'altanal_time_t'                                                 ),
    ( 'u', 'int([\s]*)testing_([^{]*){',                                                  'int\\1testing_\\2{\n\tint hres = 0;'                          ),
    ( 'u', 'int([\s]*)testing_([\s\S]*?)return 0;',                                       'int\\1testing_\\2return hres;'                                ),
    ( 'u', 'int([\s]*)testing_([\s\S]*?)FAILED([^;]*?);',                                 'int\\1testing_\\2FAILED\\3;\thres++;'                         ),
    ( 'u', 'PLASMA_desc',                                                                 'ALTANAL_desc_t'                                                 ),
    ( 'u', 'PLASMA_Finalize',                                                             'ALTANAL_Finalize'                                               ),
    ( 'u', 'PLASMA_Dealloc_Handle_Tile',                                                  'ALTANAL_Dealloc_Workspace'                                      ),

    ( 'u', 'PLASMA',                                                                      'ALTANAL'                                                        ),
    ( 'u', 'Plasma',                                                                      'Altanal'                                                        ),
    ( 'u', 'plasma',                                                                      'altanal'                                                        ),

    # Specific fix for testing_zgesv_incpiv (will be fix in plasma)
    ( 'u', 'int testing_zgesv_incpiv\(([\s\S]*)ALTANAL_Complex64_t \*L;',                   'int testing_zgesv_incpiv(\\1ALTANAL_desc_t *L;'                 ),
  ],

  # --------------------------------------------------------
  # replacements applied to ALL at the end.
  'all_end' : [
    ( 'u', 'provided by Univ. of Tennessee,',                           'provided by Inria Bordeaux - Sud-Ouest, LaBRI,'                                 ),
    ( 'u', 'Univ. of California Berkeley and Univ. of Colorado Denver', 'University of Bordeaux, Bordeaux INP'                                          ),
    ( 'u', '@version 2.6.0\n \* @author', 
           '@version 2.6.0\n * @comment This file has been automatically generated\n *          from Plasma 2.6.0 for ALTANAL 1.0.0\n * @author'           ),
    ( 'u', '/([\*]+)/\n/([\*]+)/',                                                        ''                                                             ),
    ( 'u', '/([\*]+)/\n/',                                                                '\n/'                                                          ),
    ( 'u', '/([\*]+)/([a-zA-Z]+)',                                                        '\\2'                                                          ),
  ],
};

adds = {
  # --------------------------------------------------------
  # replacements applied to ALL files.
  'all' : [
    ( 'u', None , None ),
  ],
};
