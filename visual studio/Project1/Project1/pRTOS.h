#ifndef __RTOS
#define __RTOS

#include "bmptk.h"

#define RTOS_VERSION "V4.00-CM0 (2014-12-02)"

#define global_logging 1

#define RTOS_STATISTICS_ENABLED 1

const unsigned int RTOS_WORD_STACK_SIZE = 100;

const unsigned int RTOS_MIN_PRIORITY = 1000;

// The macro HERE transates to a newline, the file-name, ":", and
// the line-number of the place where the HERE macro appears.
// This can be used for debug logging. used by the trace macro.
#define HERE_STR( X ) #X
#define HERE2( F, L ) ( "\n" F ":" HERE_STR( L ))
#define HERE HERE2( __FILE__, __LINE__ )

// Printing to trace (instead of cout) prepends HERE and a space,
// or trace can be used standalone like 'trace;'
#define trace ( hwcpp::io::cout << HERE << " " )

// the macro RTOS_STATISTICS is used to prefix a single line
// that will be commented out when statistics is disabled
#if RTOS_STATISTICS_ENABLED
#define RTOS_STATISTICS( x ) x
#else
#define RTOS_STATISTICS( x )
#endif

#define rtos_fatal(msg) \
    {  hwcpp::io::cout << "\n >> Fatal error: " << msg << "\n"; while(1); }

//***************************************************************************
//
// class RTOS
//
//***************************************************************************

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
	static void do_statistics_clear(void);

	//! this is a static class, prevent instantiation
	RTOS(void) { }

	//! the list of callabacks, in no particular order
	static callback * timerList;

	//! add a callback
	static void add(callback * t);

	//! the list of all tasks, highest priority first
	static task * taskList;

	//! add a task to the \ref taskList
	static void add(task * task);

	//! indicates whether the scheduler is running
	static bool scheduler_running;

	//! called repeatedly by run
	static void beat(void);

#if RTOS_STATISTICS_ENABLED
	static flag * flags;
	static timer * timers;
	static clock * clocks;
	static mutex * mutexes;
	static channel_base * channels;
	static mailbox_base * mailboxes;
	static pool_base * pools;

	static void add(flag * f);
	static void add(timer * t);
	static void add(clock * c);
	static void add(mutex * m);
	static void add(channel_base * cb);
	static void add(mailbox_base * mb);
	static void add(pool_base * pb);
#endif

public:
	//! runs the scheduler
	static void run(void);

	//! returns (a pointer to) the currently executing task
	static task * current_task(void) { return rtos_current_task; }

	//! get elapsed time in micro seconds since OS startup
	static bmptk::time run_time(void);

	//! prints statistics about the RTOS to the stream.
	static void print(hwcpp::io::ostream & stream);

	//! clears the statistics.
	//
	//! The actual clearing will be done later, inside \ref run(),
	//! when the current task has given up the processor.
	static void statistics_clear(void) { must_clear = true; }

	//! print the statistics collect for the used RTOS objects
	static void display_statistics(void);

	//************************************************************************
	//
	// class event
	//
	//************************************************************************

public:
	class event {
	public:

		//! prints an event, for debugging only
		void print(hwcpp::io::ostream & s) const;

		//! report wether two events are the same
		bool operator==(const event & rhs) const;

		//! report whether an event corresponds to a waitable
		bool operator==(const waitable & rhs) const;

		//! report wether two events are not the same
		bool operator!=(const event & rhs) const;

		//! report whether an event does not correspond to a waitable
		bool operator!=(const waitable & rhs) const;

		//! add two waitables, result can be used in a wait() call
		//
		//! Waitables can be added (operator+) to construct a
		//! 'set of waitables' as argument to a task::wait() call.
		event operator+(const event & rhs) const;

	protected:

		//! the owner task
		task * t;

		//! the mask of this event, one bit is set
		//
		//! The bit that is set is unique among the events
		//! owned by a task.
		unsigned int mask;

		//! constructor, used by concrete events
		event(task * t, unsigned int mask) : t(t), mask(mask) { }

		friend class waitable_set;
	};

	//************************************************************************
	//
	// class waitable
	//
	//***********************************************************************
public:
	class waitable : public event {
	public:

		//! clear the waitable
		//
		//! This is automatically doen when the waitable
		//! causes a task::wait() call to return it.
		virtual void clear(void) { t->waitables.clear(*this); }

	protected:

		//! constructor, specify owner and name
		//
		//! The name is used for debugging only.
		waitable(task * task, const char * name);

		//! set the waitable
		void set(void) { t->waitables.set(*this); }

		RTOS_STATISTICS(char waitable_name[20]; )

			friend class waitable_set;
	};


	//************************************************************************
	//
	// class callback
	//
	//************************************************************************

private:
	class callback {
	public:

		callback(const char * name = "");

		// a timer should never be destroyed
		~callback(void) {
			rtos_fatal("callback_timer destructor called");
		}

		// a timer's time_up function must be provided by a derived class
		virtual void time_up(void) = 0;

	protected:
		// the callback must fire 'time' from now
		void start(bmptk::time time) { time_to_wait = time; }

		// the callback must fire 'time' from the last firing
		void restart(unsigned long int time) {
			while (time_to_wait <= 0) { time_to_wait += time; }
		}

		// abort a started timer
		virtual void cancel(void) { time_to_wait = 0; }

		RTOS_STATISTICS(char object_name[20]; )

			// the number of microseconds until the next time_up
			bmptk::time time_to_wait;

		// called by the RTOS to state that <elapsed> time has elapsed
		void tick(int elapsed);

		// for the RTOS to link timers in a chain
		callback * nextTimer;

		friend class RTOS;
	};



	//************************************************************************
	//
	// class timer
	//
	//
	//************************************************************************

public:
	class timer : public waitable, public callback {
	public:

		//! create a timer for task t, specify its name
		timer(task * t, const char * name = "");

		//! set the timer to timeout after the specified time
		//
		//! Start the timer: it will set itself after the indicated timeout,
		//! starting from now.
		//! When the timer was already running the previous timout
		//! is overwritten.
		void set(bmptk::time time);

		//! stop and clear the timer
		//
		//! Stop the timer (when it was running),
		//! and clears its (when it was set).
		void cancel(void);

		//! print the timer (for debugging)
		void print(hwcpp::io::ostream & s, bool header = true) const;

	private:
		void time_up(void) { waitable::set(); }

		RTOS_STATISTICS(timer * next_timer; )
			RTOS_STATISTICS(unsigned int n_sets; )
			RTOS_STATISTICS(unsigned int n_cancels; )

			friend class RTOS;
	};


	//************************************************************************
	//
	// class flag
	//
	//************************************************************************

public:
	class flag : public waitable {
	public:

		//! constructor, specify onwer and name
		//
		//! This call creates a timer for task t.
		//! The name is used for debugging and statistics.
		flag(task * t, const char * name = "");

		//! sets the flag
		//
		//! Setting a flag causes the task that waits for this
		//! flag to be awakened.
		void set(void);

		//! prints flag infomation (for debugging)
		void print(hwcpp::io::ostream & s, bool header = true) const;

	private:

		// this information is needed for statistics only
		RTOS_STATISTICS(flag * next_flag; )
			RTOS_STATISTICS(unsigned int n_sets; )
			RTOS_STATISTICS(unsigned int n_gets; )

			friend class RTOS;
	};


	//************************************************************************
	//
	// class waitable_set
	//
	//************************************************************************

private:
	class waitable_set {
	private:
		event wait(unsigned int mask);

	public:
		task * client;
		unsigned int current_waitables;
		unsigned int requested_waitables;
		unsigned int used;

		waitable_set(task * client) :
			client(client),
			current_waitables(0),
			requested_waitables(0),
			used(0) {}

		unsigned int waitable_allocate(void);
		void set(const waitable &w);
		void clear(const waitable &w);

		event wait(void) { return wait(~0); }
		event wait(const waitable & w) { return wait(w.mask); }
		event wait(const event & set) { return wait(set.mask); }
	};


	//************************************************************************
	//
	// class RTOS::task
	//
	//************************************************************************

public:
	class task {

	private:

		bool task_is_blocked;

		bool task_is_suspended;

		RTOS_STATISTICS(char task_name[20]; )

			unsigned int task_priority;

	protected:

		virtual void main(void) = 0;

	private:

		friend void task_trampoline(void);

	public:

		task(
			unsigned int priority = RTOS_MIN_PRIORITY,
			const char * tname = ""
			);

		~task(void) {
			rtos_fatal("task destructor called");
		}

		virtual void suspend(void);

		virtual void resume(void);

		void release(void);

		void sleep(bmptk::time time);

		unsigned int priority(void) const { return task_priority; }

		const char * name(void) const;

		bool is_suspended(void) const { return task_is_suspended; }

		bool is_blocked(void) const { return task_is_blocked; }

		bool is_ready(void) const {
			return !(task_is_suspended || task_is_blocked);
		}

		void logging_set(bool log) { logging = log; }

		void print(hwcpp::io::ostream & stream, bool header = true) const;

		event wait(void) { return waitables.wait(); }

		event wait(const waitable & w) { return waitables.wait(w); }

		event wait(const event & set) { return waitables.wait(set); }

		void set(flag &f) { f.set(); }

		void ignore_activation_time(void) {
			ignore_this_activation = true;
		}

		int stack_used() const { return 4 * RTOS_WORD_STACK_SIZE - stack_unused(); }

		int stack_unused() const;

		int stack_size() const { return 4 * RTOS_WORD_STACK_SIZE; }

	private:

		int stack[RTOS_WORD_STACK_SIZE];

		void statistics_clear(void) {
			runtime_max = 0;
			activations = 0;
		}

		waitable_set waitables;

		timer sleep_timer;

		void block(void);

		void unblock(void);

		//! statistics
		int activated;
		int latency_max;
		bmptk::time runtime_max;
		int activations;
		bool ignore_this_activation;

		//! for debugging
		bool logging;

		//! for debugging
		void debug(const char * msg);

		//! for the RTOS to link all tasks
		task * nextTask;

		//! for the mutexes to link waiting tasks
		task * next_mutex_waiter;

		//! storage for the stack pointer of an inactive task
		int * stored_stack_pointer;

		friend class periodic_task;
		friend class waitable_set;
		friend class flag;
		friend class RTOS;
	};



	//************************************************************************
	//
	// class clock
	//
	//************************************************************************

public:
	class clock : public waitable, public callback {
	public:

		clock(
			task * t,
			bmptk::time interval,
			const char * name = ""
			);

		void clear(void) { waitable::clear(); }

		bmptk::time interval(void) { return _interval; }

		void print(hwcpp::io::ostream & s, bool header = true) const;

	private:
		void time_up(void);
		bmptk::time _interval;

		RTOS_STATISTICS(clock * next_clock; )
			RTOS_STATISTICS(unsigned int ticks; )

			friend class RTOS;
	};


	//************************************************************************
	//
	// class pool_base
	//
	//************************************************************************

public:
	class pool_base {
	public:
		pool_base(const char * name);
		~pool_base(void) {
			rtos_fatal("pool destructor called");
		}
		void print(hwcpp::io::ostream & s, bool header = true) const;

#if RTOS_STATISTICS_ENABLED
		unsigned int reads;
		unsigned int writes;
		pool_base * next_pool;
		char pool_name[20];
#endif

		friend class RTOS;
	};

	//************************************************************************
	//
	// class pool
	//
	//************************************************************************

public:
	template <class T> class pool : public pool_base {
	public:

		pool(const char * name = "") : pool_base(name) { }

		void write(T item) {
			RTOS_STATISTICS(writes++; )
				data = item;
		}

		//! atomic read operation on a pool
		//
		//! @copydetails  write
		T read(void) {
			RTOS_STATISTICS(reads++; )
				return data;
		}

	private:
		T data;
	};


	//************************************************************************
	//
	// class mutex
	//!
	//************************************************************************

public:
	class mutex {
	public:

		//! constructor, specify the name
		//
		//! The name is used for debugging only.
		mutex(const char * name = "");

		//! generates an error
		//
		//! A mutex should never be destroyed
		~mutex(void);

		//! prints a mutex, for debugging only.
		void print(hwcpp::io::ostream & stream, bool header = true) const;

		void wait(void);

		void signal(void);

	private:
		task * owner;             // current owner of the mutex
		task * waiters;           // head of the waiting tasks queue

#if RTOS_STATISTICS_ENABLED
		char mutex_name[20];    // for logging
		mutex * next_mutex;       // queue of all mutexes, for logging
		int wait_count;           // counts # wait calls;
#endif

		friend class RTOS;
	};


	//************************************************************************
	//
	// class mailbox_base
	//
	//************************************************************************

public:
	class mailbox_base {
	public:
		mailbox_base(const char * name);

		~mailbox_base(void) {
			rtos_fatal("mailbox destructor called");
		}

		void print(hwcpp::io::ostream & s, bool header = true) const;

		task * writer;
		task * reader;

#if RTOS_STATISTICS_ENABLED
		char mailbox_name[20];
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
	//************************************************************************

public:
	template <class T> class mailbox : mailbox_base {
	public:

		mailbox(const char * name = "") : mailbox_base(name) { }

		void write(const T item) {
			RTOS_STATISTICS(writes++; )
				data = item;
			if (reader != NULL) {
				task * tmp = reader;
				reader = NULL;
				// someone is waiting to read, unblock it
				tmp->unblock();
			}
			else {
				if (writer != NULL)
					rtos_fatal("second writer for mailbox");

				// block until the reader gets the data
				writer = RTOS::current_task();
				writer->block();
			}
		}

		T read(void) {
			if (writer != NULL) {
				// unblock the writer
				task * tmp = writer;
				writer = NULL;
				tmp->unblock();
			}
			else {
				if (reader != NULL)
					rtos_fatal("second reader for mailbox");

				// no writer yet, so wait for a writer
				reader = RTOS::current_task();
				reader->block();
			}
			RTOS_STATISTICS(reads++; )
				return data;
		}

	private:
		T data;
	};

	//************************************************************************
	//
	// class channel_base
	//
	//************************************************************************

public:
	class channel_base : public waitable {
	public:
		~channel_base(void) {
			rtos_fatal("channel destructor called");
		}

		void print(hwcpp::io::ostream & s, bool header = true) const;

	protected:
		channel_base(task * t, const char * name);

#if RTOS_STATISTICS_ENABLED
		char channel_name[20];
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
	//************************************************************************

public:
	template <class T, const int SIZE> class channel : public channel_base {
	public:

		channel(task * t, const char * name = "") : channel_base(t, name) { }

		//! write an item to the queue
		void write(T item) {
			if (qSize < SIZE) {
				RTOS_STATISTICS(writes++; )
					queue[head] = item;
				if (++head == SIZE) {
					head = 0;
				}
				qSize += 1;
				waitable::set();
			}
			else {
				RTOS_STATISTICS(ignores++; )
			}
		}

		//! read an item from the queue
		T read(void) {
			if (qSize == 0) {
				t->wait(*this);
			}

			if (qSize == 0) {
				rtos_fatal("channel should not be empty now");
			}

			T buf = queue[tail];
			if (++tail == SIZE) {
				tail = 0;
			}
			qSize -= 1;
			if (qSize > 0) {
				waitable::set();
			}
			else {
				waitable::clear();
			}
			return buf;
		}

		void clear(void) {
			qSize = 0;
			head = tail = 0;
			waitable::clear();
		}
	private:
		T queue[SIZE];
	};

};

std::ostream & operator<< (std::ostream & s, const RTOS::task & t);
std::ostream & operator<< (std::ostream & s, const RTOS::flag & f);
std::ostream & operator<< (std::ostream & s, const RTOS::event & e);
std::ostream & operator<< (std::ostream & s, const RTOS::timer & t);
std::ostream & operator<< (std::ostream & s, const RTOS::clock & c);
std::ostream & operator<< (std::ostream & s, const RTOS::channel_base & cb);
std::ostream & operator<< (std::ostream & s, const RTOS::mutex & m);
std::ostream & operator<< (std::ostream & s, const RTOS::mailbox_base & mb);
std::ostream & operator<< (std::ostream & s, const RTOS::pool_base & pb);

#endif
