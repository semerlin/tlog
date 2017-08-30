#include "tsysdeps.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>

void t_exit(tint code)
{
    /* use exit( instead _exit() if you want to flush I/O buffer)*/
    _exit(code);
}

void t_print_backtrace (void)
{
#if defined(HAVE_BACKTRACE) && defined(T_BUILT_R_DYNAMIC)
    void *bt[64];
    tint bt_size = 0;
    tint i = 0;
    tchar **syms = NULL;

    bt_size = backtrace(bt, 500);
    syms = backtrace_symbols(bt, bt_size);

    while (i < bt_size)
    {
        fprintf (stderr, "%s\n", syms[i]);
        ++i;
    }
    fflush (stderr);

    free (syms);
#elif defined(HAVE_BACKTRACE) && !defined(T_BUILT_R_DYNAMIC)
    fprintf (stderr, "tlog not built with -rdynamic so unable to print a backtrace with function name\n");
#else
    fprintf (stderr, "tlog not compiled with backtrace support so unable to print a backtrace\n");
#endif
}


void t_abort (void)
{
    const char *s;
    t_print_backtrace();

    abort();
    /* in case someone manages to ignore SIGABRT ? */
    t_exit (1);
}


