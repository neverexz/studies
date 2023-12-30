#include "s21_math.h"

long double s21_exp(double x) {
  long double value = x;
  long double res = 1;
  long double step = 1;
  if (x < -14.508657) {
    return S21_SMALL_VAL;
  }
  if (x == 0.0) {
    res = 1;
  } else {
    for (int i = 1; i < 115; i++) {
      step *= value / i;
      res += step;
    }
  }
  return res;
}