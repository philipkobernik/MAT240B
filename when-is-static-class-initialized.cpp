#include <iostream>  // cout, cin, printf
using namespace std;

struct Hello {
  Hello() { cout << "Hello(" << this << ")" << endl; }
  int meaningOfLife() { return 42; }
};

int func() {
  static Hello hello;
  return hello.meaningOfLife();
}

int main() {
  printf("%p\n", func);
  //  cout << func() << endl;
  //  cout << func() << endl;
  return 0;
}
