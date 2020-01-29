#include <iostream>
#include <regex>  // regex
using namespace std;

int main() {
  while (true) {
    if (!cin.good()) break;
    string line;
    getline(cin, line);

    std::regex href("<a href=\"(.+?)\".*?>(.+?)</a>");
    std::sregex_iterator next(line.begin(), line.end(), href);
    std::sregex_iterator end;
    while (next != end) {
      std::smatch match = *next;
      std::cout << match[2] << " --> " << match[1] << endl;
      next++;
    }
  }
}
