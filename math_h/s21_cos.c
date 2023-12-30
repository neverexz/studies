#include "s21_math.h"

long double s21_cos(double x) {
  long int fucktorial = 1;
  long double res, x1, os;
  int min = 1, flag = 1, flag1 = 1;
  if (x != x || x == -1.0 / 0.0 || x == 1.0 / 0.0) {
    res = 0.0 / 0.0;
  } else {
    x = s21_fabs(x);
    x1 = s21_fmod(x, 2 * S21_PI);
    if (x1 > S21_PI_2 && x1 < 3 * S21_PI_2) {
      flag1 = -1;
    }
    if (x1 > S21_PI_2) {
      if (x1 >= S21_PI && x1 < 3 * S21_PI_2) {
        x1 = s21_fmod(x1, S21_PI_2);
      } else {
        x1 = S21_PI_2 - s21_fmod(x1, S21_PI_2);
      }
    }
    if (x1 < 1.4089) {
      res = 1;
      for (int i = 1; i < 26; i++) {
        fucktorial = fucktorial * (2 * i) * (2 * i - 1);
        min = -min;
        os = min * (s21_pow(x1, 2 * i) / (long double)fucktorial);
        res = res + os;
      }
    } else {
      res = 0;
      min = -min;
      for (int i = 1; i < 24; i++) {
        if (i != 1) {
          fucktorial = fucktorial * (2 * i - 1) * (2 * i - 2);
        }
        min = -min;
        os = min * (s21_pow(-x1 + S21_PI_2, 2 * i - 1) / fucktorial);
        res = res + os;
      }
    }
    res = res * flag1 * flag;
    if (x == 0) {
      res = 1;
    }
  }
  return res;
}
