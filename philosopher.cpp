/**
 * @author Professor Erika Parsons
 * Editied by Timothy Caole 2/19/2024
 * Referenced used:https://docs.oracle.com/cd/E19455-01/806-5257/6je9h032r/index.html
 */

#include <pthread.h> // pthread
#include <stdio.h>
#include <stdlib.h>   // atoi( )
#include <unistd.h>   // sleep( )
#include <sys/time.h> // gettimeofday
#include <iostream>   // cout

#define PHILOSOPHERS 5
#define MEALS 3

using namespace std;

/**
 * Hint: Use Figure 7.7 in the book to help assist in the implementation of Class Table2
 * Class Table2 represets a dining table with synchronization for multiple philosophers
 */
class Table2 {
public:
    /**
     * Constructor, initalize the dining table with a specified number  of philosopers
     */
  Table2( ) {
    // initialize by yourself
    for (int i = 0; i < PHILOSOPHERS; i++){ // goes through all the philosopers
        state[i] = THINKING; // Inital state, initalize state of philosoper(i) to thinking
        pthread_cond_init(&self[i], NULL); // initalize contion variable for philosoper
    }
      pthread_mutex_init(&lock, NULL); // initalize mutex lock for dining table
  } // end of table2

  /**
   * Method is used for the philosopers picking up the chopsticks
   * @param i  which is the index of philosopers
   */
  void pickup( int i ) {
    // implement by yourself by referring to the textbook.
      pthread_mutex_lock(&lock);  // get mute lock to synchronized access to share resources
    state[i] = HUNGRY; // set state of philosoper is set to hungry
      test(i); // check if philosoper can start eating

      while (state[i] != EATING){
          pthread_cond_wait(&self[i], &lock); //
      }
      pthread_mutex_unlock(&lock); // release the mutex locks when the philosoper is eating
    cout << "philosopher[" << i << "] picked up chopsticks" << endl;
  } // end of pickup method

    /**
    * Method is used for the philosopers putting down the chopsticks
    * @param i  which is the index of philosopers
    */
  void putdown( int i ) {
    // implement by yourself by referring to the textbook.
    pthread_mutex_init(&lock, NULL); // initalize mutex lock
    state[i] = THINKING; // set state of philosoper is set to thinking
    test((i+4)%5); // check if the left neightboring philosoper can start eating
    test((i+1)%5); // check if the right neightboring philosoper can start eating
    pthread_mutex_unlock(&lock); // release mutex lock
      cout << "philosopher[" << i << "] put down chopsticks" << endl;
  } // end of putdown Method

private:
  enum { THINKING, HUNGRY, EATING } state[PHILOSOPHERS];
  pthread_mutex_t lock;
  pthread_cond_t self[PHILOSOPHERS];

  /**
   * Helper method to check if the philosoper can start eating
   * @param i  which is the index of philosopers
   */
  void test( int i ) {
    // implement by yourself
    // if left neightboring philosoper is not eating, current philosoper is hungry, and right neightboring philosoper is not eating
      if ((state[(i+4)%5] != EATING) && (state[i] == HUNGRY) && (state[(i+1)%5] != EATING) ) {
          state[i] = EATING; // set state of philosoper is set to eating
          pthread_cond_signal(&self[i]); // signal conditon variable for current philosoper to wake up
      }
  } // end of test Method
}; // end of Class Table1

/**
 * Class Table1 represets a dining table with a simple mutex lock for synchronization
 */
class Table1 {
public:

    /**
     * Constructor to initalize the dining table with mutex lock
     */
  Table1( ) {
    // initialize the mutex lock
    pthread_mutex_init(&lock, NULL);
  } // end of table1

    /**
    * Method is used for the philosopers picking up the chopsticks
    * @param i  which is the index of philosopers
    */
  void pickup( int i ) {
    // lock by yourself
      pthread_mutex_lock(&lock);
      cout << "philosopher[" << i << "] picked up chopsticks" << endl;
  } // end of Pickup Method


    /**
      * Method is used for the philosopers putting down the chopsticks
      * @param i  which is the index of philosopers
      */
  void putdown( int i ) {
    cout << "philosopher[" << i << "] put down chopsticks" << endl;
    // unlock by yourself
      pthread_mutex_unlock(&lock);
  }

private:
    // define a mutex lock
  pthread_mutex_t lock;
}; // end of Class Table1

class Table0 {
public:
  void pickup( int i ) {
    cout << "philosopher[" << i << "] picked up chopsticks" << endl;
  }

  void putdown( int i ) {
    cout << "philosopher[" << i << "] put down chopsticks" << endl;
  }
};

static Table2 table2;
static Table1 table1;
static Table0 table0;

static int table_id = 0;

void *philosopher( void *arg ) {
  int id = *(int *)arg;
  
  for ( int i = 0; i < MEALS; i++ ) {
    switch( table_id ) {
    case 0:
      table0.pickup( id );
      sleep( 1 );
      table0.putdown( id );
      break;
    case 1:
      table1.pickup( id );
      sleep( 1 );
      table1.putdown( id );
      break;
    case 2:
      table2.pickup( id );
      sleep( 1 );
      table2.putdown( id );
    break;
    }
  }
  return NULL;
}

int main( int argc, char** argv ) {
  pthread_t threads[PHILOSOPHERS];
  pthread_attr_t attr;
  int id[PHILOSOPHERS];
  table_id = atoi( argv[1] );

  pthread_attr_init(&attr);
  
  struct timeval start_time, end_time;
  gettimeofday( &start_time, NULL );
  for ( int i = 0; i < PHILOSOPHERS; i++ ) {
    id[i] = i;
    pthread_create( &threads[i], &attr, philosopher, (void *)&id[i] );
  }

  for ( int i = 0; i < PHILOSOPHERS; i++ )
    pthread_join( threads[i], NULL );
  gettimeofday( &end_time, NULL );

  sleep( 1 );
  cout << "time = " << ( end_time.tv_sec - start_time.tv_sec ) * 1000000 + ( end_time.tv_usec - start_time.tv_usec ) << endl;

  return 0;
}
