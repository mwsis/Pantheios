/* /////////////////////////////////////////////////////////////////////////
 * File:    pantheios/implicit_link/bec.console.h
 *
 * Purpose: Implicitly links in the Pantheios Console Back-End Common Library
 *
 * Created: 3rd July 2009
 * Updated: 16th December 2023
 *
 * Home:    http://pantheios.org/
 *
 * Copyright (c) 2019-2023, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2009-2019, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - Neither the name(s) of Matthew Wilson and Synesis Software nor the
 *   names of any contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


/** \file pantheios/implicit_link/bec.console.h
 *
 * [C, C++] Implicitly links in the
 *   \ref group__backend__stock_backends__console "Pantheios Console Back-End Common Library"
 *   common implementation (that may be used by sole/local/remote back-end).
 */

#ifndef PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE
#define PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE


/* /////////////////////////////////////////////////////////////////////////
 * version information
 */

#ifndef PANTHEIOS_DOCUMENTATION_SKIP_SECTION
# define PANTHEIOS_VER_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE_MAJOR      1
# define PANTHEIOS_VER_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE_MINOR      0
# define PANTHEIOS_VER_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE_REVISION   1
# define PANTHEIOS_VER_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE_EDIT       4
#endif /* !PANTHEIOS_DOCUMENTATION_SKIP_SECTION */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#ifndef PANTHEIOS_INCL_PANTHEIOS_H_PANTHEIOS
# include <pantheios/pantheios.h>
#endif /* !PANTHEIOS_INCL_PANTHEIOS_H_PANTHEIOS */

#ifndef PLATFORMSTL_INCL_PLATFORMSTL_H_PLATFORMSTL
# include <platformstl/platformstl.h>
#endif /* !PLATFORMSTL_INCL_PLATFORMSTL_H_PLATFORMSTL */

#if defined(PLATFORMSTL_OS_IS_WINDOWS)
# ifndef PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_WINDOWSCONSOLE
#  include <pantheios/implicit_link/bec.WindowsConsole.h>
# endif /* !PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_WINDOWSCONSOLE */
#else /* ? OS */
# ifndef PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_FPRINTF
#  include <pantheios/implicit_link/bec.fprintf.h>
# endif /* !PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_FPRINTF */
#endif /* OS */

/* ////////////////////////////////////////////////////////////////////// */

#endif /* !PANTHEIOS_INCL_PANTHEIOS_IMPLICIT_LINK_H_BEC_CONSOLE */

/* ///////////////////////////// end of file //////////////////////////// */

