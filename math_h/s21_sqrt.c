#include "s21_math.h"

long double s21_sqrt(double x) {
  long double res = 1;
  if (x < 0.0) {
    res = S21_NAN;
  } else {
    for (int i = 0; i < 100; i++) {
      res = 0.5 * (res + x / res);
    }
  }
  return res;
}