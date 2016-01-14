/***************************************************************************\

file      RTOS.cpp
version   4.03
author    Marten Wensink / Wouter van Ooijen
date      2014-12-12

\***************************************************************************/

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include "pRTOS.h"
#include "libfiber.h"

using namespace std;

#define task_logging          (global_logging && 0)
#define debug_task_logging    (global_logging && 0)
#define hartbeat_logging      (global_logging && 0)

#define TASK_STATE( task ) \
   ( (task)->is_blocked()   ? "B" : "-" ) << \
   ( (task)->is_suspended() ? "S" : "-" ) << \
   ( (task)->is_ready()     ? "R" : "-" )

#define task_trace \
   if (debug_task_logging) \
      trace \
         <<  name() \
         << " " << TASK_STATE( this ) \
         << " "

#define TASK_NAME( t ) (((t) == NULL)? "-" : (t)->name())

/*
 * Some utility functions
 */
const char * string_allocate( const char * str ) {
   int len = strlen( str );
   if( len > 16 ) len = 16;
   char * result = (char *) malloc( 1 + len);
   strncpy( result, str, len );
   result[len] = '\0';
   return result;
}

int nr_from_mask( unsigned int mask ) {
   for( int i = 0; i < 32; i++ ) {
      if( mask & ( 1 << i )) {
         return i;
      }
   }
   return -1;
}

void task_trampoline( void ) {
   RTOS::current_task()->main();
   cout
      << "\n>> Fatal error: task "
      << RTOS::current_task()->name()
      << " returned from main()\n";
   exit (EXIT_FAILURE);
}

/***************************************************************************\

task

\***************************************************************************/

RTOS::task::task( unsigned int priority, const char * tname, unsigned int stacksize )
:  task_is_blocked( false ),
   task_is_suspended( false ),
   task_priority( priority ),
   waitables( this ) {

   RTOS_STATISTICS( task_name = string_allocate( tname ); )

   sleep_timer = NULL;
   fiber = new fiber_t( task_trampoline, stacksize );
#if ( debug_task_logging == 1 )
   trace
      << name()
      << " SB="
      << hex << fiber->stackBase()
      << "  SZ="
      << hex << fiber->stackSize()
      << dec << "\n";
#endif
   ignore_this_activation = true;
   statistics_clear();
   RTOS::add( this );
   task_trace << "CREATED";
}

void RTOS::task::suspend (void) {
   task_trace << "suspend";
   task_is_suspended = true;
   release();
}

void RTOS::task::resume (void) {
   task_trace << "resume";
   task_is_suspended = false;
   release();
}

void RTOS::task::unblock (void) {
   if( ! RTOS::scheduler_running ) {
      return;
   }
   task_trace << "unblock";
   task_is_blocked = false;

   fiber_t::main_fiber.resume();
}

void RTOS::task::block (void) {
   if( ! RTOS::scheduler_running ) {
      return;
   }
   task_trace << "block";

   // Only a running task can block itself
   if (RTOS::current_task() != this) {
      rtos_fatal ("task not blocked by itself");
   }

   if( !is_ready() ) {
      rtos_fatal ("running task is not READY!?");
   }

   task_is_blocked = true;
   fiber_t::main_fiber.resume();
}

void RTOS::task::release( void ) {
   if( ! RTOS::scheduler_running ) {
      return;
   }

   if( mainFiberRunning ) {
      // the RTOS scheduler should not call release, only tasks should
      rtos_fatal ("scheduler calls release!?");
   }

   fiber_t::main_fiber.resume();
}

void RTOS::task::sleep( unsigned int time ) {
   if( sleep_timer == NULL ) {
      sleep_timer = new timer( this, "sleep timer" );
   }
   sleep_timer->start( time );
   wait( *sleep_timer );
}

void RTOS::task :: print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream
         << endl << dec
         << setw( 16 ) << left  << "task name"
         << setw(  6 ) << right << "prio"
         << setw(  5 ) << right << "stat"
         << setw( 11 ) << right << "stack u/m"
         << setw( 10 ) << right << "rt_max"
         << setw( 10 ) << right << "active"
         << endl;
   }
   stream
      << dec
      << setw( 16 ) << left  << task_name
      << setw(  6 ) << right << dec << task_priority
      << setw(  3 ) << right << TASK_STATE( this )
      << setw(  6 ) << right << fiber->stackUsed()
      << '/'
      << setw(  5 ) << left << fiber->stackSize()
      << setw(  9 ) << right << runtime_max
      << setw( 10 ) << right << activations
      << endl;
#endif
}

//***************************************************************************
//
// event
//
//***************************************************************************

bool RTOS::event :: operator==( const event & rhs ) const {
   if( t != rhs.t ) {
      rtos_fatal ("comparing incompatible waitables");
   }
   return mask == rhs.mask;
}

bool RTOS::event :: operator!=( const event & rhs ) const {
   if( t != rhs.t ) {
      rtos_fatal ("comparing incompatible waitables");
   }
   return mask != rhs.mask;
}

bool RTOS::event :: operator==( const waitable & rhs ) const {
   return *this == (event)rhs;
}

bool RTOS::event :: operator!=( const waitable & rhs ) const {
   return *this != (event)rhs;
}

RTOS::event RTOS::event :: operator+( const event & rhs ) const {
   if( t != rhs.t ) {
      rtos_fatal ("adding incompatible waitables");
   }
   return event( t, mask | rhs.mask );
}

void RTOS::event::print( std::ostream & s ) const {
   s << "event t=" << t->name() << " m=0x" << std::hex << mask;
}


//***************************************************************************
//
// waitable
//
//***************************************************************************

RTOS::waitable :: waitable( task * t, const char * arg_name ) :
   event( t, 0 )
{
   RTOS_STATISTICS( waitable_name = string_allocate( arg_name ); )
   mask = t->waitables.waitable_allocate();
}

//***************************************************************************
//
// flag
//
//***************************************************************************

RTOS::flag::flag( task * t, const char * name ):
   waitable( t, name )
#if RTOS_STATISTICS_ENABLED
   , n_sets( 0 )
   , n_gets( 0 )
#endif
{
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::flag::set( void ) {
   RTOS_STATISTICS( n_sets++; )
   waitable::set();
}

void RTOS::flag::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left  << "flag name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  2 ) << right << "fn";
      stream << setw( 10 ) << right << "sets";
      stream << setw( 10 ) << right << "gets";
      stream << "\n";
   }
   stream << setw( 18 ) << left  << waitable_name;
   stream << setw( 18 ) << left  << TASK_NAME( t );
   stream << setw(  2 ) << right << dec << nr_from_mask( mask );
   stream << setw( 10 ) << right << dec << n_sets;
   stream << setw( 10 ) << right << dec << n_gets;
   stream << "\n";
#endif
}

//***************************************************************************
//
// timer
//
//***************************************************************************

RTOS::timer::timer( task * t, const char * name ):
   waitable( t, name ),
   callback( name )
#if RTOS_STATISTICS_ENABLED
   , n_sets( 0 )
   , n_cancels( 0 )
#endif
{
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::timer::set( unsigned long int time ) {
   RTOS_STATISTICS( n_sets++; )
   RTOS::callback::start( time );
}

void RTOS::timer::cancel( void ) {
   RTOS_STATISTICS( n_cancels++; )
   RTOS::callback::cancel();
   RTOS::waitable::clear();
}

void RTOS::timer::start( unsigned long int time ) {
   RTOS_STATISTICS( n_sets++; )
   RTOS::callback::start( time );
}

void RTOS::timer::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left  << "timer name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  2 ) << right << "fn";
      stream << setw( 10 ) << right << "sets";
      stream << setw( 10 ) << right << "cancels";
      stream << "\n";
   }
   stream << setw( 18 ) << left  << waitable_name;
   stream << setw( 18 ) << left  << TASK_NAME( t );
   stream << setw(  2 ) << right << dec << nr_from_mask( mask );
   stream << setw( 10 ) << right << dec << n_sets;
   stream << setw( 10 ) << right << dec << n_cancels;
   stream << "\n";
#endif
}

//***************************************************************************
//
// clock
//
//***************************************************************************

RTOS::clock::clock(
   task * t,
   unsigned long int _period,
   const char *name
):
   waitable( t, name ),
   callback( name ),
   period( _period )
#if RTOS_STATISTICS_ENABLED
   , ticks( 0 )
#endif
{
   callback::start( period );
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::clock::time_up( void ) {
   RTOS_STATISTICS( ticks++; )
   callback::restart( period );
   waitable::set();
}

void RTOS::clock::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left  << "clock name";
      stream << setw( 18 ) << left  << "client";
      stream << setw(  2 ) << right << "fn";
      stream << setw( 10 ) << right << "period";
      stream << setw( 10 ) << right << "ticks";
      stream << "\n";
   }
   stream << setw( 18 ) << left  << waitable_name;
   stream << setw( 18 ) << left  << TASK_NAME( t );
   stream << setw(  2 ) << right << dec << nr_from_mask( mask );
   stream << setw( 10 ) << right << dec << period;
   stream << setw( 10 ) << right << dec << ticks;
   stream << "\n";
#endif
}

//***************************************************************************
//
// waitable_set
//
//***************************************************************************

unsigned int RTOS::waitable_set :: waitable_allocate( void ) {
   if( used > 8 * sizeof( current_waitables )) {
      rtos_fatal ("max 32 waitables per group");
   }
   return 1 << used++;
}

void RTOS::waitable_set::set ( const waitable &w ) {

   // set the waitable bit
   current_waitables |= w.mask;

   // the client will figure out whether he is runnable again
   if( requested_waitables != 0 ) {
      w.t->unblock();
   }
}

void RTOS::waitable_set::clear( const waitable &w ) {
   current_waitables &= ~ w.mask;
}

RTOS::event RTOS::waitable_set::wait ( unsigned int mask ) {

   for( ; ; ) {
      // try to find a waitable for which we are waiting
      for (unsigned int i = 0 ; i < used; i++) {
         if( current_waitables & mask & ( 1U << i )) {
            // clear the waitable
            current_waitables &= ~(1U << i);

#if RTOS_STATISTICS_ENABLED
            // update statistics
            for( flag * f = flags; f != NULL; f = f->next_flag ) {
                if (f->t == client && f->mask == (1U << i)) {
                    f->n_gets++;
                    break;
                }
            }
#endif
            // return an event for the waitable
            return event( client, 1U << i );
         }
      }

      // no waitable found? wait for better times..
      requested_waitables = mask;
      client->block();
      requested_waitables = 0;
   }
}

/***************************************************************************\

mutex

\***************************************************************************/

RTOS::mutex::mutex( const char * name ) :
   owner( NULL ),
   waiters( NULL )
#if RTOS_STATISTICS_ENABLED
   , wait_count( 0 )
#endif
{
   RTOS_STATISTICS( mutex_name = string_allocate( name ); )
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::mutex::wait (void) {
   RTOS_STATISTICS( wait_count++; )
   if( owner == NULL ) {
      owner = RTOS::current_task();
   }
   else {
      task *  t = RTOS::current_task();
      task ** p = &waiters;

      // get p to point to the last pointer
      while( *p != NULL ) {
         p = & (*p)-> next_mutex_waiter;
      }

      // insert t after the last pointer
      *p = t;
      t->next_mutex_waiter = NULL;

      // we'll wait for better times...
      t->block();
   }
}

void RTOS::mutex::signal (void) {
   if( owner != RTOS::current_task()) {
      rtos_fatal ("mutex not signaled by owner task");
   }
   else {
      task * t = waiters;
      if ( t != NULL ) {
         // remove task t from the queue
         waiters = waiters->next_mutex_waiter;

         // t is now the owner of the mutex and can run again
         owner = t;
         t->unblock();
      }
      else
         owner = NULL;
   }
}

void RTOS::mutex::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left  << "mutex name";
      stream << setw( 19 ) << left  << "owner";
      stream << setw( 11 ) << right << "waits";
      stream << setw( 10 ) << right << "waiters";
      stream << "\n";
   }
   stream << setw ( 18 ) << left  << mutex_name;
   if( owner == NULL ) {
      stream << setw( 19 ) << left  << "-";
   }
   else {
      stream << setw( 19 ) << left  << owner->task_name;
   }
   stream << setw( 11 ) << right << dec << wait_count;
   stream << setw(  5 ) << right << "[ ";
   if (waiters == NULL)
      stream << '-';
   for( task *t = waiters; t != NULL; t = t->next_mutex_waiter ) {
      stream << left << t->task_name;
      if (t->next_mutex_waiter != NULL)
         stream << ", ";
   }
   stream << " ]\n";
#endif
}

RTOS::mutex::~mutex( void ) {
   rtos_fatal ("mutex destructor called");
}

/***************************************************************************\

callback

\***************************************************************************/

RTOS::callback::callback( const char * name ) :
    time_to_wait (0)
{
   RTOS_STATISTICS( object_name = string_allocate( name ); )
   RTOS::add( this );
}

//***************************************************************************
//
// channel
//
//***************************************************************************

RTOS::channel_base::channel_base( task * t, const char * name ):
   waitable( t, name ),
#if RTOS_STATISTICS_ENABLED
   writes( 0 ),
   ignores( 0 ),
#endif
   qSize( 0 ),
   head( 0 ),
   tail ( 0 )
{
#if RTOS_STATISTICS_ENABLED
   channel_name = string_allocate( name );
   RTOS::add( this );
#endif
}

void RTOS::channel_base::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left  << "channel name";
      stream << setw( 18 ) << left  << "owner";
      stream << setw(  2 ) << right << "fn";
      stream << setw( 10 ) << right << "writes";
      stream << setw( 10 ) << right << "ignores";
      stream << setw(  8 ) << right << "queued";
      stream << "\n";
   }
   stream << setw( 18 ) << left  << channel_name;
   stream << setw( 18 ) << left  << t->task_name;
   stream << setw(  2 ) << right << nr_from_mask( mask );
   stream << setw( 10 ) << right << dec << writes;
   stream << setw( 10 ) << right << dec << ignores;
   stream << setw(  8 ) << right << dec << qSize;
   stream << "\n";
#endif
}

//***************************************************************************
//
// pool
//
//***************************************************************************

RTOS::pool_base::pool_base( const char * name )
#if RTOS_STATISTICS_ENABLED
   : reads( 0 ),
   writes( 0 )
#endif
{
   RTOS_STATISTICS( pool_name = string_allocate( name ); )
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::pool_base::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left << "pool name";
      stream << setw( 10 ) << left << "writes";
      stream << setw( 10 ) << left << "reads";
      stream << "\n";
   }
   stream << setw( 18 ) << left << pool_name;
   stream << setw( 10 ) << left << dec << writes;
   stream << setw( 10 ) << left << dec << reads;
   stream << "\n";
#endif
}

//***************************************************************************
//
// mailbox
//
//***************************************************************************

RTOS::mailbox_base::mailbox_base( const char * name )
#if RTOS_STATISTICS_ENABLED
   : writer( NULL),
   reader( NULL ),
   writes( 0 ),
   reads( 0 )
#endif
{
   RTOS_STATISTICS( mailbox_name = string_allocate( name ); )
   RTOS_STATISTICS( RTOS::add( this ); )
}

void RTOS::mailbox_base::print( std::ostream & stream, bool header ) const {
#if RTOS_STATISTICS_ENABLED
   if( header ) {
      stream << setw( 18 ) << left << "mailbox name";
      stream << setw( 18 ) << left << "writer";
      stream << setw( 18 ) << left << "reader";
      stream << setw( 10 ) << left << "writes";
      stream << setw( 10 ) << left << "reads";
      stream << "\n";
   }
   stream << setw( 18 ) << left << mailbox_name;
   stream << setw( 18 ) << left << TASK_NAME( writer );
   stream << setw( 18 ) << left << TASK_NAME( reader );
   stream << setw( 10 ) << left << dec << writes;
   stream << setw( 10 ) << left << dec << reads;
   stream << "\n";
#endif
}

//***************************************************************************
//
// << operators
//
//***************************************************************************

std::ostream & operator<< ( std::ostream & s, const RTOS::task & t ) {
   t.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::flag & f ) {
   f.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::event & e ) {
   e.print( s );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::timer & t ) {
   t.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::clock & c ) {
   c.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::channel_base & cb ) {
   cb.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::mutex & m ) {
   m.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::mailbox_base & mb ) {
   mb.print( s, false );
   return s;
}

std::ostream & operator<< ( std::ostream & s, const RTOS::pool_base & pb ) {
   pb.print( s, false );
   return s;
}

/***************************************************************************\

RTOS

\***************************************************************************/

// Reference to the task currently executed:
RTOS::task * RTOS::rtos_current_task = NULL;

// timer list head
RTOS::callback * RTOS::timerList = NULL;

// list of tasks, highest prority first
RTOS::task * RTOS::taskList = NULL;

// Return the difference of two timespecs in us
static unsigned long long int diff (timespec & start, timespec & end) {
    unsigned long int temp_sec;
    unsigned long int temp_nsec;
    if (end.tv_nsec - start.tv_nsec < 0) {
        temp_sec  = end.tv_sec - start.tv_sec - 1;
        temp_nsec = 1000000000UL + end.tv_nsec - start.tv_nsec;
    }
    else {
        temp_sec  = end.tv_sec  - start.tv_sec;
        temp_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp_sec * 1000000ULL + temp_nsec / 1000ULL;
}

static timespec rtos_start_time;

unsigned long long int RTOS::run_time( void ) {
    timespec cur_time;
    clock_gettime (CLOCK_REALTIME, &cur_time);

    return diff (rtos_start_time, cur_time);
}

// flag set to clear statistics
bool RTOS::must_clear = false;

// not running yet
bool RTOS::scheduler_running = false;

const char * RTOS::task::name( void ) const {
#if RTOS_STATISTICS_ENABLED
   return task_name;
#else
   return "";
#endif
}

// adding various objects to the RTOS lists
#if RTOS_STATISTICS_ENABLED

RTOS::flag * RTOS::flags = NULL;
RTOS::timer * RTOS::timers = NULL;
RTOS::clock * RTOS::clocks = NULL;
RTOS::mutex * RTOS::mutexes = NULL;
RTOS::channel_base * RTOS::channels = NULL;
RTOS::mailbox_base * RTOS::mailboxes = NULL;
RTOS::pool_base * RTOS::pools = NULL;

void RTOS::add( flag * f ) {
   f->next_flag = flags;
   flags = f;
}

void RTOS::add( timer * t ) {
   t->next_timer = timers;
   timers = t;
}

void RTOS::add( clock * c ) {
   c->next_clock = clocks;
   clocks = c;
}

void RTOS::add( mutex * m ) {
   m->next_mutex = mutexes;
   mutexes = m;
}

void RTOS::add( channel_base * cb ) {
   cb->next_channel = channels;
   channels = cb;
}

void RTOS::add( mailbox_base * mb ) {
   mb->next_mailbox = mailboxes;
   mailboxes = mb;
}

void RTOS::add( pool_base * pb ) {
   pb->next_pool = pools;
   pools = pb;
}

#endif

void RTOS::print( ostream & stream ) {

   // global info
   stream << "RTOS version  : " << RTOS_VERSION << "\n";
   stream << "Fiber version : " << FIBER_VERSION << "\n";
   stream << "RAM free      : " << dec << memory_free() << endl;

#if RTOS_STATISTICS_ENABLED
   bool header;

   if( rtos_current_task != NULL ) {
      rtos_current_task->ignore_this_activation = true;
   }

   // tasks
   header = true;
   for( task * t = taskList; t != NULL; t = t->nextTask ) {
      t->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no tasks\n";
   }
   stream << "\n";

   // flags
   header = true;
   for( flag * f = flags; f != NULL; f = f->next_flag ) {
      f->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no flags\n";
   }
   stream << "\n";

   // timers
   header = true;
   for( timer * t = timers; t != NULL; t = t->next_timer ) {
      t->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no timers\n";
   }
   stream << "\n";

   // clocks
   header = true;
   for( clock * c = clocks; c != NULL; c = c->next_clock ) {
      c->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no clocks\n";
   }
   stream << "\n";

   // channels
   header = true;
   for( channel_base * cb = channels; cb != NULL; cb = cb->next_channel ) {
      cb->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no channels\n";
   }
   stream << "\n";

   // mutexes
   header = true;
   for( mutex * m = mutexes; m != NULL; m = m->next_mutex ) {
      m->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no mutexes\n";
   }
   stream << "\n";

   // mailboxes
   header = true;
   for( mailbox_base * mb = mailboxes; mb != NULL; mb = mb->next_mailbox ) {
      mb->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no mailboxes\n";
   }
   stream << "\n";

   // pools
   header = true;
   for( pool_base * pb = pools; pb != NULL; pb = pb->next_pool ) {
      pb->print( stream, header );
      header = false;
   }
   if( header ) {
      stream << "no pools\n";
   }
   stream << "\n";

#endif
}

unsigned long long int last_run_time;

void RTOS::beat (void) {

   // get the elapse time since last beat, and reset it to 0
   unsigned long long int new_run_time = RTOS::run_time();
   long int elapsed = new_run_time - last_run_time;
   last_run_time = new_run_time;

   if (elapsed > 0) {
      // service the callback timer queue
      for ( callback * t = timerList; t != NULL; t = t->nextTimer ) {
#if ( hartbeat_logging == 1 )
         trace
            RTOS_STATISTICS( << t->object_name )
            << "@" << std::hex << (int) t << std::dec
            << " ttw=" << t->time_to_wait;
#endif
         if( t->time_to_wait > 0 ) {
            t->time_to_wait -= elapsed;
            if( t->time_to_wait <= 0 ) {
               t->time_up();
            }
         }
      }
   }

   // find the highest-priority ready task and run it, then return
   // rtos_current_task is a class attribute, used by current_task
   for (
      rtos_current_task = taskList;
      rtos_current_task != NULL;
      rtos_current_task = rtos_current_task->nextTask
   ) {
      if (rtos_current_task->is_ready()) {
#if ( hartbeat_logging == 1 )
         trace
            << "resume " << rtos_current_task->name()
            << " prio="  << (int) rtos_current_task->priority()
            << "\n";
#endif

         unsigned long long int start = RTOS::run_time();
         rtos_current_task->fiber->resume();
         unsigned long long int end = RTOS::run_time();

#if ( hartbeat_logging == 1 )
         trace << "back from " << rtos_current_task->name() << "\n";
#endif

         unsigned long int runtime = end - start;
         if( !rtos_current_task->ignore_this_activation ) {
            if (rtos_current_task->activations > 1 &&
                runtime > rtos_current_task->runtime_max) {
               rtos_current_task->runtime_max = runtime;
            }
         }

#if ( hartbeat_logging == 1 )
         if( runtime > 10 MS ) {
            trace
               << rtos_current_task->name()
               << " runtime=" << runtime;
         }
#endif
         rtos_current_task->ignore_this_activation = false;
         rtos_current_task->activations++;
         if (must_clear) {
#if ( hartbeat_logging == 1 )
            do_statistics_clear();
#endif
            must_clear = false;
         }

         return;
      }
   }

#if RTOS_STATISTICS_ENABLED

   // no runnable task has been found, nothing to do right now
   // we might as well do deadlock detection
   for( clock * c = clocks; c != NULL; c = c->next_clock ) {
      if( c->t->waitables.requested_waitables & c->mask ) {
         return;
      }
   }
   for( timer * t = timers; t != NULL; t = t->next_timer ) {
      if( ( t->time_to_wait > 0 )
          && ( t->t->waitables.requested_waitables & t->mask ) ) {
         return;
      }
   }

   // no task is waiting for a running timer or clock: DEADLOCK
   cout << "\n\n********** DEADLOCK DETECTED **********\n\n";
   print( cout );

   rtos_fatal ("deadlock detected");

#endif
}

void cc_signal_handler (int) {
   RTOS::display_statistics();
   exit(EXIT_SUCCESS);
}

void RTOS::run (void) {

   // Install a signal handler for Ctrl-C.
#if ( global_logging == 1 )
   std::cout << "\nInstalling signal handler..." << endl;
#endif
   struct sigaction sigusr;
   sigusr.sa_flags = 0;
   sigusr.sa_handler = cc_signal_handler;
   sigemptyset (&sigusr.sa_mask);

   int r = sigaction (SIGINT, &sigusr, NULL);
   if (r == -1)
      rtos_fatal ("Installing signal handler failed");

   // Set realtime scheduling and priority
#if ( global_logging == 1 )
      std::cout << "Installing real-time scheduler..." << endl;
#endif

   // Use the minimum realtime scheduling priority.
   sched_param param;
   param.sched_priority = sched_get_priority_min(SCHED_RR);

   // We'll operate on the currently running thread.
   pthread_t this_thread = pthread_self();

   // Attempt to set the realtime SCHED_RR policy and thread priority
   r = pthread_setschedparam(this_thread, SCHED_RR, &param);
   if (r != 0)
      rtos_fatal("Installing realtime scheduler failed");

#if ( global_logging == 1 )
   std::cout << "Initialising the RTOS run time..." << endl;
#endif
   clock_gettime(CLOCK_REALTIME, &rtos_start_time);
   last_run_time = rtos_start_time.tv_sec  * 1000000ULL +
                   rtos_start_time.tv_nsec / 1000ULL;

   // Show initial statistics
   print( std::cout );

#if ( global_logging == 1 )
   std::cout << "Scheduler starts" << endl;
#endif
   scheduler_running = true;
   rtos_current_task = NULL;
   int n = 0;
   for( ; ; ) {
#if ( hartbeat_logging == 1 )
      if ( ++n > 10000 ) {
         std::cout << '.';
         n = 0;
      }
#endif
      beat();
   }
}

// register a timer
void RTOS::add( callback * t ) {

   // add the timer to the timer list
   t->nextTimer = timerList;
   timerList = t;
}

// register a task
void RTOS::add( task * new_task ) {
#if ( task_logging == 1 )
   trace
      << "register task " << new_task->name()
      << " priority=" << new_task->task_priority;
#endif

   if( new_task->task_priority > RTOS_MIN_PRIORITY ) {
      rtos_fatal ("illegal task priority");
   }

   // walk the task queue untill the next task either
   // - does not exist, or
   // - has a lower priority (higher priority number) than the new task
   task ** t = &taskList;

   while( ( *t != NULL ) && ( (*t)->task_priority <= new_task->task_priority ) ) {
      // if the task priorities are equal, increment the priority
      // of the newly allocated task if possible
      if( (*t)->task_priority == new_task->task_priority ) {
         if( new_task->task_priority < RTOS_MIN_PRIORITY ) {
            new_task->task_priority++;
         }
         else {
            rtos_fatal ("duplicate task priority");
         }
      }
      t = &( (*t)->nextTask );
   }
   // now insert the new task after the current task
   new_task->nextTask = *t;
   *t = new_task;

#if ( task_logging == 1 )
   trace << "registering done ";
#endif
}

void RTOS::do_statistics_clear (void) {
   for (
      task * task = taskList;
      task != NULL;
      task = task->nextTask
   ) {
      task->statistics_clear();
   }
}

void RTOS::display_statistics( void ) {
   std::cout << std::endl;
   print (std::cout);
   statistics_clear();
}
