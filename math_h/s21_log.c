#include "s21_math.h"

long double s21_log(double x) {
  int n = 0, k = 0;
  int kol = 1800;
  long double res = 0;
  long double os = 0, elem = -1;
  double f;
  if (x == 0) {
    res = -1.0 / 0.0;
  } else if (x < 0) {
    res = 0.0 / 0.0;
  } else if (x != x) {
    res = -0.0 / 0.0;
  } else {
    while (x >= 10) {
      x = (double)x / 10;
      n++;
    }
    while (x >= 2) {
      x = (double)x / 2;
      k++;
    }
    f = x - 1;
    res = S21_LN10 * n + S21_LN2 * k;
    for (int i = 0; i < kol; i++) {
      elem = elem * f * (-1);
      os = os + (elem / (i + 1));
    }
    res = res + os;
  }
  return res;
}
