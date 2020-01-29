#include <iostream>
#include <string>
#include <vector>  // can be used as a stack/LIFO
using namespace std;
int main() {
  while (true) {
    printf("Type a sentence (then hit return): ");
    string line;
    getline(cin, line);
    if (!cin.good()) {
      printf("Done\n");
      return 0;
    }

    unsigned index = 0;
    vector<char> stack;
    while (true) {
      // fill the stack
      unsigned i;
      for (i = index; i < line.size() && line[i] != ' '; i++) {
        stack.push_back(line[i]);
      }

      // dump the stack
      while (stack.size() > 0) {
        printf("%c", stack.back());
        stack.pop_back();
      }
      printf(" ");

      // advance; maybe move to the next line
      index = 1 + i;
      if (index >= line.size()) {
        printf("\n");
        break;
      }
    }
  }
}

// a session looks like this:
//
// Type a sentence (then hit return): this is the truth
// siht si eht hturt
// Type a sentence (then hit return): that is false
// taht si eslaf
// Type a sentence (then hit return): enough of this nonsense!
// hguone fo siht !esnesnon
// Type a sentence (then hit return): Done
