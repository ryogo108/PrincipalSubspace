#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <gmpxx.h>
#include <list>
#include <algorithm>
#include "split.h"

using std::cout;      using std::endl;
using std::cin;       using std::list;
using std::map;       using std::string;
using std::stoi;      using std::vector;
using std::pair;      using std::ostream;
using std::to_string;

using QQ = mpq_class;
using Q = int;
using H = int;
using monomial = pair<list<H>, Q>;
using L = map<monomial, QQ>;

vector<int> stoi(const vector<string>& strs)
{
  vector<int> ret;
  transform(strs.begin(), strs.end(), back_inserter(ret),
            [](string s){return stoi(s);});
  return ret;
}

ostream& operator << (ostream& os, const list<H>& l) 
{
  os << "[";
  for(list<H>::const_iterator iter = l.begin();
    iter != l.end(); ++iter) {
    if(iter != l.begin()) os << ", ";
    os << (*iter);
  }
  return os << "]";
}

ostream& operator << (ostream& os, const monomial& m)
{
  return os << "[" << m.first << ", " << m.second << "]";
}

ostream& operator << (ostream& os, const L& v)
{
  os << "[";
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter) {
    if(iter != v.begin()) os << ", ";
    os << "["
       << iter -> first << ", "
       << iter -> second
       << "]";
  }
  return os << "]";
}

L X(int k, const L& v);

string Xs_str(const vector<int>& in, string v)
{
  if(in.empty()) return v;
  string ret = "[";
  for(vector<int>::const_iterator iter = in.begin();
    iter != in.end(); ++iter) {
    if(iter != in.begin()) ret += ", ";
    ret += "(" + to_string(*iter) + ", 1)";
  }
  return ret + "] * " + v;
}

L Xs(const vector<int>& in, const L& v)
{
  L ret = v;
  for(vector<int>::const_reverse_iterator iter = in.rbegin();
    iter != in.rend(); ++iter) {
    ret = X(*iter, ret);
  }
  return ret;
}

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
  return 0;
}
