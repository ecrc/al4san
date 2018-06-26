/**
 *
 * @file auxiliary.h
 *
 * @copyright 2009-2014 The University of Tennessee and The University of
 *                      Tennessee Research Foundation. All rights reserved.
 * @copyright 2012-2016 Bordeaux INP, CNRS (LaBRI UMR 5800), Inria,
 *                      Univ. Bordeaux. All rights reserved.
 *
 ***
 *
 * @brief Chameleon auxiliary header
 *
 * @version 1.0.0
 * @author Jakub Kurzak
 * @author Piotr Luszczek
 * @author Emmanuel Agullo
 * @author Cedric Castagnede
 * @date 2010-11-15
 *
 */
#ifndef _ALTANAL_AUXILIARY_H_
#define _ALTANAL_AUXILIARY_H_

#include "altanal/altanal_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Internal routines
 */
void altanal_warning      (const char *func_name, const char* msg_text);
void altanal_error        (const char *func_name, const char* msg_text);
void altanal_fatal_error  (const char *func_name, const char* msg_text);
int  altanal_rank         (ALTANAL_context_t *altanal);
int  altanal_tune         (ALTANAL_enum func, int M, int N, int NRHS);

/**
 *  API routines
 */
int  ALTANAL_Version      (int *ver_major, int *ver_minor, int *ver_micro);
int  ALTANAL_Element_Size (int type);
int  ALTANAL_My_Mpi_Rank  (void);

#ifdef __cplusplus
}
#endif

#endif
