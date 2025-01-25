/* /////////////////////////////////////////////////////////////////////////
 * File:    test/scratch/test.scratch.bec.AnsiConsole/test.scratch.bec.AnsiConsole.c
 *
 * Purpose: Scratch test program for demonstrating be.AnsiConsole.
 *
 * Created: 21st October 2024
 * Updated: 25th January 2025
 *
 * ////////////////////////////////////////////////////////////////////// */


#include <pantheios/pantheios.h>


/* /////////////////////////////////////////////////////////////////////////
 * process identity
 *
 * This is required for when we link with one of several stock front-end
 * libraries - such as fe.all and fe.simple - that implement
 * pantheios_fe_getProcessIdentity() in terms of this externally defined
 * array
 */

/* Declared in include:pantheios/frontend.h */
PAN_CHAR_T const PANTHEIOS_FE_PROCESS_IDENTITY[] = PANTHEIOS_LITERAL_STRING("test.scratch.bec.AnsiConsole");


/* /////////////////////////////////////////////////////////////////////////
 * main()
 */

int main(int argc, char* argv[])
{
    ((void)&argc);
    ((void)&argv);


    pantheios_logprintf(PANTHEIOS_SEV_EMERGENCY, "emergency");
    pantheios_logprintf(PANTHEIOS_SEV_ALERT, "alert");
    pantheios_logprintf(PANTHEIOS_SEV_CRITICAL, "critical");
    pantheios_logprintf(PANTHEIOS_SEV_ERROR, "error");
    pantheios_logprintf(PANTHEIOS_SEV_WARNING, "warning");
    pantheios_logprintf(PANTHEIOS_SEV_NOTICE, "notice");
    pantheios_logprintf(PANTHEIOS_SEV_INFORMATIONAL, "informational");
    pantheios_logprintf(PANTHEIOS_SEV_DEBUG, "debug");


    return EXIT_SUCCESS;
}


/* ///////////////////////////// end of file //////////////////////////// */

