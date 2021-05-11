#ifndef GUARD_calc_z_h
#define GUARD_calc_z_h
#include "partition.h"
#include <gmpxx.h>

mpz_class z(partition::Par p);

mpq_class pow(mpq_class, int);
#endif
