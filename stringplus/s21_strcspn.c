#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  int sizestr1 = s21_strlen(str1);
  int sizestr2 = s21_strlen(str2);
  s21_size_t res = 0;
  int flag = 0;
  for (int i = 0; i < sizestr1; i++) {
    for (int j = 0; j < sizestr2; j++) {
      if (str1[i] == str2[j]) {
        res = i;
        j = sizestr2;
        flag = 1;
      }
    }
    if (flag != 0) {
      i = sizestr1;
    }
    if (res == 0 && i == sizestr1 - 1) {
      res = sizestr1;
    }
  }
  return res;
}
