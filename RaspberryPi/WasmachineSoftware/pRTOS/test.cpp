/******************************************************************************\
 FILE:   main.cpp

 This file contains several programs to test the RTOS package.

\******************************************************************************/

#include "pRTOS.h"
#include <iostream>

using namespace std;

/******************************************************************************\
 IdleTask class
\******************************************************************************/
class IdleTask : public RTOS::task {
public:
   IdleTask (void) : task (1000, "IdleTask"), n (0) { }

private:
   void main (void) {
        for (;;) {
            if (n % 10000 == 0) {
                cout << "i: " << (n / 10000) << endl;
            }
            n += 1;
            sleep(2500 US);
        }
   }

   int n;
};

/******************************************************************************\
 PoolTask class
\******************************************************************************/
class PoolTask : public RTOS::task {
public:
    PoolTask (short prio, const char * name, RTOS::pool<int> * p, int i) :
        RTOS::task (prio, name),
        pp (p),
        id (i),
        n (0)
    {
        pp->write (0);
    }

private:

    void main (void) {
        for (;;) {
            if (id == 1) {
                cout << "pw: " << n << endl;
                pp->write (n++);
            }
            else {
                n = pp->read();
                cout << "pr: " << n << endl;
            }
            sleep (id == 1 ? 500 MS : 1000 MS);
        }
    }

private:
   RTOS::pool<int> * pp;
   int id;
   int n;
};

/******************************************************************************\
 Pool test
\******************************************************************************/
void poolTest (void) {

   RTOS::pool<int> pool ("int pool");

   PoolTask p0 (1, "PoolTask 1", &pool, 1);
   PoolTask p1 (2, "PoolTask 2", &pool, 2);
   IdleTask it;

   RTOS::run();
}


/******************************************************************************\
 MutexTask class
\******************************************************************************/
class MutexTask : public RTOS::task {
public:
   MutexTask (short prio, const char * name, RTOS::mutex * _mut, int i) :
      RTOS::task (prio, name),
      id (i),
      pm (_mut),
      tm (this, "mutex timer")
   {
   }

private:

    void main (void) {
        for (;;) {
            pm->wait();
            if (id == 1) {
                cout << "m1: " << ++n << endl;
                tm.set (500 MS);
            }
            else {
                cout << "m2: " << n << endl;
                tm.set (750 MS);
            }
            pm->signal();
            wait (tm);
        }
    }

private:
    static int n;

    int id;
    RTOS::mutex * pm;
    RTOS::timer   tm;
};

int MutexTask::n = 0;

/******************************************************************************\
 Mutex test
\******************************************************************************/
void mutexTest (void) {

   RTOS::mutex mut ("mutex");

   MutexTask mt0 (1, "MutexTask 1", &mut, 1);
   MutexTask mt1 (2, "MutexTask 2", &mut, 2);
   IdleTask it;

   RTOS::run();
}


/******************************************************************************\
 FlagTask class
\******************************************************************************/
class FlagTask : public RTOS::task {
public:
   FlagTask (short prio, const char * name, int i) :
      RTOS::task (prio, name),
      id (i),
      fl (this, "my flag")
   {
   }

    void setTask (FlagTask * ft) {
        clientTask = ft;
    }

    void signal (void) {
        fl.set();
    }

private:

    void main (void) {
        for (;;) {
            clientTask->signal();
            wait(fl);
            if (id == 1) {
                cout << "f1: " << ++n << endl;
                sleep (500 MS);
            }
            else {
                cout << "f2: " << n << endl;
                sleep (1000 MS);
            }
        }
    }

private:
    static int n;

    int id;
    RTOS::flag  fl;
    FlagTask * clientTask;
};

int FlagTask::n = 0;

/******************************************************************************\
 Flag test
\******************************************************************************/
void flagTest (void) {

   FlagTask ft0 (1, "FlagTask 1", 1);
   FlagTask ft1 (2, "FlagTask 2", 2);
   ft0.setTask (&ft1);
   ft1.setTask (&ft0);
   IdleTask it;

   RTOS::run();
}

/******************************************************************************\
 MailboxTask class
\******************************************************************************/
class MailboxTask : public RTOS::task  {
public:
    MailboxTask (short prio, const char * name, RTOS::mailbox<int> * mb, int i) :
        RTOS::task (prio, name),
        pmb (mb),
        id (i),
        n (0)
    {
    }

private:

    void main (void) {
        for (;;) {
            if (id == 1) {
                n = pmb->read();
                cout << "mr: " << n << endl;
                sleep ((n & 1) == 0 ? 1000 MS : 500 MS);
            }
            else {
                cout << "mw: " << n << endl;
                pmb->write (n++);
                sleep ((n & 1) == 0 ? 1500 MS : 1000 MS);
            }
        }
    }

    RTOS::mailbox<int> * pmb;
    int id;
    int n;
};

/******************************************************************************\
 Mailbox test
\******************************************************************************/
void mailboxTest (void) {

   RTOS::mailbox<int> mb("int box");

   MailboxTask mbt0 (1, "MailboxTask 1", &mb, 1);
   MailboxTask mbt1 (2, "MailboxTask 2", &mb, 2);
   IdleTask it;

   RTOS::run();
}

/******************************************************************************\
 ChannelTask classes
\******************************************************************************/
class ChannelReader : public RTOS::task {
public:
    ChannelReader (short prio, const char * name) :
        RTOS::task (prio, name),
        ch (this, "int channel"),
        n (0)
    {
    }

    void writeInt (int n) {
        ch.write (n);
    }

private:

    void main (void) {
        for (;;) {
            n = ch.read();
            cout << "cr: " << n << endl;
        }
    }

    RTOS::channel<int, 4> ch;
    int n;
};

class ChannelWriter : public RTOS::task {
public:
    ChannelWriter (short prio, const char * name, ChannelReader * cr, int i) :
        RTOS::task (prio, name),
        chTask (cr),
        id (i),
        n (0)
    {
    }

private:

    void main (void) {
        for (;;) {
            if (id == 1) {
                cout << "cw1: " << n << endl;
                chTask->writeInt (n++);
                sleep ((n & 1) == 0 ? 500 MS : 250 MS);
            }
            else {
                cout << "cw2: " << n << endl;
                chTask->writeInt (n--);
                sleep ((n & 1) == 1 ? 750 MS : 1500 MS);
            }
        }
    }

    ChannelReader * chTask;
    int id;
    int n;
};

/******************************************************************************\
 Channel test
\******************************************************************************/
void channelTest (void) {

   ChannelReader cr  (3, "ChannelReader");
   ChannelWriter cw1 (2, "ChannelWriter 1", &cr, 1);
   ChannelWriter cw2 (1, "ChannelWriter 2", &cr, 2);
   IdleTask it;

   RTOS::run();
}


/******************************************************************************\
Clock test
\******************************************************************************/
class ClockTask : public RTOS::task {
public:
    ClockTask (short prio) :
        RTOS::task (prio, "ClockTask"),
        clk ( this, 1 S, "my clock" )
    { }

    void main ( void ) {
        int m = 0, s = 0;
        cout << endl << setfill ('0') << setw(2) << m << ':' << setw(2) << s
                     << setfill (' ') << endl;

        for( ; ; ) {
            wait( clk );
            s++;
            if( s >= 60 ) { s = 0; m++; }
            if( m >= 60 ) { m = 0; }
            cout << setfill ('0') << setw(2) << m << ':' << setw(2) << s
                 << setfill (' ') << endl;
        }
   }

private:
   RTOS::clock clk;
};

void clockTest (void) {
   ClockTask t(0);

   RTOS::run();
}


/******************************************************************************\
 main routine
\******************************************************************************/
int main( void )
{
   // Select one of the tests to run.
   //clockTest();
   //poolTest();
   //mutexTest();
   //flagTest();
   //mailboxTest();
   channelTest();

   return 0;
}
