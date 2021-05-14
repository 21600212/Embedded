#include <iostream>

using namespace std;

typedef enum {
    UNKNOWN,
    NEC,
    NEC_REPEAT,
    AEHA,
    AEHA_REPEAT,
    SONY
} Format;

int main(){
  Format format = NEC;
  cout << "Format:" << format << endl;
  return 0;
}
