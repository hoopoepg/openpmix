/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2004-2008 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2015      Los Alamos National Security, LLC. All rights
 *                         reserved.
 * Copyright (c) 2016-2020 Intel, Inc.  All rights reserved.
 * Copyright (c) 2021-2022 Nanook Consulting.  All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 *
 */

#include "src/include/pmix_config.h"
#include "pmix_common.h"

#include "pmdl_mpich.h"
#include "src/mca/pmdl/pmdl.h"
#include "src/util/pmix_argv.h"

static pmix_status_t component_register(void);
static pmix_status_t component_query(pmix_mca_base_module_t **module, int *priority);

/*
 * Instantiate the public struct with all of our public information
 * and pointers to our public functions in it
 */
pmix_pmdl_mpich_component_t pmix_mca_pmdl_mpich_component = {
    .super = {
        PMIX_PMDL_BASE_VERSION_1_0_0,

        /* Component name and version */
        .pmix_mca_component_name = "mpich",
        PMIX_MCA_BASE_MAKE_VERSION(component,
                                   PMIX_MAJOR_VERSION,
                                   PMIX_MINOR_VERSION,
                                   PMIX_RELEASE_VERSION),

        /* Component open and close functions */
        .pmix_mca_register_component_params = component_register,
        .pmix_mca_query_component = component_query,
    },
    .include = NULL,
    .exclude = NULL
};

static pmix_status_t component_register(void)
{
    pmix_mca_base_component_t *component = &pmix_mca_pmdl_mpich_component.super;

    pmix_mca_pmdl_mpich_component.incparms = "MPIR_CVAR*";
    (void) pmix_mca_base_component_var_register(
        component, "include_envars",
        "Comma-delimited list of envars to harvest (\'*\' and \'?\' supported)",
        PMIX_MCA_BASE_VAR_TYPE_STRING,
        &pmix_mca_pmdl_mpich_component.incparms);
    if (NULL != pmix_mca_pmdl_mpich_component.incparms) {
        pmix_mca_pmdl_mpich_component.include = pmix_argv_split(pmix_mca_pmdl_mpich_component.incparms, ',');
    }

    pmix_mca_pmdl_mpich_component.excparms = NULL;
    (void) pmix_mca_base_component_var_register(
        component, "exclude_envars",
        "Comma-delimited list of envars to exclude (\'*\' and \'?\' supported)",
        PMIX_MCA_BASE_VAR_TYPE_STRING,
        &pmix_mca_pmdl_mpich_component.excparms);
    if (NULL != pmix_mca_pmdl_mpich_component.excparms) {
        pmix_mca_pmdl_mpich_component.exclude = pmix_argv_split(pmix_mca_pmdl_mpich_component.excparms, ',');
    }

    return PMIX_SUCCESS;
}

static pmix_status_t component_query(pmix_mca_base_module_t **module, int *priority)
{
    *priority = 40;
    *module = (pmix_mca_base_module_t *) &pmix_pmdl_mpich_module;
    return PMIX_SUCCESS;
}
