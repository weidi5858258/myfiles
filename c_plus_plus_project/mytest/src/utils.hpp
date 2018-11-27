#ifndef MYTEST_UTILS_HPP
#define MYTEST_UTILS_HPP

#include "my_struct.h"

void redirect_output(bool to_file, const char *signal_name) {
    if (to_file && logfp != filelogfp) {
        if (signal_name) {
            fprintf(stderr, "\n%s received.", signal_name);
        }
        if (!filelogfp) {
            filelogfp = unique_create(DEFAULT_LOGFILE, false, &logfile);
            if (filelogfp) {
                fprintf(stderr, _("\nRedirecting output to %s.\n"),
                        quote(logfile));
                /* Store signal name to tell wget it's permanent redirect to log file */
                redirect_request_signal_name = signal_name;
                logfp = filelogfp;
                /* Dump the context output to the newly opened log.  */
                log_dump_context();
            } else {
                /* Eek!  Opening the alternate log file has failed.  Nothing we
                  can do but disable printing completely. */
                fprintf(stderr, _("%s: %s; disabling logging.\n"),
                        (logfile) ? logfile : DEFAULT_LOGFILE, strerror(errno));
                inhibit_logging = true;
            }
        } else {
            fprintf(stderr, _("\nRedirecting output to %s.\n"),
                    quote(logfile));
            logfp = filelogfp;
            log_dump_context();
        }
    } else if (!to_file && logfp != stdlogfp) {
        logfp = stdlogfp;
        log_dump_context();
    }
}

static void check_redirect_output(void) {
#ifndef WINDOWS
    /* If it was redirected already to log file by SIGHUP, SIGUSR1 or -o parameter,
     * it was permanent.
     * If there was no SIGHUP or SIGUSR1 and shell is interactive
     * we check if process is fg or bg before every line is printed.*/
    if (!redirect_request_signal_name && shell_is_interactive && !global_options.lfilename) {
        pid_t foreground_pgrp = tcgetpgrp(STDIN_FILENO);

        if (foreground_pgrp != -1 && foreground_pgrp != getpgrp()) {
            /* Process backgrounded */
            redirect_output(true, NULL);
        } else {
            /* Process foregrounded */
            redirect_output(false, NULL);
        }
    }
#endif /* WINDOWS */
}

void logprintf(enum log_options o, const char *fmt, ...) {
    va_list args;
    struct logvprintf_state lpstate;
    bool done;
    int errno_saved = errno;

    check_redirect_output();
    errno = errno_saved;
    if (inhibit_logging)
        return;
    CHECK_VERBOSE (o);

    xzero (lpstate);
    errno = 0;
    do {
        va_start (args, fmt);
        done = log_vprintf_internal(&lpstate, fmt, args);
        va_end (args);

        if (done && errno == EPIPE)
            exit(WGET_EXIT_GENERIC_ERROR);
    } while (!done);

    errno = errno_saved;
}

#endif //MYTEST_UTILS_HPP
