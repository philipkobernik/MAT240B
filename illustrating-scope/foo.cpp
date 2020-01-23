#include <iostream>
using namespace std;

struct Foo {
  Foo() {
    //
    cout << "Foo(" << this << ") called" << endl;
  }
  ~Foo() {
    //
    cout << "~Foo(" << this << ") called" << endl;
  }
};

int main() {
  //
  Foo foo;
  { Foo foo; }
}
