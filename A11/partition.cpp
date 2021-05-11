#include <iostream>
#include <vector>
#include <queue>

using std::ostream;

namespace partition
{
  typedef unsigned short part;
  typedef unsigned short mult;

  /* p[0] >= p[1] >= ... >= p[l(p) - 1] > 0*/
  typedef std::vector<part> Par;

  /* m[i] = #{j | p[j] = i}.*/
  typedef std::vector<part> Mul;

  ostream& operator << (ostream& os, const Par& p)
  {
    os << "[";
    for(Par::const_iterator iter = p.begin();
        iter != p.end(); ++iter) {
      if(iter != p.begin()) os << ", ";
      os << (*iter);
    }
    return os << "]";
  }

  template<class T> T sum(const std::vector<T> & vec) {
    T sum = T(0);
    for(int i = 0; i < vec.size(); i++){
      sum += vec[i];
    }
    return sum;
  }

  template<class T> void print(const std::vector<T> & vec) {
    std::cout << '[';
    for(int i = 0; i < vec.size(); i++){
      if(i) std::cout << ", ";
      std::cout << vec[i];
    }
    std::cout << ']' << std::endl;
  }

  void printPar(const Par & p) {
    std::cout << "Par";
    print<part>(p);
  }

  void printPars(const std::vector<Par> & partitions){
    for(int i = 0; i < partitions.size(); i++){
      printPar(partitions[i]);
    }
  }

  void printMul(const Mul & m) {
    std::cout << "Mul";
    print<mult>(m);
  }

  part sum(const Par & p) {
    return sum<part>(p);
  }

  int l(const Par & p) {
    return p.size();
  }

  part max(const Par & p) {
    return p[0];
  }

  part min(const Par & p) {
    return p[l(p) - 1];
  }

  Par append(const Par & p, part head) {
    Par new_p = p;
    new_p.push_back(head);
    return new_p;
  }

  Mul Par2Mul(const Par & p) {
    if(p.empty()){
      return Mul({});
    }
    Mul m(max(p) + 1);
    for(int i = 0; i < l(p); i++) {
      m[p[i]]++;
    }
    return m;
  }

  int generate(int n, std::vector<Par> & dest) {
    if(n == 0) {
      dest.push_back(Par({}));
      return 1;
    }
    std::queue<Par> que;

    for(part head = n; head >= 1; head--) {
      que.push(Par(1, head));
    }

    while(!que.empty()) {
      Par p = que.front();
      que.pop();

      if(sum(p) == n) {
        dest.push_back(p);
      }
      else {
        for(part head = min(p); head >= 1; head--) {
          Par new_p = append(p, head);
          if(sum(new_p) <= n){
            que.push(new_p);
          }
        }
      }
    }
    return 1;
  }
}

/*
int main(){
  std::vector<Par> partitions;
  generate(0, partitions);
  printPars(partitions);
  Par p = partitions[0];
  Mul m = Par2Mul(p);
  std::cout << sum(p) << std::endl;
  std::cout << l(p) << std::endl;
  printMul(m);
  return 0;
}
*/
