/***************************************************************************\

file      RTOS.cpp
version   4.00
author    Marten Wensink / Wouter van Ooijen
date      2014-11-01

\***************************************************************************/

#include "bmptk.h"
#include "pRTOS.h"
#include "switch_to.h"

// using namespace std;

#define task_logging             (global_logging && 0)
#define debug_task_logging       (global_logging && 0)
#define hartbeat_logging         (global_logging && 0)

#define TASK_STATE( task ) \
   ( (task)->is_blocked() ? "B" : "-" ) << \
   ( (task)->is_suspended() ? "S" : "-" ) << \
   ( (task)->is_ready() ? "R" : "-" )

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

int nr_from_mask(unsigned int mask) {
	for (int i = 0; i < 32; i++) {
		if (mask & (1 << i)) {
			return i;
		}
	}
	return -1;
}

void task_trampoline(void) {
	RTOS::current_task()->main();
	rtos_fatal(
		"\n>> Fatal error: task "
		<< RTOS::current_task()->name()
		<< " returned from main()\n");
}


/***************************************************************************\

task

\***************************************************************************/

void strncopy(char * d, int n, const char *s) {
	while (--n && (*s != '\0')) {
		*d++ = *s++;
	}
	*d = '\0';
}

constexpr int marker = 0xDEADBEEF;

RTOS::task::task(unsigned int priority, const char * tname) :
	task_is_blocked(false),
	task_is_suspended(false),
	task_priority(priority),
	waitables(this),
	sleep_timer(this, "__sleep"),
	logging(task_logging)
{
	RTOS_STATISTICS(strncopy(task_name, sizeof(task_name), tname); )

		for (unsigned int i = 0; i < RTOS_WORD_STACK_SIZE; ++i) {
			this->stack[i] = marker;
		}

	this->stack[RTOS_WORD_STACK_SIZE - 1] = (int)task_trampoline;
	this->stored_stack_pointer = &(this->stack[RTOS_WORD_STACK_SIZE - 10]);

	ignore_this_activation = false;
	statistics_clear();
	RTOS::add(this);
	task_trace << "CREATED";
}

int RTOS::task::stack_unused() const {
	unsigned int i = 0;
	while (
		(i < RTOS_WORD_STACK_SIZE)
		&& (this->stack[i] == marker)
		) {
		++i;
	}
	return 4 * i;
}

void RTOS::task::suspend(void) {
	task_trace << "suspend";
	task_is_suspended = true;
	release();
}

void RTOS::task::resume(void) {
	task_trace << "resume";
	task_is_suspended = false;
	release();
}

void RTOS::task::unblock(void) {
	/*
	if( ! rtos_running ) {
	return;
	}*/
	task_trace << "unblock";
	task_is_blocked = false;

	release();
}

void RTOS::task::block(void) {
	/*if( ! rtos_running ) {
	return;
	}*/
	task_trace << "block";

	// Only a running task can block itself
	if (RTOS::current_task() != this) {
		rtos_fatal("task not blocked by itself");
	}

	if (!is_ready()) {
		rtos_fatal("running task is not READY!?");
	}

	task_is_blocked = true;
	release();
}

void RTOS::task::sleep(bmptk::time time) {
	sleep_timer.set(time);
	wait(sleep_timer);
}

void RTOS::task::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream
			<< hwcpp::io::endl << hwcpp::io::dec
			<< hwcpp::io::setw(16) << hwcpp::io::left << "task name"
			<< hwcpp::io::setw(6) << hwcpp::io::right << "prio"
			<< hwcpp::io::setw(7) << hwcpp::io::right << "state"
			<< hwcpp::io::setw(10) << hwcpp::io::right << "rt_max"
			<< hwcpp::io::setw(10) << hwcpp::io::right << "active"
			<< hwcpp::io::setw(11) << hwcpp::io::right << "st u/m (b)"
			<< "\n";
	}
	stream
		<< hwcpp::io::dec
		<< hwcpp::io::setw(16) << hwcpp::io::left << task_name
		<< hwcpp::io::setw(6) << hwcpp::io::right << hwcpp::io::dec << task_priority
		<< hwcpp::io::setw(4) << hwcpp::io::right << TASK_STATE(this)
		<< hwcpp::io::setw(11) << hwcpp::io::right << runtime_max
		<< hwcpp::io::setw(10) << hwcpp::io::right << activations
		<< hwcpp::io::setw(6) << hwcpp::io::right << stack_used()
		<< "/"
		<< hwcpp::io::setw(6) << hwcpp::io::left << stack_size()
		<< "\n";
#endif
}

//***************************************************************************
//
// event
//
//***************************************************************************

bool RTOS::event :: operator==(const event & rhs) const {
	if (t != rhs.t) {
		rtos_fatal("comparing incompatible waitables");
	}
	return mask == rhs.mask;
}

bool RTOS::event :: operator!=(const event & rhs) const {
	if (t != rhs.t) {
		rtos_fatal("comparing incompatible waitables");
	}
	return mask != rhs.mask;
}

bool RTOS::event :: operator==(const waitable & rhs) const {
	return *this == (event)rhs;
}

bool RTOS::event :: operator!=(const waitable & rhs) const {
	return *this != (event)rhs;
}

RTOS::event RTOS::event :: operator+(const event & rhs) const {
	if (t != rhs.t) {
		rtos_fatal("adding incompatible waitables");
	}
	return event(t, mask | rhs.mask);
}

void RTOS::event::print(hwcpp::io::ostream & s) const {
	s << "event t=" << t->name() << " m=0x" << std::hex << mask;
}


//***************************************************************************
//
// waitable
//
//***************************************************************************

RTOS::waitable::waitable(task * t, const char * arg_name) :
	event(t, 0)
{
	RTOS_STATISTICS(strncopy(waitable_name, sizeof(waitable_name), arg_name); )
		mask = t->waitables.waitable_allocate();
}

//***************************************************************************
//
// flag
//
//***************************************************************************

RTOS::flag::flag(task * t, const char * name) :
	waitable(t, name)
#if RTOS_STATISTICS_ENABLED
	, n_sets(0)
	, n_gets(0)
#endif
{
	RTOS_STATISTICS(RTOS::add(this); )
}

void RTOS::flag::set(void) {
	RTOS_STATISTICS(n_sets++; )
		waitable::set();
}

using namespace hwcpp::io;

void RTOS::flag::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "flag name";
		stream << setw(18) << left << "client";
		stream << setw(3) << right << "fn";
		stream << setw(8) << right << "sets";
		stream << setw(8) << right << "gets";
		stream << "\n";
	}
	stream << setw(18) << left << waitable_name;
	stream << setw(18) << left << TASK_NAME(t);
	stream << setw(3) << right << dec << nr_from_mask(mask);
	stream << setw(8) << right << dec << n_sets;
	stream << setw(8) << right << dec << n_gets;
	stream << "\n";
#endif
}

//***************************************************************************
//
// timer
//
//***************************************************************************

RTOS::timer::timer(task * t, const char * name) :
	waitable(t, name),
	callback(name)
#if RTOS_STATISTICS_ENABLED
	, n_sets(0)
	, n_cancels(0)
#endif
{
	RTOS_STATISTICS(RTOS::add(this); )
}

void RTOS::timer::set(bmptk::time time) {
	RTOS_STATISTICS(n_sets++; )
		RTOS::callback::start(time);
}

void RTOS::timer::cancel(void) {
	RTOS_STATISTICS(n_cancels++; )
		RTOS::callback::cancel();
	RTOS::waitable::clear();
}

void RTOS::timer::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "timer name";
		stream << setw(18) << left << "client";
		stream << setw(3) << right << "fn";
		stream << setw(8) << right << "sets";
		stream << setw(9) << right << "cancels";
		stream << "\n";
	}
	stream << setw(18) << left << waitable_name;
	stream << setw(18) << left << TASK_NAME(t);
	stream << setw(3) << right << dec << nr_from_mask(mask);
	stream << setw(8) << right << dec << n_sets;
	stream << setw(9) << right << dec << n_cancels;
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
	bmptk::time interval,
	const char *name
	) :
	waitable(t, name),
	callback(name),
	_interval(interval)
#if RTOS_STATISTICS_ENABLED
	, ticks(0)
#endif
{
	callback::start(_interval);
	RTOS_STATISTICS(RTOS::add(this); )
}

void RTOS::clock::time_up(void) {
	RTOS_STATISTICS(ticks++; )
		callback::restart(_interval);
	waitable::set();
}

void RTOS::clock::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "clock name";
		stream << setw(18) << left << "client";
		stream << setw(3) << right << "fn";
		stream << setw(10) << right << "interval";
		stream << setw(8) << right << "ticks";
		stream << "\n";
	}
	stream << setw(18) << left << waitable_name;
	stream << setw(18) << left << TASK_NAME(t);
	stream << setw(3) << right << dec << nr_from_mask(mask);
	stream << setw(10) << right << dec << _interval;
	stream << setw(8) << right << dec << ticks;
	stream << "\n";
#endif
}

//***************************************************************************
//
// waitable_set
//
//***************************************************************************

unsigned int RTOS::waitable_set::waitable_allocate(void) {
	if (used > 8 * sizeof(current_waitables)) {
		rtos_fatal("max 32 waitables per task");
	}
	return 1 << used++;
}

void RTOS::waitable_set::set(const waitable &w) {

	// set the waitable bit
	current_waitables |= w.mask;

	// the client will figure out whether he is runnable again
	if (requested_waitables != 0) {
		w.t->unblock();
	}
}

void RTOS::waitable_set::clear(const waitable &w) {
	current_waitables &= ~w.mask;
}

RTOS::event RTOS::waitable_set::wait(unsigned int mask) {

	for (; ; ) {
		// try to find a waitable for which we are waiting
		for (unsigned int i = 0; i < used; i++) {
			if (current_waitables & mask & (1U << i)) {
				// clear the waitable
				current_waitables &= ~(1U << i);

#if RTOS_STATISTICS_ENABLED
				// update statistics
				for (flag * f = flags; f != NULL; f = f->next_flag) {
					if (f->t == client && f->mask == (1U << i)) {
						f->n_gets++;
						break;
					}
				}
#endif
				// return an event for the waitable
				return event(client, 1U << i);
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

RTOS::mutex::mutex(const char * name) :
	owner(NULL),
	waiters(NULL)
#if RTOS_STATISTICS_ENABLED
	, wait_count(0)
#endif
{
	RTOS_STATISTICS(strncopy(mutex_name, sizeof(mutex_name), name); )
		RTOS_STATISTICS(RTOS::add(this); )
}

void RTOS::mutex::wait(void) {
	RTOS_STATISTICS(wait_count++; )
		if (owner == NULL) {
			owner = RTOS::current_task();
		}
		else {
			task *  t = RTOS::current_task();
			task ** p = &waiters;

			// get p to point to the last pointer
			while (*p != NULL) {
				p = &(*p)->next_mutex_waiter;
			}

			// insert t after the last pointer
			*p = t;
			t->next_mutex_waiter = NULL;

			// we'll wait for better times...
			t->block();
		}
}

void RTOS::mutex::signal(void) {
	if (owner != RTOS::current_task()) {
		rtos_fatal("mutex not signaled by owner task");
	}
	else {
		task * t = waiters;
		if (t != NULL) {
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

void RTOS::mutex::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "mutex name";
		stream << setw(18) << left << "owner";
		stream << setw(5) << right << "waits";
		stream << "   waiters";
		stream << "\n";
	}
	stream << setw(18) << left << mutex_name;
	if (owner == NULL) {
		stream << setw(18) << left << "-";
	}
	else {
		stream << setw(18) << left << owner->task_name;
	}
	stream << setw(5) << right << dec << wait_count;
	stream << "   [ ";
	for (task *t = waiters; t != NULL; t = t->next_mutex_waiter) {
		stream << t->task_name << " ";
	}
	stream << "]\n";
#endif
}

RTOS::mutex::~mutex(void) {
	rtos_fatal("mutex destructor called");
}

/***************************************************************************\

callback

\***************************************************************************/

RTOS::callback::callback(const char * name) :
	time_to_wait(0)
{
	RTOS_STATISTICS(strncopy(object_name, sizeof(object_name), name); )
		RTOS::add(this);
}

//***************************************************************************
//
// channel
//
//***************************************************************************

RTOS::channel_base::channel_base(task * t, const char * name) :
	waitable(t, name),
#if RTOS_STATISTICS_ENABLED
	writes(0),
	ignores(0),
#endif
	qSize(0),
	head(0),
	tail(0)
{
#if RTOS_STATISTICS_ENABLED
	strncopy(channel_name, sizeof(channel_name), name);
	RTOS::add(this);
#endif
}

void RTOS::channel_base::print(hwcpp::io::ostream & stream, bool header) const {

#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "channel name";
		stream << setw(18) << left << "owner";
		stream << setw(3) << right << "fn";
		stream << setw(8) << right << "writes";
		stream << setw(9) << right << "ignores";
		stream << setw(10) << right << "cur.size";
		stream << "\n";
	}
	stream << setw(18) << left << channel_name;
	stream << setw(18) << left << t->task_name;
	stream << setw(3) << right << nr_from_mask(mask);
	stream << setw(8) << right << dec << writes;
	stream << setw(9) << right << dec << ignores;
	stream << setw(10) << right << dec << qSize;
	stream << "\n";
#endif
}

//***************************************************************************
//
// pool
//
//***************************************************************************

RTOS::pool_base::pool_base(const char * name)
#if RTOS_STATISTICS_ENABLED
	: reads(0), writes(0)
#endif
{
	RTOS_STATISTICS(strncopy(pool_name, sizeof(pool_name), name); )
		RTOS_STATISTICS(RTOS::add(this); )
}

void RTOS::pool_base::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "pool name";
		stream << setw(8) << right << "writes";
		stream << setw(8) << right << "reads";
		stream << "\n";
	}
	stream << setw(18) << left << pool_name;
	stream << setw(8) << right << dec << writes;
	stream << setw(8) << right << dec << reads;
	stream << "\n";
#endif
}

//***************************************************************************
//
// mailbox
//
//***************************************************************************

RTOS::mailbox_base::mailbox_base(const char * name)
#if RTOS_STATISTICS_ENABLED
	: writer(NULL),
	reader(NULL),
	writes(0),
	reads(0)
#endif
{
	RTOS_STATISTICS(strncopy(mailbox_name, sizeof(mailbox_name), name); )
		RTOS_STATISTICS(RTOS::add(this); )
}

void RTOS::mailbox_base::print(hwcpp::io::ostream & stream, bool header) const {
#if RTOS_STATISTICS_ENABLED
	if (header) {
		stream << setw(18) << left << "mailbox name";
		stream << setw(18) << left << "writer";
		stream << setw(18) << left << "reader";
		stream << setw(8) << right << "writes";
		stream << setw(8) << right << "reads";
		stream << "\n";
	}
	stream << setw(18) << left << mailbox_name;
	stream << setw(18) << left << TASK_NAME(writer);
	stream << setw(18) << left << TASK_NAME(reader);
	stream << setw(8) << right << dec << writes;
	stream << setw(8) << right << dec << reads;
	stream << "\n";
#endif
}

//***************************************************************************
//
// << operators
//
//***************************************************************************

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::task & t) {
	t.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::flag & f) {
	f.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::event & e) {
	e.print(s);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::timer & t) {
	t.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::clock & c) {
	c.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::channel_base & cb) {
	cb.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::mutex & m) {
	m.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::mailbox_base & mb) {
	mb.print(s, false);
	return s;
}

hwcpp::io::ostream & operator<< (hwcpp::io::ostream & s, const RTOS::pool_base & pb) {
	pb.print(s, false);
	return s;
}

/***************************************************************************\

RTOS

\***************************************************************************/

// Reference to the task currently executed:
RTOS::task * RTOS::rtos_current_task = nullptr;

// timer list head
RTOS::callback * RTOS::timerList = nullptr;

// list of tasks, highest prority first
RTOS::task * RTOS::taskList = nullptr;

// int * * old_task_sp_ptr;

bmptk::time RTOS::run_time(void) {
	return bmptk::now();
}

// flag set to clear statistics
bool RTOS::must_clear = false;

// not running yet
bool RTOS::scheduler_running = false;

const char * RTOS::task::name(void) const {
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

void RTOS::add(flag * f) {
	f->next_flag = flags;
	flags = f;
}

void RTOS::add(timer * t) {
	t->next_timer = timers;
	timers = t;
}

void RTOS::add(clock * c) {
	c->next_clock = clocks;
	clocks = c;
}

void RTOS::add(mutex * m) {
	m->next_mutex = mutexes;
	mutexes = m;
}

void RTOS::add(channel_base * cb) {
	cb->next_channel = channels;
	channels = cb;
}

void RTOS::add(mailbox_base * mb) {
	mb->next_mailbox = mailboxes;
	mailboxes = mb;
}

void RTOS::add(pool_base * pb) {
	pb->next_pool = pools;
	pools = pb;
}

#endif

void RTOS::print(hwcpp::io::ostream & stream) {

	stream << "RTOS version  : " << RTOS_VERSION << "\n";

#if RTOS_STATISTICS_ENABLED
	bool header;

	if (rtos_current_task != NULL) {
		rtos_current_task->ignore_this_activation = true;
	}

	// tasks
	header = true;
	for (task * t = taskList; t != NULL; t = t->nextTask) {
		t->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no tasks\n";
	}
	stream << "\n";

	// flags
	header = true;
	for (flag * f = flags; f != NULL; f = f->next_flag) {
		f->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no flags\n";
	}
	stream << "\n";

	// timers
	header = true;
	for (timer * t = timers; t != NULL; t = t->next_timer) {
		t->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no timers\n";
	}
	stream << "\n";

	// clocks
	header = true;
	for (clock * c = clocks; c != NULL; c = c->next_clock) {
		c->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no clocks\n";
	}
	stream << "\n";

	// channels
	header = true;
	for (channel_base * cb = channels; cb != NULL; cb = cb->next_channel) {
		cb->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no channels\n";
	}
	stream << "\n";

	// mutexes
	header = true;
	for (mutex * m = mutexes; m != NULL; m = m->next_mutex) {
		m->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no mutexes\n";
	}
	stream << "\n";

	// mailboxes
	header = true;
	for (mailbox_base * mb = mailboxes; mb != NULL; mb = mb->next_mailbox) {
		mb->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no mailboxes\n";
	}
	stream << "\n";

	// pools
	header = true;
	for (pool_base * pb = pools; pb != NULL; pb = pb->next_pool) {
		pb->print(stream, header);
		header = false;
	}
	if (header) {
		stream << "no pools\n";
	}
	stream << "\n";

#endif
}

unsigned long long int last_run_time;

int * stored_main_stack_pointer;

void RTOS::task::release(void) {
	if (rtos_current_task) {
		switch_from_to(
			&(rtos_current_task->stored_stack_pointer),
			stored_main_stack_pointer
			);
	}
}

void RTOS::beat(void) {

	// get the elapse time since last beat, and reset it to 0
	bmptk::time new_run_time = RTOS::run_time();
	long int elapsed = new_run_time - last_run_time;
	last_run_time = new_run_time;

	if (elapsed > 0) {
		// service the callback timer queue
		for (callback * t = timerList; t != NULL; t = t->nextTimer) {
			if (hartbeat_logging) {
				trace
					RTOS_STATISTICS(<< t->object_name)
					<< "@" << std::hex << (int)t << std::dec
					<< " ttw=" << t->time_to_wait;
			}
			if (t->time_to_wait > 0) {
				t->time_to_wait -= elapsed;
				if (t->time_to_wait <= 0) {
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
			if (hartbeat_logging) {
				trace
					<< "resume " << rtos_current_task->name()
					<< " prio=" << (int)rtos_current_task->priority()
					<< "\n";
			}

			bmptk::time start = RTOS::run_time();
			switch_from_to(
				&stored_main_stack_pointer,
				rtos_current_task->stored_stack_pointer
				);
			bmptk::time end = RTOS::run_time();

			if (hartbeat_logging) {
				trace << "back from " << rtos_current_task->name() << "\n";
			}

			bmptk::time runtime = end - start;
			if (!rtos_current_task->ignore_this_activation) {
				if (runtime > rtos_current_task->runtime_max) {
					rtos_current_task->runtime_max = runtime;
				}
			}

			rtos_current_task->ignore_this_activation = false;
			rtos_current_task->activations++;
			if (must_clear) {
				if (hartbeat_logging) do_statistics_clear();
				must_clear = false;
			}

			rtos_current_task = nullptr;
			return;
		}
	}

#if RTOS_STATISTICS_ENABLED

	// no runnable task has been found, nothing to do right now
	// we might as well do deadlock detection
	for (clock * c = clocks; c != NULL; c = c->next_clock) {
		if (c->t->waitables.requested_waitables & c->mask) {
			return;
		}
	}
	for (timer * t = timers; t != NULL; t = t->next_timer) {
		if ((t->time_to_wait > 0)
			&& (t->t->waitables.requested_waitables & t->mask)) {
			return;
		}
	}

	// no task is waiting for a running timer or clock: DEADLOCK
	hwcpp::io::cout << "\n\n********** DEADLOCK DETECTED **********\n\n";
	print(cout);

	rtos_fatal("deadlock detected");

#endif
}

void RTOS::run(void) {

	// Show initial statistics
	print(hwcpp::io::cout);

	if (global_logging) {
		hwcpp::io::cout << "Scheduler starts" << endl;
	}
	scheduler_running = true;
	int n = 0;
	for (; ; ) {
		if (hartbeat_logging) {
			if (++n > 10000) {
				hwcpp::io::cout << '.';
				n = 0;
			}
		}
		beat();
	}
}

// register a timer
void RTOS::add(callback * t) {

	// add the timer to the timer list
	t->nextTimer = timerList;
	timerList = t;
}

// register a task
void RTOS::add(task * new_task) {
	if (task_logging) {
		trace
			<< "register task " << new_task->name()
			<< " priority=" << new_task->task_priority;
	}

	if (new_task->task_priority > RTOS_MIN_PRIORITY) {
		rtos_fatal("illegal task priority");
	}

	// walk the task queue untill the next task either
	// - does not exist, or
	// - has a lower priority (higher priority number) than the new task
	task ** t = &taskList;

	while ((*t != NULL) && ((*t)->task_priority <= new_task->task_priority)) {
		// if the task priorities are equal, increment the priority
		// of the newly allocated task if possible
		if ((*t)->task_priority == new_task->task_priority) {
			if (new_task->task_priority < RTOS_MIN_PRIORITY) {
				new_task->task_priority++;
			}
			else {
				rtos_fatal("duplicate task priority");
			}
		}
		t = &((*t)->nextTask);
	}
	// now insert the new task after the current task
	new_task->nextTask = *t;
	*t = new_task;

	if (task_logging) {
		trace << "registering done ";
	}
}

void RTOS::do_statistics_clear(void) {
	for (
		task * task = taskList;
		task != NULL;
		task = task->nextTask
		) {
		task->statistics_clear();
	}
}

void RTOS::display_statistics(void) {
	print(hwcpp::io::cout);
	statistics_clear();
}
