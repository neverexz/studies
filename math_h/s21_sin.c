#include "s21_math.h"

long double s21_sin(double x) {
  long double res, x1, os;
  int min = 1, flag = 1, flag1 = 1;
  long int fucktorial = 1;
  if (x != x || x == -1.0 / 0.0 || x == 1.0 / 0.0) {
    res = 0.0 / 0.0;
  } else {
    if (x < 0) {
      flag = -1;
      x = s21_fabs(x);
    }
    x1 = s21_fmod(x, 2 * S21_PI);
    if (x1 >= S21_PI) {
      flag1 = -1;
      x1 = s21_fmod(x1, S21_PI);
    }
    if (x1 > S21_PI / 2) {
      x1 = S21_PI / 2 - s21_fmod(x1, S21_PI / 2);
    }
    if (x1 == 0) {
      res = 0;
    } else if (x1 < 1.4089) {
      res = x1;
      for (int i = 2; i < 24; i++) {
        fucktorial = fucktorial * (2 * i - 2) * (2 * i - 1);
        min = -min;
        os = min * (s21_pow(x1, 2 * i - 1) / (long double)fucktorial);
        res = res + os;
      }
    } else {
      res = 1;
      for (int i = 1; i < 26; i++) {
        if (i == 1) {
          fucktorial = (fucktorial) * (2 * i);
        } else {
          fucktorial = (fucktorial) * (2 * i) * (2 * i - 1);
        }
        min = -min;
        os = min * (s21_pow(x1 - S21_PI / 2, 2 * i) / (long double)fucktorial);
        res = res + os;
      }
    }
    res = res * flag1 * flag;
  }
  return res;
}
