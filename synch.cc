using namespace std;
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <vector>
#include "io_utils.h"

#define QUEUE_NUM 4                   // number of incoming steets to intersection
#define CARS_NUM 23                   // number of cars

int streets[QUEUE_NUM];               // integers representing the cars waiting in each queue
int streetIDs[QUEUE_NUM];             // integers representing the queues themselves
bool empty = true;                    // indicates no car in intersection
bool all_arrived = false;             // indicates all cars have arrived
sem_t mutex;                          // mutual exclusion for checking empty intersection
sem_t all_queues_empty;               // indicates when to join all threads
sem_t street_permits[QUEUE_NUM];      // semphores to tell a car waiting in particular queue to drive
vector<pthread_t *> *cars = new vector<pthread_t *>();

void drive (int *);
void *arrival(void *);
double rndom();

// a main function woot
int main (int argc, char *argv[]) {

  // initialize all street queue lengths to zero
  for (int i = 0; i < QUEUE_NUM; i++) {
    streets[i] = 0;
    streetIDs[i] = i;
  }
  
  // assign the cars to queues
  for (int i = 0; i < CARS_NUM; i++) {
    streets[((int) (4 * rndom()))]++;
  }
  
  // initialize all street semaphores to zero
  for (int i = 0; i < QUEUE_NUM; i++) {
    sem_init(&street_permits[i], 0, 0);
  }

  // initialize mutex and empty queues semaphores to one and zero, respectively
  sem_init(&mutex, 0, 1);
  sem_init(&all_queues_empty, 0, 0);
  
  // let the first car from each street arrive at intersection
  for (int i = 0; i < QUEUE_NUM; i++) {
    pthread_t next_car;
    (*cars).push_back(&next_car);
    pthread_create(&next_car, NULL, &arrival, (void *) &streetIDs[i]);
  }

  all_arrived = true;
  sem_wait(&all_queues_empty);
  for (int i = 0; i < (*cars).size(); i++) {
    cout << "waiting on " << i << endl;
    pthread_join((*((*cars)[i])), NULL);
  }

  cout << "Problem.solved()\n";
}

// car arrives at street i
void *arrival(void *param) {

  // get queue number
  int *i = (int *) param;

  sem_wait(&mutex);                 // wait to enter critical section
  if (empty) {
    empty = false;
    sem_post(&mutex);               // release critical section
  } else {
    sem_post(&mutex);               // release critical section

    // wait for other car to signal that it's this car's street's turn
    sem_wait(&street_permits[*i]);
  }
  drive(i);
}

// car leaves the intersection
void departure (int *i) {
  // one car departs
  // pick the next intersection for the next car to go
  int next_queue = *i;
  int k;
  for (k = 0; k < QUEUE_NUM; k++) {
    next_queue = (next_queue + 1) % QUEUE_NUM;
    if (streets[next_queue] > 0) {
      break;
    }
  }
  if (all_arrived && k == QUEUE_NUM) {
    sem_post(&all_queues_empty);
  }

  cout << "Releasing: " << next_queue << endl;
  sem_post(&street_permits[next_queue]);


  // if no cars are in queues, empty = true (first assume empty, then disprove)
  empty = true;
  for (int j = 0; j < QUEUE_NUM; j++) {
    if (streets[j] > 0) {
      empty = false;
    }
  }

  pthread_exit(NULL);
}

// car drives through intersection
void drive (int *i) {
  cout << "Crossing: " << *i << endl;
  streets[*i]--;

  // initialize  the next car from this street queue
  if (streets[*i] > 0) {
    pthread_t *next_car = new pthread_t();
    (*cars).push_back(next_car);
    pthread_create(next_car, NULL, &arrival, (void *) i);
  }

  // when done crossing intersection, call departure to finish
  departure(i);
}

/* Returns a (random) number from 0 to 1 */
double rndom() {
  const long A = 48271;
  const long M = 2147483647;
  const long Q = M/A;
  const long R = M%A;

  static long state = 1;
  long t = A * (state % Q) - R * (state / Q);

  if (t > 0) {
    state = t;
  } else {
    state = t + M;
  }

  return ((double) state / M);
}
