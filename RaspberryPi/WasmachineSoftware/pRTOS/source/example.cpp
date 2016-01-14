#include "libfiber.h"
#include <stdio.h>
#include <unistd.h>

fiber_t * fibers[3];

void fiber1(void) {
    printf( "Fiber1: SB=%x  SZ=%d  SU=%d\n", (unsigned int) fibers[0]->stackBase(), fibers[0]->stackSize(), fibers[0]->stackUsed() );

    for ( int i = 0; i <= 15; ++ i )
    {
        printf( "Hey, I'm fiber #1: %d\n", i );
        fibers[1]->resume();
    }
}

void fibonacchi(void) {
    int fib[2] = { 0, 1 };

    printf( "Fibona: SB=%x  SZ=%d  SU=%d\n", (unsigned int) fibers[1]->stackBase(), fibers[1]->stackSize(), fibers[0]->stackUsed() );

    printf( "fibonacchi(0) = 0\nfibonnachi(1) = 1\n" );
    for( int i = 2; i <= 17; ++ i )
    {
        int nextFib = fib[0] + fib[1];
        printf( "fibonacchi(%d) = %d\n", i, nextFib );
        fib[0] = fib[1];
        fib[1] = nextFib;
        fibers[2]->resume();
    }
}

void squares(void) {
    printf( "Square: SB=%x  SZ=%d  SU=%d\n", (unsigned int) fibers[2]->stackBase(), fibers[2]->stackSize(), fibers[0]->stackUsed() );

    for ( int i = 0; i <= 15; ++ i )
    {
        printf( "%d*%d = %d\n", i, i, i*i );
        fibers[0]->resume();
    }
}

int main(void) {
    // Create the fibers
    fibers[0] = new fiber_t (fiber1, 8192);
    fibers[1] = new fiber_t (fibonacchi, 8192);
    fibers[2] = new fiber_t (squares, 8192);

    printf( "Free mem = %ld\n", memory_free() );

    // Go fibers
    fibers[0]->resume();

    printf( "Main terminates\n" );

    /* The program quits */
    return 0;
}
