//**************************************************************************
//
// This file contains its own documentation in Doxygen format.
// After running doxygen: open the index.html file.
//
//! Coroutine-based tasking service with synchronisation mechanisms
//!
//! \authors
//!    - Marten Wensink (marten.wensink@hu.nl)
//!    - Wouter van Ooijen (wouter@voti.nl)
//!
//! \version
//!    V4.03 (2014-12-12)
//!
//! \if never
//!    The version is also available as the macro RTOS_VERSION,
//!    don't for get to updata that macro too!
//! \endif
//!
//! \par Introduction
//!
//! This RTOS provides a simple coroutine-based tasking service with
//! a number of synchronisation mechanisms. The RTOS is designed to
//! run in een posix thread on an Raspberry Pi. It uses the ucontext
//! mechanism to provide the task switching.
//! Additionally the RTOS provides waitables, clocks, timers, semaphores,
//! flags, queues and mutexes.
//!
//! This RTOS (Real Time Operating Stystem) is used by the courses V2RTSP1
//! (Realtime System Programming) and V2TH06
//! (Themaopdracht 6 Technische Informatica).
//!
//! The RTOS consists of the files pRTOS.h and pRTOS.cpp and the file
//! libfiber.h and libfiber.cpp for the implementation of coroutines.
//! It is meant to be compiled by the GCC compiler to produce an executable
//! for a Raspberry Pi.
//!
//! Whenever this documentation states that something is or produces an
//! error this means that rtos_fatal will be called, which will
//! show the error on stdout and terminate the process.
//!
//! \par Files
//! - pRTOS.h
//! - pRTOS.cpp
//!
//
//***************************************************************************

#ifndef __RTOS
#define __RTOS

#define RTOS_VERSION "V4.03 (2014-12-12)"

// we need NULL
#include <cstring>
#include <iostream>
#include <iomanip>
#include <libfiber.h>

//***************************************************************************
//
//!
//! \page configuration Compile-time configuration
//!
//! The RTOS can be configured by changing some \#define's in pRTOS.h
//!
//! \par global_logging
//!
//! All RTOS debug logging can be switched off by defining
//! \b global_logging as 0
//! It is advised to make all application debug logging likewise
//! dependent on this macro. Check pRTOS.cpp for examples
//!
//! \par RTOS_STATISTICS_ENABLED
//!
//! By default \b RTOS_STATISTICS_ENABLED is defined as 1,
//! which enables printing statistics for all objects.
//! It can be defined as 0 to reduce code and data size.
//! NOTE: doing so will disable deadlock detection!
//!
//! When statistics is enabled, hitting Ctrl-C terminates the RTOS schedular
//! and shows the statistics.
//!
//! \par RTOS_DEFAULT_STACK_SIZE
//!
//! The default stack size is 4K.
//! You can choose another value by changing the initialization of
//! \b RTOS_DEFAULT_STACK_SIZE.
//!
//! \par RTOS_MIN_PRIORITY
//!
//! The priority you can assign to a task ranges from 0 to 1000, 0 being the
//! highest priority and 1000 the lowest.
//! Tasks should all have a different priority.
//
//***************************************************************************

#define global_logging 1

#define RTOS_STATISTICS_ENABLED 1

const unsigned int RTOS_DEFAULT_STACK_SIZE = 4096;

const unsigned int RTOS_MIN_PRIORITY = 1000;


//***************************************************************************
//
//! \page debug Debug logging support
//!
//! The RTOS uses std::cout output stream for logging support.
//!
//! The RTOS defines the trace macro, which can be used like cout,
//! but prefixes each output with the current source file name and
//! the current source line number. Hence (after the appropriate
//! preparations) the statement
//!
//! \code
//! trace << "n=" << n << "\n";
//! \endcode
//!
//! can create the output line
//!
//! \code
//! main.c:20 n=15
//! \endcode
//!
//! This provides an easy way to check if and when a certain line of code
//! is executed, and optionally print some debugging information.
//!
//! Note that using the logging mechanism influences the execition of
//! the task, maybe resulting in missing their deadlines.
//! The suggested initialization does not implement buffering, so using
//! cout or trace can change the timing of a
//! task that does printing considerably.
//!
//! All objects (RTOS, task, event, all waitables, mutex, pool, mailbox,
//! channel) can be printed to an ostream using the << operator.
//! Printing the RTOS will print all RTOS objects.
//
//***************************************************************************

// The macro HERE transates to a newline, the file-name, ":", and
// the line-number of the place where the HERE macro appears.
// This can be used for debug logging. used by the trace macro.
#define HERE_STR( X ) #X
#define HERE2( F, L ) ( "\n" F ":" HERE_STR( L ))
#define HERE HERE2( __FILE__, __LINE__ )

// Printing to trace (instead of cout) prepends HERE and a space,
// or trace can be used standalone like 'trace;'
#define trace ( std::cout << HERE << " " )

// allocate memory for the string and copy the content
const char * string_allocate( const char *name );

// the macro RTOS_STATISTICS is used to prefix a single line
// that will be commented out when statistics is disabled
#if RTOS_STATISTICS_ENABLED
   #define RTOS_STATISTICS( x ) x
#else
   #define RTOS_STATISTICS( x )
#endif

//***************************************************************************
//! error handler
//------------------------------------------------------------------------------
#define rtos_fatal(msg) \
    do { std::cout << "\n>> Fatal error: " << msg << std::endl; exit(EXIT_FAILURE); } while (0)

//***************************************************************************
//
//! \page units Unit macro's
//!
//! These macro's make it easier to specify (interval) times
//!
//! \code
//! #define  S * ( 1000 * 1000 )
//! #define MS * 1000
//! #define US * 1
//! \endcode
//!
//! Time in the RTOS is expressed in microseconds.
//! Using these marco's avoids knowledge of this detail.
//
//***************************************************************************

#define  S * ( 1000 * 1000 )
#define MS * 1000
#define US * 1

//***************************************************************************
//
//!
//! \page non-preemptive Non-preemptive task switching
//!
//! The RTOS uses non-preemptive task switching.
//! This means that the CPU can be switched to another task only
//! when the currenly executing task (directly or indirectly)
//! calls an RTOS function.
//! Three groups of RTOS function calls can cause such a task switch:
//!
//! -# functions that cause the current task to become non-runnable,
//!    like task::wait(), and task::suspend()
//! -# functions that make a higher-priority task runnable,
//!    like flag::set(), and task::resume()
//! -# the function task::release(), which only purpose is
//!    to give up the CPU to a higher priority task.
//!
//! A task can be made runnable either by an excplicit action
//! from another task, like an event_flag::set() or task::resume()
//! call, or implicitly by the expiration of a timer.
//! But even for the latter case (timer expiration) the switching
//! to another task can occur only when an RTOS function is called.
//!
//! The diagram below shows the state-event diagram for a task.
//! The transitions from ready to running and back are governed
//! by the RTOS always selecting the highest-priority runnable task.
//! The events that cause the transitions between runnable and blocked
//! and vice versa, and between blocked-and-suspended and suspended
//! are the same, they are shown in the enlarged box.
//! A task can only get blocked by doing something (wait,
//! read a mailbox, etc), hence there is no transition
//! from suspended to blocked-and-suspended.
//
//***************************************************************************

//***************************************************************************
//
//!
//! \page latency Latency
//!
//! When a task is activated by a timeout (either by a timer or clock,
//! or because it is a periodic task) at a certain moment in time it will
//! in general not be run at that time exactly, but at some later time.
//! This delay is called the latency. Two things contribute to the latency:
//!
//! -# Higher priority taks will be run first, until no higher
//!    priority tasks are runnable.
//! -# When a lower priority task is running when the timer fires
//!    the RTOS will notice this only when one of the RTOS functions
//!    is called that does a rescheduling: task::wait(), flag::set(),
//!    task::suspend(), task::resume(), task::release().
//!
//! The first contribution is a consequence of the design of the application.
//! When you feel that it is inappropriate that a particular higher-priority
//! task is run first and hence contributes to the latency of the task that
//! is activated by the timer, you have set the task priorities wrong.
//!
//! The second contribution is specific for a non-preemptive RTOS.
//! (A preemptive RTOS would immediately stop (preempt) the running
//! lower-priority task and switch to the higher-priority task.)
//! When you have lower priority tasks in your system that use a
//! lot of CPU time between the RTOS calls that do rescheduling
//! you can insert task::release() calls.
//! This call checks whether any timers that have timed out made a
//! higher-priority task runnable, and if so, switches to that task.
//!
//! When a task is made runnable by an explicit action of another task,
//! for instance a task:resume() call or a flag::set() call,
//! only the first source of delay (higher priority tasks that are
//! runnable) is applicable, because inside such calls the RTOS will
//! immediatley switch to the highest priority runnable task.
//
//***************************************************************************

//***************************************************************************
//
// class RTOS
//
//! static class, namespace-like container for RTOS declarations
//
//! The RTOS is a static class, instantiation is not needed.
//! After creating the tasks, call RTOS::run() to start the scheduling
//! of the tasks. RTOS::run() will never return.
//
//***************************************************************************
//
class RTOS {
public:
   class task;
   class event;
   class waitable;
   class flag;
   class timer;
   class clock;
   class mutex;
   template <class T> class mailbox;
   template <class T, const int SIZE> class channel;
   template <class T> class pool;
   class mailbox_base;
   class channel_base;
   class pool_base;

private:
   class waitable_set;
   class callback;

      //! the task that is currently executed
   static task * rtos_current_task;

      //! used by statistics clearance
   static bool must_clear;

      //! used by statistics clearance
   static void do_statistics_clear (void);

     //! this is a static class, prevent instantiation
   RTOS( void ) { }

      //! the list of callabacks, in no particular order
   static callback * timerList;

      //! add a callback
   static void add ( callback * t );

     //! the list of all tasks, highest priority first
   static task * taskList;

      //! add a task to the \ref taskList
   static void add( task * task );

      //! indicates whether the scheduler is running
   static bool scheduler_running;

     //! called repeatedly by run
   static void beat (void);

#if RTOS_STATISTICS_ENABLED
   static flag * flags;
   static timer * timers;
   static clock * clocks;
   static mutex * mutexes;
   static channel_base * channels;
   static mailbox_base * mailboxes;
   static pool_base * pools;

   static void add( flag * f );
   static void add( timer * t );
   static void add( clock * c );
   static void add( mutex * m );
   static void add( channel_base * cb );
   static void add( mailbox_base * mb );
   static void add( pool_base * pb );
#endif

public:
   //! runs the scheduler
   static void run( void );

   //! returns (a pointer to) the currently executing task
   static task * current_task( void ) { return rtos_current_task; }

   //! get elapsed time in micro seconds since OS startup
   static unsigned long long int run_time( void );

   //! prints statistics about the RTOS to the stream.
   static void print( std::ostream & stream );

   //! clears the statistics.
   //
   //! The actual clearing will be done later, inside \ref run(),
   //! when the current task has given up the processor.
   static void statistics_clear( void ) { must_clear = true; }

   //! print the statistics collect for the used RTOS objects
   static void display_statistics( void );

   //************************************************************************
   //
   // class event
   //
   //! set of things that can happen, or a thing that has happened
   //
   //! An event
   //! - is the result of adding waitables
   //! - is accepted as argument to wait()
   //! - is returned by wait()
   //! - can be compared to a waitable
   //!
   //! The task::wait() calls return an event.
   //! Such an event can be compared to a waitable.
   //! The result is true if and only if the waitable caused the event.
   //!
   //! Events are the only RTOS objects that can be destroyed
   //! (without causing an error).
   //
   //************************************************************************
public:
   class event {
   public:

      //! prints an event, for debugging only
      void print( std::ostream & s ) const;

      //! report wether two events are the same
      bool operator==( const event & rhs ) const;

      //! report whether an event corresponds to a waitable
      bool operator==( const waitable & rhs ) const;

      //! report wether two events are not the same
      bool operator!=( const event & rhs ) const;

      //! report whether an event does not correspond to a waitable
      bool operator!=( const waitable & rhs ) const;

      //! add two waitables, result can be used in a wait() call
      //
      //! Waitables can be added (operator+) to construct a
      //! 'set of waitables' as argument to a task::wait() call.
      event operator+( const event & rhs ) const;

   protected:

      //! the owner task
      task * t;

      //! the mask of this event, one bit is set
      //
      //! The bit that is set is unique among the events
      //! owned by a task.
      unsigned int mask;

      //! constructor, used by concrete events
      event( task * t, unsigned int mask ): t( t ), mask( mask ) { }

      friend class waitable_set;
   };

   //************************************************************************
   //
   // class waitable
   //
   //! abstract thing that a task can wait for
   //
   //! The operation clear() is provided (virtual, the default only clears
   //! the waitable) set() is provided but private (not all waitables can
   //! be set by the user).
   //!
   //! Waitable is an abstract class (there are no objects that are
   //! just a waitable).
   //! \ref flag, \ref timer, \ref clock and \ref channel are concrete
   //! classes that inherit from waitable.
   //! A waitable is always created for a particular task.
   //! A maximum of 31 waitables can be created for each task.
   //! (Actually the maximum is 32, but one waitable created
   //! internally to implement the sleep() call.)
   //! A waitable can be in two states: set or cleared.
   //! A waitable is initially cleared.
   //!
   //! A task can wait for one, a subset, or all waitables created for it.
   //! The default is to wait for all waitables created for the task,
   //! the other variants are specified by supplying to the task:wait()
   //! call either a single waitable, or the sum (operator+) of the
   //! waitables you want to wait for. When one of the waitables that
   //! is waited for becomes set the wait() call clears that waitable
   //! and returns an event that compares equal to the waitable.
   //! (Note that some waitables, for instance the channel,
   //! can immediately set itself again.) The calling task can compare
   //! that event to the waitables to see which event happened.
   //! When more than one of the waited-for waitables is set the wait()
   //! call makes an arbitrary choice from these waitables.
   //
   //************************************************************************
public:
   class waitable : public event {
   public:

      //! clear the waitable
      //
      //! This is automatically doen when the waitable
      //! causes a task::wait() call to return it.
      virtual void clear( void ) { t->waitables.clear( *this ); }

   protected:

      //! constructor, specify owner and name
      //
      //! The name is used for debugging only.
      waitable( task * task , const char * name );

      //! set the waitable
      void set ( void ) { t->waitables.set( *this ); }

      RTOS_STATISTICS( const char * waitable_name; )

      friend class waitable_set;
   };


   //************************************************************************
   //
   // class flag
   //
   //! basic synchronisation mechanism.
   //
   //! The basic synchronization mechanism is the (event) flag.
   //! Like all waitables, a flag is created for a particular task.
   //! A flag is set by a flag::set() call (or the task::set( flag)
   //! call, which has the same effect).
   //! Like all waitables, when a task is waiting for a flag
   //! (using a task::wait call) and that flag becomes set,
   //! the wait call will clear the flag, and return an event
   //! that compares equal to the flag.
   //! Note that a flag does not count: setting a flag that is
   //! already set has no effect on the flag.
   //!
   //! A flag must be created for a specific task.
   //! The normal place to do this is in the task's creator.
   //! An flag is initially cleared.
   //!
   //! The example below shows a led_task that responds to two event flags.
   //! The shift flag will cause it to shift the pattern on the LEDs one
   //! position to the left, while the invert flag will cause it
   //! to invert the pattern.
   //! Two addional tasks do notghing but set these flags at fixed intervals.
   //! The result is a sort of one-direction Kitt display,
   //! which will occasionally flip polarity.
   //! Note that in this example the wait call excplicitly
   //! mentions the flags it waits for.
   //
   //************************************************************************

public:
   class flag : public waitable {
   public:

      //! constructor, specify onwer and name
      //
      //! This call creates a timer for task t.
      //! The name is used for debugging and statistics.
      flag( task * t, const char * name = "" );

      //! sets the flag
      //
      //! Setting a flag causes the task that waits for this
      //! flag to be awakened.
      void set ( void );

      //! prints flag infomation (for debugging)
      void print( std::ostream & s, bool header = true ) const;

   private:

      // this information is needed for statistics only
      RTOS_STATISTICS( flag * next_flag; )
      RTOS_STATISTICS( unsigned int n_sets; )
      RTOS_STATISTICS( unsigned int n_gets; )

      friend class RTOS;
   };


   //************************************************************************
   //
   // class waitable_set
   //
   //! RTOS private implementation construct
   //
   //************************************************************************

private:
   class waitable_set {
   private:
      event wait( unsigned int mask );

   public:
      task * client;
      unsigned int current_waitables;
      unsigned int requested_waitables;
      unsigned int used;

      waitable_set( task * client ):
         client( client ),
         current_waitables( 0 ),
         requested_waitables( 0 ),
         used( 0 ) {}

      unsigned int waitable_allocate( void );
      void set( const waitable &w );
      void clear( const waitable &w );

      event wait( void ) { return wait( ~0 ); }
      event wait( const waitable & w ) { return wait( w.mask ); }
      event wait( const event & set ) { return wait( set.mask ); }
   };


   //************************************************************************
   //
   // class RTOS::task
   //
   //! an independent thread of execution
   //
   //! A task is an independent thread of execution, using its own stack.
   //! Tasks share the single CPU, so only one task can be running at any time.
   //! The RTOS determines which task is running.
   //! A task has two bi-value states that determine whether
   //! the task is runnable: the suspended/resumed flag and the
   //! waiting/non-waiting flag.
   //! A task is runnable only when it is both resumed, and non-waiting.
   //! When a task is created it is resumed and non-waiting.
   //! All tasks (and the RTOS code) run in the same memory space,
   //! without protection from each other.
   //! So a 'wild pointer' in one task can destroy data in
   //! another task, or even in the RTOS.
   //!
   //! Each task is created with a fixed priority, which can be any
   //! unsigned integer value below RTOS_MIN_PRIORITY (= 98).
   //! After creation the priority can not be changed.
   //! The value 0 indicates the highest task priority, a higher number
   //! indicates a lower priority.
   //! Each task must have a unqiue priority, it is an error to create
   //! a task with same priority as an existing task.
   //! You can omit the priority, in which case the RTOS will
   //! select an unused priority starting at RTOS_MIN_PRIORITY
   //! (in other words, it will choose a low priority for your task).
   //!
   //! Each task has its own stack.
   //! You can specify the size of the stack at task creation.
   //! If you omit the stack size, RTOS_DEFAULT_STACK_SIZE will be used
   //! (default: 4 Kb).
   //! This will be enough for most tasks, if you take care not to
   //! allocate big things on the stack, and avoid very deep nesting
   //! (watch out for recursion!).
   //!
   //! A task is created by instatiating a class that derives from
   //! RTOS::task and supplies a main().
   //! This main() should never return.
   //! The fragment below shows how you can do this.
   //! The task name is used for statistics and debugging.
   //! As shown for the name, it might be wise to get the task parameters
   //! as arguments to the constructor of your task.
   //!
   //! \code
   //! class my_task_class : public RTOS::task {
   //! public:
   //!    my_task_class( const char * name ):
   //!       task(
   //!          name,  // name of the task
   //!          10,    // task priority
   //!          16384  // task stack size
   //!       ){}
   //! private:
   //!    void main( void ){
   //!       // put the code of your task here
   //!    }
   //! };
   //! my_task_class my_task( "my first task" );
   //! my_task_class my_task( "my second task" );
   //! \endcode
   //!
   //! The example below is a complete program that shows the
   //! standard part (initialization, and a main that calls
   //! RTOS::run()), a function for writing to an individual LED,
   //! a task class that blinks a LED, and two instatiations of
   //! this class.
   //! Note that the sleep() call is used instead of mkt_wait_ms
   //! or mkt_wait_us. Sleep() causes other tasks to run while
   //! this task is waiting, whereas the mkt_wait_* calls would
   //! use monopolize the CPU to do a busy wait.
   //!
   //! Subsequent examples will not show the standard
   //! initialization (the part up to the comment line).
   //!
   //! \code
   //!
   //! #include "pRTOS.h"
   //!
   //! int main( void ) {
   //!    RTOS::run();
   //!    return 0;
   //! }
   //!
   //! // end of standard part
   //!
   //! class blinker : public RTOS::task {
   //! public:
   //!    blinker( int LED, int period ):
   //!       LED( LED ), period( period ){}
   //! private:
   //!    int LED, period;
   //!    void main( void ){
   //!       for( ; ; ) {
   //!          led_set( LED, 1 );
   //!          sleep( period / 2 );
   //!          led_set( LED, 0 );
   //!          sleep( period / 2 );
   //!       }
   //!    }
   //! };
   //!
   //! blinker led0( 0, 1000 MS );
   //! blinker led1( 1,  300 MS );
   //! \endcode
   //!
   //! A task can be suspended and resumed by the task::suspend()
   //! and task::resume() calls.
   //! The suspend/resume state does not count the number of suspends
   //! and resumes: a suspend call on an already suspended task
   //! (or a resume on an already resumed task) has no effect.
   //! Suspend and resume are intended for use by the application code:
   //! the RTOS will never suspend or resume a task.
   //! (The RTOS uses the waiting/non-waiting state, which can
   //! not be changed directly by the application.)
   //!
   //! The example below shows one task that beeps the speaker at 1 kHz,
   //! while the other taks suspends and resumes the first task to make
   //! it beep 5 times, after which it suspends itself, which ends all
   //! activity.
   //! (This will trigger the RTOS deadlock detection, because a normal
   //! embedded application should never terminate.)
   //!
   //! \code
   //! class beeper : public RTOS::task {
   //! public:
   //!    unsigned int speaker;
   //!    beeper( unsigned int speaker ): speaker( speaker ){}
   //!    void main( void ){
   //!       mkt_pin_configure( speaker, mkt_output);
   //!       for( ; ; ) {
   //!          mkt_pin_write( speaker, 1  );
   //!          sleep( 500 US );
   //!          mkt_pin_write( speaker, 0  );
   //!          sleep( 500 US );
   //!       }
   //!    }
   //! };
   //! beeper speaker( 10 );
   //!
   //! class suspender : public RTOS::task {
   //!    void main( void ){
   //!       for( int i = 0; i < 5 ; i++ ) {
   //!          speaker.resume();
   //!          sleep( 500 MS );
   //!          speaker.suspend();
   //!          sleep( 1 S );
   //!       }
   //!       suspend();
   //!    }
   //! };
   //! suspender task2;
   //! \endcode
   //
   // A task is created by inheriting
   // from task and providing a main() function. Initialisation of the task,
   // including creating its waitables, should be done in the constructor.
   // Don't forget to call the constructor of the task class!
   //
   // The main() is the body of the task. It should never terminate.
   //
   // Each task has a unique priority (an unsigned integer).
   // A lower value indicates a higher priority.
   // The RTOS scheduler will always run the task with the higest-priority
   // runnable (neither blocked nor suspended) task.
   // A task runs until it changes this 'situation' by using an RTOS
   // call that changes its own state to not runnable, or the state of a
   // higher priority task to runnable.
   //
   // Timers are served only when the RTOS is activated
   // by calling any of its state-changing interfaces.
   // Hence the longest run time between such calls determines the granularity
   // (time wise responsiveness) of the application.
   // Within a time consuming computation a task can call release() to have
   // the RTOS serve the timers.
   //
   //************************************************************************

public:
   class task {

   private:

      // The RTOS uses block() and unblock() to implement the
      // synchronisation mechanisms (waitables).
      bool task_is_blocked;

      // A task can be suspended and resumed by the application,
      // using the suspend() and resume() calls.
      // The RTOS itself never suspends or resumes a task.
      bool task_is_suspended;

      // task name, only for statistics
      RTOS_STATISTICS( const char * task_name; )

      // a lower number means a higher priority, 0 is highest
      unsigned int task_priority;

      // timer for sleep() call
      timer * sleep_timer;

   protected:

      //! task body, must be provided by a derived class
      //
      //! A task is created by inheriting
      //! from task and providing a main() function.
      //! Initialisation of the task, including creating its waitables,
      //! should be done in the constructor.
      //! Don't forget to call the constructor of the task class!
      //!
      //! The main() is the body of the task. It should never terminate.
      //!
      //! Each task has a unique priority (an unsigned integer).
      //! A lower value indicates a higher priority.
      //! The RTOS scheduler will always run the task with the
      //! higest-priority runnable (neither blocked nor suspended) task.
      //! A task runs until it changes this 'situation' by using an RTOS
      //! call that changes its own state to not runnable, or the state of a
      //! higher priority task to runnable.
      //!
      //! Timers are served only when the RTOS is activated
      //! by calling any of its state-changing interfaces.
      //! Hence the longest run time between such calls determines the
      //! granularity (time wise responsiveness) of the application.
      //! Within a time consuming computation a task can call release()
      //! to have the RTOS serve the timers.
      //!
   virtual void main( void ) = 0;

   private:

      // must call main()
      friend void task_trampoline( void );

   public:

      //! constructor, specify priority, name and stack size
      //
      //! Priorities are reasonably-valued (below RTOS_MIN_PRIORITY)
      //! unsigned integers. 0 is te highest priority.
      //! Priorities must be unqiue.
      //! The default causes the constructor to choose a free priority
      //! starting at RTOS_MIN_PRIORITY (default: 1000).
      //!
      //! The name is used for debugging and statistics.
      //!
      //! A stack of stack_size bytes is allocated for the task.
      //! The default is 4 kB.
      task(
         unsigned int priority  = RTOS_MIN_PRIORITY,
         const char * tname     = "",
         unsigned int stacksize = RTOS_DEFAULT_STACK_SIZE
      );

      //! throws an error, beacuse tasks should never be destroyed
      ~task ( void ) {
         rtos_fatal ("task destructor called");
      }

      //! suspend a task (prevent execution until a resume)
      //
      //! Suspends the task (prevents execution).
      //! Has no effect when the task is already suspended.
      //! Can be extended by an application task.
      //!
      //! A concrete task can extend this operation to suit its needs.
      virtual void suspend( void );

      //! continue a suspended task
      //
      // Resumes the task (cancels a suspension).
      //! Has no effect when the task is not suspended.
      //!
      //! Can be extended by an application task to suit its needs.
      virtual void resume( void );

      //! release the CPU to the scheduler
      //
      //! Sevices timers and releases the CPU to a higher
      //! priority task if is ready.
      void release( void );

      //! wait for some time
      //
      //! Sleeps the task (prevents execution) for the indicated time.
      void sleep( unsigned int time );

      //! report the task priority
      unsigned int priority( void ) const   { return task_priority; }

      //! report the task name
      const char * name( void ) const;

      //! report whether the task is currently suspended
      bool is_suspended( void ) const { return task_is_suspended; }

      //! report whether the task is currently blocked
      bool is_blocked( void ) const   { return task_is_blocked; }

      //! report whether the task is currently ready for execution
      bool is_ready( void ) const {
         return !( task_is_suspended || task_is_blocked );
      }

      //! print task statistics
      void print( std::ostream & stream, bool header = true ) const;

      //! wait for all waitables created for this task
      //
      //! Wait (prevent execution) until at least one of the waitables
      //! is set. Return and clear that waitable.
      //! Three variants for the parameter:
      //!
      //! - The default (no parameter) waits for all waitables
      //!   defined for the task.
      //! - One waitable as argument waits for that specific waitable.
      //! - The addition (operator+) of waitables: wait for any one of
      //!   those waitables.
      //!
      //! It is an error to wait for waitables that have not been created
      //! for this task.
      event wait( void ) { return waitables.wait(); }

      //! wait for a single waitable
      //
      //! @copydetails wait
      event wait( const waitable & w ) { return waitables.wait( w ); }

      //! wait for a set of waitables
      //
      //! @copydetails wait
      event wait( const event & set ) { return waitables.wait( set ); }

      //! set a flag
      void set( flag &f ) { f.set(); }

      //! ignore this activation for the statistics
      //
      //! Calling this function makes the RTOS statistics ignore
      //! the current task activation as far as statistics is concerned.
      //! You can use this to avoid pullution of your task statistics
      //! with the timing effects of debug logging. But make sure you
      //! don't use it in the 'normal' execution paths, because that would
      //! make the statistics lie to you.
      void ignore_activation_time( void ) {
         ignore_this_activation = true;
      }

   private:

      //! clear statistics of this task
      void statistics_clear( void ) {
         runtime_max = 0;
         activations = 0;
      }

      //! waitables on which the task can wait
      waitable_set waitables;

      //! for blocking a task by means of a synchronization mechanism
      void block( void );

      //! for unblocking a task by means of a synchronization mechanism
      void unblock( void );

      //! statistics
      int activated;
      int latency_max;
      unsigned long int runtime_max;
      int activations;
      bool ignore_this_activation;

      //! for debugging
      void debug( const char * msg );

      //! for the RTOS to link all tasks
      task * nextTask;

      //! for the mutexes to link waiting tasks
      task * next_mutex_waiter;

      //! the fiber that holds the task body and stack
      fiber_t * fiber;

      friend class periodic_task;
      friend class waitable_set;
      friend class flag;
      friend class RTOS;
   };


   //************************************************************************
   //
   // class callback
   //
   //! RTOS private implementation class
   //
   //************************************************************************

private:
   class callback {
   public:

      callback( const char * name = "" );

      // a timer should never be destroyed
      ~callback( void ) {
         rtos_fatal ("callback_timer destructor called");
      }

      // a timer's time_up function must be provided by a derived class
      virtual void time_up (void) = 0;

   protected:
      // the callback must fire 'time' from now
      void start( unsigned long int time ) { time_to_wait = time; }

      // the callback must fire 'time' from the last firing
      void restart( unsigned long int time ) {
         while( time_to_wait <= 0 ) { time_to_wait += time; }
      }

      // abort a started timer
      virtual void cancel( void ) { time_to_wait = 0; }

      RTOS_STATISTICS( const char * object_name; )

      // the number of microseconds until the next time_up
      long long int time_to_wait;

      // called by the RTOS to state that <elapsed> time has elapsed
      void tick( int elapsed );

      // for the RTOS to link timers in a chain
      callback * nextTimer;

      friend class RTOS;
   };


   //************************************************************************
   //
   // class timer
   //
   //! one-short timer
   //
   //! A (one-shot) timer is a special type of flag, which can be
   //! instructed to set itself after a fixed amount of time.
   //! The amount of time is supplied with the timer::set() call.
   //! This call starts the timer.
   //! A timer that is running (waiting for its timeout to expire)
   //! can be canceled by the timer::cancel() call.
   //! When a timer that is already running is set again the previous
   //! timeout is overwritten by the new one.
   //! The suspend/resume state of its owner taks has no effect on
   //! a timer: even when the task is suspended the timer will run
   //! to its timeout and set isetlf.
   //! But of course the task, being suspended, will not be able to react.
   //!
   //************************************************************************

public:
   class timer : public waitable, public callback {
   public:

      //! create a timer for task t, specify its name
      timer( task * t, const char * name = "" );

      //! Start the timer: it will set itself after the indicated timeout,
      //! starting from now.
      //! When the timer was already running the previous timout
      //! is overwritten.
      void set( unsigned long int time );

      //! stop and clear the timer
      //
      //! Stop the timer (when it was running),
      //! and clears its (when it was set).
      void cancel( void );

      //! print the timer (for debugging)
      void print( std::ostream & s, bool header = true ) const;

   private:
      void start( unsigned long int time );

      void time_up( void ) { waitable::set(); }

      RTOS_STATISTICS( timer * next_timer; )
      RTOS_STATISTICS( unsigned int n_sets; )
      RTOS_STATISTICS( unsigned int n_cancels; )

      friend class RTOS;
   };

   //************************************************************************
   //
   // class clock
   //
   //! free-running clock, ticks at a fixed frequency
   //
   //! A clock is a waitable which is automatically sets itself
   //! at fixed intervals.
   //! The interval between these moments is specified when the
   //! clock is created.
   //! A clock is always running, even when the task to which it
   //! belongs is suspended.
   //!
   //************************************************************************

public:
   class clock :  public waitable, public callback {
   public:

      //! create a clock for task t, specify interval and name
      //
      //! The name is used for debugging and statistics.
      clock(
         task * t,
         unsigned long int _period,
         const char * name = ""
      );

      //! clear the waitable within the clock
      //
      //! Note that this does not stop the clock.
      void clear( void ) { waitable::clear(); }

      //! the interval of the clock
      unsigned long int interval( void ) { return period; }

      //! print the clock (for debugging)
      void print( std::ostream & s, bool header = true ) const;

   private:
      void time_up( void );
      unsigned long int period;

      RTOS_STATISTICS( clock * next_clock; )
      RTOS_STATISTICS( unsigned int ticks; )

      friend class RTOS;
   };


   //************************************************************************
   //
   // class pool_base
   //
   //! RTOS private implementation class
   //
   //************************************************************************

public:
   class pool_base {
   public:
      pool_base( const char * name );
      ~pool_base( void ) {
         rtos_fatal ("pool destructor called");
      }
      void print( std::ostream & s, bool header = true ) const;

   #if RTOS_STATISTICS_ENABLED
      unsigned int reads;
      unsigned int writes;
      pool_base * next_pool;
      const char * pool_name;
   #endif

      friend class RTOS;
   };

   //************************************************************************
   //
   // class pool
   //
   //! place to store and rectrieve data, no built-in synchronisation
   //
   //! A (communication) pool is a template class that stores a single value.
   //! It supports the read and write operations, which are guaranteed to be
   //! atomic. (On a non-preemptive RTOS every assignment is atomic,
   //! but the pool template is still usefull to make it explicit that
   //! data is transferred between tasks.)
   //! A pool is just a variable.
   //!
   //! The example below demonstrates the use of a pool to
   //! maintain a seconds-since-startup counter.
   //! Note that the call RTOS::runtime() returns the time elapsed since
   //! startup, so there is no need to maintain a
   //! seconds-since-startup this way yourself.
   //!
   //! \code
   //!
   //! pool< unsigned int > seconds;
   //!
   //! void show_time( void ){
   //!    unsigned int n = seconds.read();
   //!    std::cout << ( seconds / 60 ) % 60 << ":" << seconds % 60;
   //! }
   //!
   //! class seconds_counter_class : public periodic_task {
   //!    seconds_counter( void ){
   //!       periodic_task::periodic_task( "sec-counter", 10, 1000 MS );
   //!       seconds.write( 0 );
   //!    }
   //!    void main( void ){
   //!       for( ; ; ) {
   //!          (void) wait(); // only one thing to wait for
   //!          seconds.write( seconds.read() + 1 );
   //!       }
   //!    }
   //! }
   //!
   //! seconds_counter_class seconds_counter;
   //!
   //! \endcode
   //
   //************************************************************************

public:
   template <class T> class pool : public pool_base {
   public:

      //! construct a pool, specify its name (for debgging only)
      //
      //! Use it to make (global) variables use for communication
      //! between tasks explicit.
      //!
      //! The template argument T must be a class that has a
      //! non-arguments constructor and supports assignment.
      pool( const char * name = "" ): pool_base( name ) { }

      //! atomic write operation on a pool
      //
      //! A read opeartion returns the most recently written data.
      //!
      //! In the context of co-operative multitasking a read of write
      //! operation on anything is always atomic, unless the implementation
      //! of that operating somehow invokes the RTOS.
      //! But for clearness it is a good idea to implement such task-global
      //! data as pools.
      void write (T item) {
         RTOS_STATISTICS( writes++; )
         data = item;
      }

      //! atomic read operation on a pool
      //
      //! @copydetails  write
      T read (void) {
         RTOS_STATISTICS( reads++; )
         return data;
      }

   private:
      T data;
   };


   //************************************************************************
   //
   // class mutex
   //
   //! mutual execlusion semaphore
   //
   //! A mutex (mutual exclusion semaphore) is a synchronization mechanism
   //! that is used to give a task exclusive access to some resource:
   //! the task can execute a sequence of statements, being sure that
   //! no other task is accessing the same resource.
   //!
   //! A typical use is to protect a resource (for instance global data)
   //! that should be used by only one task at a time, so it can update
   //! it and leave it in a consistent state.
   //!
   //! A mutex is not created for a particular task, and it is not a waitable.
   //!
   //! Initially a mutex is free.
   //! The mutex::wait() operation blocks the task until the mutex is free,
   //! and then claims the mutex for the executing task.
   //! The mutex::signal() operation frees the mutex again.
   //! It is an error to call mutex::signal on a mutex that is not
   //! currently owned by the executing task.
   //!
   //************************************************************************

public:
   class mutex {
   public:

      //! constructor, specify the name
      //
      //! The name is used for debugging only.
      mutex( const char * name = "" );

      //! generates an error
      //
      //! A mutex should never be destroyed
      ~mutex( void );

      //! prints a mutex, for debugging only.
      void print( std::ostream & stream, bool header = true ) const;

      //! claim the mutex
      //!
      //! If the mutex was set it it is now cleared,
      //! and the calling task owns the mutex.
      //!
      //! Otherwise the current task waits (is halted) until the
      //! owning task calls signal() on the same mutex.
      //! The signal() calls will release the tasks in
      //! the order of their wait() calls.
      void wait   (void);

      //! release the mutex
      //
      //! If one or more tasks are waiting for the mutex the fires
      //! one is released, and it now owns the mutex.
      //! Otherwise, if the mutex is cleared it is now set.
      //!
      //! It is an error for a task to call signal() on a
      //! mutex that it does not own (that it did not call wait() on).
      //! After the signal the task no longer owns the mutex.
      void signal (void);

   private:
      task * owner;             // current owner of the mutex
      task * waiters;           // head of the waiting tasks queue

   #if RTOS_STATISTICS_ENABLED
      const char * mutex_name;  // for logging
      mutex * next_mutex;       // queue of all mutexes, for logging
      int wait_count;           // counts # wait calls;
   #endif

      friend class RTOS;
   };


   //************************************************************************
   //
   // class mailbox_base
   //
   //! RTOS private implementation class
   //
   //************************************************************************

public:
   class mailbox_base {
   public:
      mailbox_base( const char * name );

      ~mailbox_base( void ) {
         rtos_fatal ("mailbox destructor called");
      }

      void print( std::ostream & s, bool header = true ) const;

      task * writer;
      task * reader;

   #if RTOS_STATISTICS_ENABLED
      const char * mailbox_name;
      unsigned int writes;
      unsigned int reads;
      mailbox_base * next_mailbox;
   #endif

   };

   //************************************************************************
   //
   //
   // class mailbox
   //
   //! Synchronously handing over of a data item
   //
   //! A mailbox is a template class synchronization mechanism.
   //! A single value can be written to the mailbox.
   //! Another task can read the value from the mailbox.
   //! The read and write calls wait on each other before they
   //! are allowed to proceed.
   //!
   //! A mailbox is not created for a particular task,
   //! and it is not a waitable.
   //!
   //! Initially a mailbox is empty.
   //! The \ref write() operation writes to the mailbox, and, if present,
   //! unblocks a reading task and returns, otherwise the writing task is
   //! blocked.
   //! The \ref read() operation blocks the calling task until
   //! there is a value in the mailbox.
   //! Then it reads the value, unblocks the task that wrote to
   //! the mailbox, and returns.
   //
   //************************************************************************

public:
   template <class T> class mailbox : mailbox_base {
   public:

      //! constructor, specify mailbox name
      //
      //! Create a mailbox. The mailbox is initially empty.
      // The name is used for debugging and statistics.
      //!
      //! The template argument T must be a class that has a
      //! non-arguments constructor and supports assignment.
      mailbox( const char * name = ""  ): mailbox_base( name ) { }

      //! write an item into the mailbox
      //!
      //! The current (writing) task stores an item in the mailbox.
      //! If a client (reader) is waiting, it is unblocked.
      //! Otherwise the task waits (is blocked) until a reading task
      //! has read the item.
      void write( const T item ) {
         RTOS_STATISTICS( writes++; )
         data = item;
         if ( reader != NULL ) {
            task * tmp = reader;
            reader = NULL;
            // someone is waiting to read, unblock it
            tmp->unblock();
         }
         else {
            if (writer != NULL)
                rtos_fatal ("second writer for mailbox");

            // block until the reader gets the data
            writer = RTOS::current_task();
            writer->block();
         }
      }

      //! read a value from the mailbox
      //
      //! If a writing tasks is waiting for the mailbox
      //! it is unblocked and the reader gets the data.
      //! Otherwise the current task is blocked until it
      //! is released by a writer.
      T read (void) {
         if ( writer != NULL ) {
            // unblock the writer
            task * tmp = writer;
            writer = NULL;
            tmp->unblock();
         }
         else {
            if (reader != NULL)
               rtos_fatal ("second reader for mailbox");

            // no writer yet, so wait for a writer
            reader = RTOS::current_task();
            reader->block();
         }
         RTOS_STATISTICS( reads++; )
         return data;
      }

   private:
      T data;
   };

   //************************************************************************
   //
   // class channel_base
   //
   //! RTOS private implementation class
   //
   //************************************************************************

public:
   class channel_base : public waitable {
   public:
      ~channel_base( void ) {
         rtos_fatal ("channel destructor called");
      }

      void print( std::ostream & s, bool header = true ) const;

   protected:
      channel_base( task * t, const char * name );

   #if RTOS_STATISTICS_ENABLED
      const char * channel_name;
      channel_base * next_channel;
      int writes;
      int ignores;
   #endif

      int qSize;
      int head;
      int tail;

      friend class RTOS;
   };

   //************************************************************************
   //
   // class channel
   //
   //! waitable data queue
   //
   //! The (communication) channel is a template class that stores a queue
   //! of values.
   //! Values can be written at the tail of the queue, up to the number of
   //! entries for which the channel was created.
   //! It is an error to write to a channel that is full.
   //! Writes are not blocking.
   //! Any task can write to a channel.
   //!
   //! A channel is created for a particular task.
   //! Only this owner task can read from the channel.
   //! A read will block until an entry is available.
   //! Reads are from the head of the queue.
   //!
   //! A channel is a waitable, so the task that owns the channel
   //! can wait for the channel to be non-empty, after which a read
   //! from a channel will be non-blocking (because the channel is
   //! not empty).
   //! After a wait() that returns the channel's event, the channel
   //! will set itself again (because the wait did not cause it to
   //! become empty).
   //! Only a read that results in an empty queue will clear the channel.
   //!
   //! The example below shows how writing to cout can be buffered
   //! by first writing to a 2kB channel, and reading from that
   //! channel at a maximum of one character per 2 MS.
   //! The UART hardware in the LPC2148 chip buffers one character,
   //! which at default baudrate (38k4) takes ~ 1 MS to write.
   //! So by writing at a maximum rate of one character per 2 MS
   //! no blocking will occur.
   //!
   //! \code
   //!
   //! class output_class : public task {
   //! public:
   //!    channel< char, 2048 > buffer( this, "buffer" );
   //!    timer hartbeat( this, "hartbeat" );
   //!    void main( void ){
   //!       for( ; ; ) {
   //!          wait( buffer );
   //!          cout << buffer.get();
   //!          timer.set( 2 MS );
   //!          wait( timer );
   //!      }
   //!    }
   //! }
   //!
   //! output_class output;
   //!
   //! void print( char * s ){
   //!    while( *s != '\0' ) { output.buffer.write( *s++ ); }
   //! }
   //!
   //! \endcode
   //
   //************************************************************************

public:
   template <class T, const int SIZE> class channel : public channel_base {
   public:

      //! constructor, specify stored type, number of entries, and name
      //
      //! The template argument T must be a class that has a
      //! non-arguments constructor and supports assignment.
      channel( task * t, const char * name = "" ): channel_base( t, name ) { }

      //! write an item to the queue
      void write( T item ) {
         if( qSize < SIZE ) {
            RTOS_STATISTICS( writes++; )
            queue[head] = item;
            if( ++head == SIZE ) {
               head = 0;
            }
            qSize += 1;
            waitable::set();
         }
         else {
            RTOS_STATISTICS( ignores++; )
         }
      }

      //! read an item from the queue
      T read( void ) {
         if( qSize == 0 ) {
            t->wait( *this );
         }

         if( qSize == 0 ) {
            rtos_fatal ("channel should not be empty now");
         }

         T buf = queue[ tail ];
         if( ++tail == SIZE ) {
            tail = 0;
         }
         qSize -= 1;
         if( qSize > 0 ) {
            waitable::set();
         }
         else {
            waitable::clear();
         }
         return buf;
      }

      void clear (void) {
         qSize = 0;
         head = tail = 0;
         waitable::clear();
      }
   private:
      T queue[ SIZE] ;
   };

};

std::ostream & operator<< ( std::ostream & s, const RTOS::task & t );
std::ostream & operator<< ( std::ostream & s, const RTOS::flag & f );
std::ostream & operator<< ( std::ostream & s, const RTOS::event & e );
std::ostream & operator<< ( std::ostream & s, const RTOS::timer & t );
std::ostream & operator<< ( std::ostream & s, const RTOS::clock & c );
std::ostream & operator<< ( std::ostream & s, const RTOS::channel_base & cb );
std::ostream & operator<< ( std::ostream & s, const RTOS::mutex & m );
std::ostream & operator<< ( std::ostream & s, const RTOS::mailbox_base & mb );
std::ostream & operator<< ( std::ostream & s, const RTOS::pool_base & pb );

#endif
