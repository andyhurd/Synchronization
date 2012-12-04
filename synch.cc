using namespace std;
#include <queue>
#include "io_utils.h"


int street[4];
queue<int> street_queues[4];
bool empty;

void drive (int);

// a main function woot
int main (int argc, char *argv[]) {
  cout << "Haldo doods!\nDo this homework dawgs!\n";
}

// car arrives at street i
void arrival(int i) {
  if (empty) {
    empty = false;
    drive(i);
  } else {
    // so...are these queues or ints....does it matter?
    //street_queues[i]++;

    // wait for someone to signal that it is street[i]'s turn to go
    drive(i);
  }
}

// car leaves the intersection
void departure (int i) {
  // one car departs
  // pick the next intersection for the next car to go
  // if no cars are in queues, empty = true
}

// car drives through intersection
void drive (int i) {
  // one car departs
  // pick the next intersection for the next car to go
  // if no cars are in queues, empty = true
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
