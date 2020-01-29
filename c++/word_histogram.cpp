#include <algorithm>      // sort
#include <iostream>       // cin, cout, printf
#include <unordered_map>  // unordered_map
#include <vector>         // vector
using namespace std;
int main() {
  unordered_map<string, unsigned> dictionary;

  string word;
  while (cin >> word) {
    auto search = dictionary.find(word);
    if (search == dictionary.end()) {
      dictionary[word] = 1;
    } else {
      dictionary[word] += 1;
    }

    /*
    // an alternative
    if (dictionary.count(word))
      ++dictionary[word];
    else
      dictionary.insert({word, 1});
    */
  }

  vector<pair<string, unsigned>> wordList;
  for (auto& t : dictionary) wordList.push_back(t);

  sort(wordList.begin(), wordList.end(),
       [](const pair<string, unsigned>& a, const pair<string, unsigned>& b) {
         return a.second > b.second;
       });

  for (auto& t : wordList) printf("%u:%s\n", t.second, t.first.c_str());
}
