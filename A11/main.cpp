#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <gmpxx.h>
#include <list>
#include <algorithm>
#include "split.h"
#include "partition.h"
#include "calc_z.h"

using std::cout;      using std::endl;
using std::cin;       using std::list;
using std::map;       using std::string;
using std::stoi;      using std::vector;
using std::pair;      using std::ostream;
using std::to_string; using std::max;
using partition::Par; using partition::generate;
using std::count;

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

L& omit(L& v)
{
  for(L::const_iterator iter = v.begin();
    iter != v.end();) {
    if(iter -> second == 0)
      iter = v.erase(iter);
    else
      ++iter;
  }
  return v;
}

L& add(L& v, const L& vr)
{
  L& ret = v;
  for(L::const_iterator iter = vr.begin();
    iter != vr.end(); ++iter) {
    L::iterator it = ret.find(iter -> first);
    if(it != ret.end()) {
      it -> second += iter -> second;
    }
    else
      ret.insert(*iter);
  }
  return omit(ret);
}

list<H> append(const H& x, list<H> l)
{
  l.push_front(x);
  l.sort();
  return l;
}

L append(const H& x, const L& v)
{
  L ret;
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter) {
    const monomial& m = iter -> first;
    ret.insert(
      make_pair(
        monomial(append(x, m.first), m.second),   // monomial part
        iter -> second                            // scalar part
      )
    );
  }
  return ret;
}

L append(const Par& p, const pair<monomial, QQ>& v)
{
  L ret;
  ret.insert(v);
  for(Par::const_iterator it = p.begin();
    it != p.end(); ++it) {
    ret = append(H(-(*it)), ret);
  }
  return ret;
}

L operator * (const QQ& a, const L& v)
{
  L ret = v;
  for(L::iterator iter = ret.begin();
    iter != ret.end(); ++iter)
    iter -> second *= a;
  return ret;
}

// Calculate the action of E_minus of degree k on a given monomial.
L E_minus(int k, const pair<monomial, QQ>& v)
{
  if(k < 0) return L();
  L ret;
  vector<Par> pars;
  // Generate all partitions of the given integer k.
  generate(k, pars);
  for(vector<Par>::const_iterator it = pars.begin();
    it != pars.end(); ++it) {
    ret = add(ret,
              (QQ(1) / QQ(z(*it))) *
              (append(*it, v))
             );
  }
  return omit(ret);
}

L E_minus(int k, const L& v)
{
  if(k < 0) return L();
  L ret;
  // Calculate the action of E_minus of degree k on each monomial in v.
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter)
    ret = add(ret, E_minus(k, *iter));
  return omit(ret);
}

L d(const H& x, const pair<monomial, QQ>& v)
{
  L ret;
  const monomial& m = v.first;
  list<H> l = m.first;
  list<H>::iterator it = find(l.begin(), l.end(), H(-int(x)));
  if(it == l.end())
    // The derivation x acts as zero on v.
    return L();
  // Count multiplicity of -x in the given monomial m.
  const unsigned int mul = count(l.begin(), l.end(), H(-int(x)));
  // Calculate the derivation x on v.
  l.erase(it);
  ret[monomial(l, m.second)] = v.second * 2 * mul * int(x);
  return ret;
}

L d(const H& x, const L& v)
{
  L ret;
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter) {
    ret = add(ret, d(x, *iter));
  }
  return omit(ret);
}

L d(const Par& p, const pair<monomial, QQ>& v)
{
  L ret;
  ret.insert(v);
  for(Par::const_iterator it = p.begin();
    it != p.end(); ++it) {
    ret = d(H(*it), ret);
  }
  return omit(ret);
}

L E_plus(int k, const pair<monomial, QQ>& v)
{
  if(k > 0) return L();
  L ret;
  vector<Par> pars;
  generate(-k, pars);
  for(vector<Par>::const_iterator it = pars.begin();
    it != pars.end(); ++it) {
    ret = add(ret,
              QQ(partition::l(*it) % 2 == 0 ? 1 : -1) *
              (QQ(1) / QQ(z(*it))) *
              (d(*it, v))
             );
  }
  return omit(ret);
}

L E_plus(int k, const L& v)
{
  if(k > 0) return L();
  L ret;
  // Calculate the action of E_plus of degree k on each monomial in v.
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter)
    ret = add(ret, E_plus(k, *iter));
  return omit(ret);
}

// todo : Try L -> L& for return value type
//        and const L& -> L& for argument value type for acceleration.
L e(const L& v)
{
  L ret;
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter) {
    L::key_type target = iter -> first;
    // Act e ^ \alpha on target monomial.
    target.second = target.second + 1;
    ret[target] = iter -> second;
  }
  return ret;
}

// Calculate the following non negative integer N of a given monomial
// such as N = n_1 + n_2 - ... + n_l w.r.t a monomial of shape
// [-n_1, -n_2, ..., -n_l] \tensor e ^ m \alpha for some integer m.
int sum(const monomial& m)
{
  int ret = 0;
  list<H> l = m.first;
  for(list<H>::const_iterator iter = l.begin();
    iter != l.end(); ++iter) {
    ret += -(*iter);
  }
  return ret;
}

int max(const L& v)
{
  int ret = 0;
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter) {
    ret = max(ret, sum(iter -> first));
  }
  return ret;
}

L X(int k, const L& v)
{
  L ret;
  // Calculate action of the coefficient of degree -k - 1 of X on each monomial in v.
  // Note that X is the generating function in indeterminate \zeta
  // which consists of E_\minus(\zeta) * E_\plus(\zeta) * e ^ \alpha * \zeta ^ \alpha.
  for(L::const_iterator iter = v.begin();
    iter != v.end(); ++iter) {
    const monomial& m = iter -> first;
    L target;
    target.insert(*iter);
    // Calculate the degree of which we take the coefficient in the generating function
    // of operators E ^ \minus * E ^ \plus.
    // Remark : This degree depends on second part of target monomial, which means a element
    //          e ^ n \alpha (for some integer n) of the group algebra CC[Q].
    int deg = -k - 1 - 2 * m.second;
    // Act e ^ \alpha on the target vector.
    target = e(target);
    // Act the degree def coefficient of E_\minus * E_\plus on target vector.
    // Remark : There is some integer N ( = max(target)) such that
    //          an operator in E_\plus acts as zero on target vector if its degree is greater
    //          than N.
    const int N = max(target);
    for(int i = 0; i <= N; i++) {
      ret = add(ret, E_minus(deg + i, E_plus(-i, target)));
    }
  }
  return ret;
}

string Xs_str(const vector<int>& in, string v)
{
  if(in.empty()) return v;
  string ret = "[";
  for(vector<int>::const_iterator iter = in.begin();
    iter != in.end(); ++iter) {
    if(iter != in.begin()) ret += ", ";
    ret += to_string(*iter);
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
