#include <iostream>
#include <limits>
#include <climits>
#include <cstdlib>
#include <sstream>

int getUserInt(string message, int min, int max) {

  int value = INT_MIN;
  while (value < min || value > max) {
    cout << message + ":\n";
    cin >> value;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Not a valid integer\n";
    }
  }

  // clear iostream
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  return value;
}

