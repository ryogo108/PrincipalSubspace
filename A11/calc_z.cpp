#include <gmpxx.h>
#include "partition.h"

using namespace partition;

using ZZ = mpz_class;
using QQ = mpq_class;

ZZ factorial(int n){
  if(n < 0) return ZZ(0);
  if(n == 0) return ZZ(1);
  return ZZ(n) * factorial(n - 1);
}

QQ pow(QQ a, int n){
  if(n < 0) return QQ(1) / pow(a, -n);
  if(n == 0) return QQ(1);
  return a * pow(a, n - 1);
}

ZZ z(Par p){
  Mul m = Par2Mul(p);
  ZZ val = ZZ(1);
  for(Mul::size_type i = 1; i < m.size(); i++){
    val *= pow(i, m[i]) * factorial(m[i]);
  }
  return val;
}

QQ inverse_sum_z_over_partitions(int n){
  QQ val = QQ(0);
  std::vector<Par> pars;
  generate(n, pars);
  for(int i = 0; i < pars.size(); i++){
    /*
    printPar(pars[i]);
    std::cout << 1 / QQ(z(pars[i])) << std::endl;
    */
    val += (1 / QQ(z(pars[i])));
  }
  return val;
}

/*int main(){
  for(int i = 0; i <= 20; i++) {
    std::cout << inverse_sum_z_over_partitions(i) << std::endl;
  }
}*/
