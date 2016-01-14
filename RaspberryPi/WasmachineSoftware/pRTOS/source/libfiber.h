//**************************************************************************
//
// This file contains its own documentation in Doxygen format.
// After running doxygen: open the index.html file.
//
//! Coroutine-based tasking service
//!
//! \authors
//!    - Marten Wensink (marten.wensink@hu.nl)
//!
//! \version
//!    V1.00 (2014-11-01)
//!
//! \if never
//!    The version is also available as the macro FIBER_VERSION,
//!    don't for get to updata that macro too!
//! \endif
//!
//! \par Introduction
//!
//! This library provides a coroutine-based tasking service using the
//! POSIX ucontext.
//***********************************************************************

#ifndef LIBFIBER_H
#define LIBFIBER_H

#include <ucontext.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <chrono>

//------------------------------------------------------------------------------
// error handler
//------------------------------------------------------------------------------
#define fiber_fatal(msg) \
    do { std::cout << "Fatal error: " << msg << std::endl; exit(EXIT_FAILURE); } while (0)

#define FIBER_VERSION       "V1.00 (2014-11-01)"

#define mainFiberRunning    (&fiber_t::main_fiber == fiber_t::running_fiber)


//***************************************************************************
//
// class fiber_t
//
//! Provides a coroutine-based tasking service using the POSIX ucontext.
//
//***************************************************************************

class fiber_t {
public:

   /**
    * Construct a fiber object
    *
    * @param func the function body for the fiber
    * @param sz the size of the stack
    */
   fiber_t ( void (*func)(void), int sz ) : dura(250) {
      if ( createContext ( func, sz ) == -1 )
         fiber_fatal( "fiber creation failure" );
   }

   /**
    * Destroy a fiber object
    */
   ~fiber_t ( void ) {
      free ( context.uc_stack.ss_sp );
   }

   /**
    * Resume this fiber
    */
   void resume ( void );

   /**
    * Return the base address off the stack
    *
    * @return the base address off the stack
    */
   inline void * stackBase ( void ) {
      return context.uc_stack.ss_sp;
   }

   /**
    * Return the size off the stack
    *
    * @return the size off the stack
    */
   inline int stackSize ( void ) {
      return context.uc_stack.ss_size;
   }

   /**
    * Return the # stack bytes used
    *
    * @return # bytes used
    */
   int stackUsed ( void );

   /**
    * The fiber object for the main thread.
    */
   static fiber_t   main_fiber;

   /**
    * Pointer to the object for the running fiber.
    */
   static fiber_t * running_fiber;

private:
   ucontext_t  context;             // Stores the fiber context
   std::chrono::microseconds dura;  // Time to sleep if idle

   // Default constructor
   fiber_t (void ) { }

   // Private context creation
   int createContext ( void (*func)(void), int sz );

   // Private context switching
   int fiberSwitchContext ( fiber_t * old, fiber_t * next );
};

extern unsigned long int memory_free (void);

#endif // LIBFIBER_H
