
#include "io_utils.h"

//MENU!
int main(int argc, char *argv[]){
  cout<< "Welcome to the four way stop simulator!" << endl;
  
  int sourceCode = getUserInt("Type 1 for c++ and type 2 for java code", 1, 2);

  
  int number_of_cars = getUserInt("Enter number of cars", 1, 400000000);

  ostringstream os;
  system("make");
  
  switch (sourceCode){
    case 1:
      //Run c++ code
      os << "./cpp_sync " << number_of_cars << '\0';
      system((os.str()).c_str());
      break;
    case 2:
      //Run java code
      os << "java java_sync " << number_of_cars << '\0';
      system((os.str()).c_str());
      break;
  }

}
