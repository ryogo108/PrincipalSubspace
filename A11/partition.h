#ifndef GUARD_partition_h
#define GUARD_partition_h
#include <vector>
#include <iostream>

namespace partition
{
  typedef unsigned short part;
  typedef unsigned short mult;
  
  /* p[0] >= p[1] >= ... >= p[l(p) - 1] > 0*/
  typedef std::vector<part> Par;
  
  /* m[i] = #{j | p[j] = i}.*/
  typedef std::vector<part> Mul;
  
  void printMul(const Mul & m);
  
  int l(const Par & p);
  
  void printPars(const std::vector<Par> &);
  int generate(int, std::vector<Par> &);
  
  Mul Par2Mul(const Par & p);
  
  std::ostream& operator << (std::ostream&, const Par&);
}

#endif
