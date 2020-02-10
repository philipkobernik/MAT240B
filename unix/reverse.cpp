#include <iostream>
#include <string>
#include <vector>
using namespace std;

// reverse a sound clip
//
// - takes lines of floats
// - gives lines of floats
//
// use it like this:
//
//   ./read.exe something.wav | ./reverse.exe | ./write.exe reversed.wav
//
//

int main() {
  // read in a sequence of lines where each line is a human readable floating
  // point number.
  //
  string line;
  getline(cin, line);
  vector<float> sample;
  while (line != "") {
    float f = stof(line);
    getline(cin, line);
    sample.push_back(f);
  }

  // write a sequence of lines where each line is a human readable floating
  // point number.
  //
  while (sample.size() > 0) {
    printf("%f\n", sample.back());
    sample.pop_back();
  }
}
