/* /////////////////////////////////////////////////////////////////////////
 * File:        test/component/test.component.inserters.windows.hive/implicit_link.cpp
 *
 * Purpose:     Implicit link file for the test.component.inserters.windows.hive project.
 *
 * Created:     26th January 2017
 * Updated:     26th January 2017
 *
 * Status:      Wizard-generated
 *
 * License:     (Licensed under the Synesis Software Open License)
 *
 *              Copyright (c) 2017, Synesis Software Pty Ltd.
 *              All rights reserved.
 *
 *              www:        http://www.synesis.com.au/software
 *
 * ////////////////////////////////////////////////////////////////////// */


/* Pantheios header files */
#include <pantheios/implicit_link/core.h>
#include <pantheios/implicit_link/fe.simple.h>
#include <pantheios/implicit_link/be.test.h>

/* xTests header files */
#include <xtests/implicit_link.h>

/* UNIXem header files */
#if defined(PLATFORMSTL_OS_IS_UNIX) && \
    defined(_WIN32)
# include <unixem/implicit_link.h>
#endif /* PLATFORMSTL_OS_IS_UNIX && _WIN32 */

/* ////////////////////////////////////////////////////////////////////// */
