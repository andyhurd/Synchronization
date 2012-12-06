using namespace std;
#include <iostream>
#include <sstream>
#include <semaphore.h>
#include <pthread.h>
#include <vector>
#include <queue>
#include <unistd.h>
#include "io_utils.h"

#define QUEUE_NUM 4                             // number of incoming steets to intersection

queue<pthread_t *> *street_queues[QUEUE_NUM];   // queues of waiting car threads
int streetIDs[QUEUE_NUM];                       // integers representing the queues themselves
queue<int> *carIDs[QUEUE_NUM];                  // to pull carIDs for output
char streetCompass[] = {'N', 'E', 'S', 'W'};    // pull street leaving for output
bool empty = true;                              // indicates no car in intersection
sem_t mutex;                                    // mutual exclusion for checking empty intersection
sem_t street_permits[QUEUE_NUM];                // semphores to tell a car waiting in particular queue to drive
vector<pthread_t *> *cars;

void drive (int *);
void *arrival(void *);
double rndom();

// a main function woot
int main (int argc, char *argv[]) {

  int num_cars;
  bool invalid_run = false;

  // validate args
  if (argc < 2) {
    invalid_run = true;
  } else {
    stringstream ss;
    ss.clear();
    ss.str(argv[1]);
    ss >> num_cars;
    
    if (ss.fail()) {
        invalid_run = true;
      }
  }

  // print out error message if invalid args
  if (invalid_run) {
    cout << "Usage: " << argv[0] << " <Number of Cars>" << endl;
    exit(1);
  }

  // initialize car vector
  cars = new vector<pthread_t *>();

  // initialize all street queues
  for (int i = 0; i < QUEUE_NUM; i++) {
    street_queues[i] = new queue<pthread_t*>();
    carIDs[i] = new queue<int>();
    streetIDs[i] = i;
  }
  
  // assign the cars to queues
  for (int i = 0; i < num_cars; i++) {
    pthread_t *next_car = new pthread_t();
    int next_rand = ((int) (4 * rndom()));
    (*cars).push_back(next_car);
    (*(carIDs[next_rand])).push(i);
    (*(street_queues[next_rand])).push(next_car);
  }
  
  // initialize all street semaphores to zero
  for (int i = 0; i < QUEUE_NUM; i++) {
    sem_init(&street_permits[i], 0, 0);
  }

  // initialize mutex and empty queues semaphores to one and zero, respectively
  sem_init(&mutex, 0, 1);
  
  // let the first car from each street arrive at intersection
  for (int i = 0; i < QUEUE_NUM; i++) {
    if (!(*(street_queues[i])).empty()) {
      pthread_t *next_car = (*(street_queues[i])).front();
      pthread_create(next_car, NULL, &arrival, (void *) &streetIDs[i]);
    }
  }

  // wait for all threads to finish, then release mem
  for (int i = 0; i < (*cars).size(); i++) {
    pthread_join((*((*cars)[i])), NULL);
    delete((*cars)[i]);
  }
  delete(cars);

  // notify user that simulation is done
  cout << "All cars finished." << endl;

  // release memory resources
  for (int i = 0; i < QUEUE_NUM; i++) {
    delete(street_queues[i]);
    delete(carIDs[i]);
    streetIDs[i] = i;
  }
  
}

// car arrives at street *param
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
  // pick the next street to release a car
  int next_queue = *i;
  for (int k = 0; k < QUEUE_NUM; k++) {
    next_queue = (next_queue + 1) % QUEUE_NUM;
    if (!(*(street_queues[next_queue])).empty()) {
      break;
    }
  }

  // indicate next car's turn
  sem_post(&street_permits[next_queue]);

  // if no cars are in queues, empty = true (first assume empty, then disprove)
  empty = true;
  for (int j = 0; j < QUEUE_NUM; j++) {
    if (!(*(street_queues[j])).empty()) {
      empty = false;
    }
  }

  // finish this car thread
  pthread_exit(NULL);
}

// car drives through intersection
void drive (int *i) {

  // leave street queue
  (*(street_queues[*i])).pop();
  int carID = (*(carIDs[*i])).front();
  (*(carIDs[*i])).pop();
  cout << "Car " << carID << " from " << streetCompass[*i] << " street" << endl;

  // initialize  the next car from this street queue
  if (!(*(street_queues[*i])).empty()) {
    pthread_t *next_car = (*(street_queues[*i])).front();
    pthread_create(next_car, NULL, &arrival, (void *) i);
  }

  // simulate driving (two milliseconds)
  usleep(2000);

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
