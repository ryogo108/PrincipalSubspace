#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <gmpxx.h>
#include <list>
#include "split.h"

using std::cout;    using std::endl;
using std::cin;     using std::list;
using std::map;     using std::string;
using std::stoi;    using std::vector;
using std::pair;    using std::ostream;

using QQ = mpq_class;
using Q = int;
using H = int;
using monomial = pair<list<H>, Q>;
using L = map<monomial, QQ>;

vector<int> stoi(const vector<string>& strs);
ostream& operator << (ostream& os, const H& x);
ostream& operator << (ostream& os, const list<H>& l);
ostream& operator << (ostream& os, const monomial& m);
ostream& operator << (ostream& os, const L& v);

string Xs_str(const vector<int>& in, string v);
L Xs(const vector<int>& in, const L& v);

int main()
{
  L v;
  v[L::key_type(list<H>(), 0)] = 1;
  cout << "v = " << v << endl;
  string str;
  while(getline(cin, str)) {
    vector<string> strs = split(str);
    vector<int> in = stoi(strs);
    L result = Xs(in, v);
    cout << Xs_str(in, "v") << "=" << endl
         << result << endl;
  }
}
