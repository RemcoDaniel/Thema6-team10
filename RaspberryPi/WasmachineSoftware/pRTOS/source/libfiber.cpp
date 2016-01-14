/***************************************************************************\

file      libfiber.cpp
version   1.00
author    Marten Wensink
date      2014-11-01

\***************************************************************************/

#include "libfiber.h"

#include <stdlib.h>
#include <ucontext.h>
#include <errno.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <thread>

/*
 * Static class members
 */
fiber_t   fiber_t::main_fiber;
fiber_t * fiber_t::running_fiber = &main_fiber;

/*
 * Switch to 'this' fiber storing the context of the running fiber
 */
void fiber_t::resume( void ) {
    fiber_t * oldFiber = running_fiber;
    running_fiber = this;

    if (oldFiber == running_fiber)
       std::this_thread::sleep_for(dura);

    if ( fiberSwitchContext ( oldFiber, running_fiber ) == -1 )
        fiber_fatal ("could not switch to fiber");
}

/*
 * Create the fiber context
 */
int fiber_t::createContext ( void (*func)(void), int sz ) {
    void * stack_base = calloc (sz, sizeof (char));
    if (stack_base == NULL)
        return -1;

    // Create the new fiber context
    if ( getcontext( &context ) == -1 )
        return -1;

    // Set the context to a newly allocated stack
    context.uc_link = 0;
    context.uc_stack.ss_sp = stack_base;
    context.uc_stack.ss_size = sz;
    context.uc_stack.ss_flags = 0;

    // Create the context. The context calls func.
    makecontext( &context, func, 0 );

    return 0;
}

/*
 * Switches from the old to the next fiber
 */
int fiber_t::fiberSwitchContext ( fiber_t * old, fiber_t * next )
{
    if (old == NULL || next == NULL)
        return EINVAL;

    return swapcontext(&(old->context), &(next->context));
}

int fiber_t::stackUsed ( void ) {
    int sz = context.uc_stack.ss_size;
    char * sb = (char *) context.uc_stack.ss_sp;
    int i = 0;
    while (sb[i] == '\0' && i < sz)
        i++;
    return (sz - i);
}

unsigned long int memory_free (void) {
    struct sysinfo si;
    if (sysinfo(&si) < 0)
        fiber_fatal ("couldn't get system info");

    return si.freeram;
}
