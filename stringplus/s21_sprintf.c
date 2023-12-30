#include <stdio.h>

#include "s21_string.h"

typedef struct {
  int plus;           //+
  int minus;          //-
  int space;          //' '
  int width;          // ширина
  int accuracy;       // точность
  int zero;           // 0
  int decpoint;       // #
  char length;        // длина
  char spec;          // тип который рассматриваем
  int flag_wight;     // * ширина
  int flag_accuracy;  // * точность
  int gg;             // g или G
  int inwidth;        // задана ли ширина явно
  int flag_pointer;
} Spec;

char transnumber[] = "0123456789abcdef";

int s21_sprintf(char *str, const char *format, ...);

// парсинг
const char *parsing_wigh(const char *format, Spec *specs);
// парсинг ширины и точности
const char *parsing_flag(const char *format, Spec *specs);
// парсинг флагов
const char *parsing_lengh(const char *format, Spec *specs);
// парсинг длины(l,h,L)
char *parsing_specifier(const char *format, Spec specs, char *str,
                        va_list *arg);
// парсинг спецификаторов

// обработка спецификаторов
char *add_char(char *str, Spec specs, char a);
// обработка спецификатора с
char *add_string(char *str, char *buff, Spec specs);
// обработка спицификатора s
char *addinttostr(va_list *arg, Spec specs, char *str);
// обработка спецификатора d
char *addunsignedtostr(va_list *arg, Spec specs, char *str);
// обработка спецификаторов u o x X
char *addpointertostr(va_list *arg, Spec specs, char *str);
// обработка спецификтаора р
char *FloatToString(va_list *arg, char *str, Spec specs);
// обработка спецификатора f
char *exp_view(va_list *arg, Spec specs, char *str);
// обработка спецификатора е Е
char *gG_view(va_list *arg, Spec specs, char *str);
// обработка спецификатора g G

char *paddingzeros(char *buf, Spec specs, int lengh_int);
// обработка флага 0, заполняет буфферный массив нулями

char *help_add(int lenspace, Spec specs, char *str, char *buff);
// вставляет буферную строку в исходную строку
char *buftostr(char *str, char *buff, Spec specs);
// тупо вставляет буферную строку в исходную строку
char *spacetostr(char *str, int lenspace);
// вставляет пробелы в исходную сроку

int size_decimal(long int num);
// вычисляет размер целого числа
char *inttostr(long int num, Spec specs, char *buf, int lengh_int1,
               int flag_minus);
// переводит целое число в строку
char *help_addinttostr(Spec specs, long int num, char *buf, int *lengh_int);
// переводит число в буферную строку в зависимости от системы счисления и
// вычиляет его длину
char *helpaddzero_addinttostr(Spec specs, char *buf, int *lengh_int,
                              int flag_minus);
// проверяет надо ли заполнять строку с числами нулями и если надо то заполняет
char *helpaddflagtostr_addinttostr(int flag_minus, char *buf, int *lengh_int,
                                   Spec specs, int flag_zero);
// добавляет в знак или начло (для 8 и 16 системы счисления) разное если надо
void backwards_str(char *buf, int lengh_int);
// переворачивает строку. Пример : на вход в функцию "426-" на выход "-624"
char *add_int(char *str, char *buff, Spec specs);
// добавляет число в строку

char *pointertostr(unsigned long int num, Spec specs, char *buf);
// переводит адресс в строку

void help_float(Spec specs, long double f, int *isNegative, long double *ipart,
                long double *fpart, int *size_buf);
// находит целую и дробную часть числа а так же его размер
char *float_inttostr(long int ipart, int *lengh_f, char *buf, int isNegative,
                     Spec specs);
// целую часть дробного числа в буферную строку
char *float_floattostr(char *buf, int *lengh_f, Spec specs, long double fpart);
// дробную часть дробного числа в буферную строку

int counter_for_pow(long double *number);
// степень десятки
char *add_exptostr(Spec specs, char *buf, int counter, int *lengh_f);
// добавление степени десятки в буферную строку

void anti_sega(Spec *specs);

char *exp_help(Spec specs, long double number, char *str, int counter,
               int isNegative);
char *float_help(Spec specs, long double f, char *str, int isNegative);

// int main(void){
//   char str1[3000];
//   char str2[3000];
//   char str3[2800];
//   char *format = "s%20.20ds";
//   s21_sprintf(str1,format,12);
//   sprintf(str2,format,12);
//   printf("%s",str1);
//   printf("ne%sne",str2);

//   return 0 ;
// }

int s21_sprintf(char *str, const char *format, ...) {
  char *str1 = str;
  va_list arg;
  va_start(arg, format);
  while (*format) {
    if (*format == '%') {
      format++;
      Spec specs = {0};
      specs.accuracy = -1;
      format = parsing_flag(format, &specs);
      format = parsing_wigh(format, &specs);
      format = parsing_lengh(format, &specs);
      str = parsing_specifier(format, specs, str, &arg);
    } else {
      *str = *format;
      str++;
    }
    format++;
  }
  *str = '\0';
  va_end(arg);
  return (str - str1);
}

const char *parsing_flag(const char *format, Spec *specs) {
  int flag = 1;
  while (flag == 1) {
    if (*format == ' ' && specs->space == 0) {
      specs->space = 1;
      format++;
    } else if (*format == '+') {
      specs->plus = 1;
      format++;
    } else if (*format == '-') {
      specs->minus = 1;
      format++;
    } else if (*format == '#') {
      specs->decpoint = 1;
      format++;
    } else if (*format == '0') {
      specs->zero = 1;
      format++;
    } else if (*format == '*') {
      specs->flag_wight = 1;
      format++;
    } else {
      flag = 0;
    }
  }
  return format;
}

const char *parsing_wigh(const char *format, Spec *specs) {
  int flag = 1;
  int flag_accuracy = 0;
  int res = 0;
  while (flag == 1) {
    if (flag_accuracy == 1 && *format == '*') {
      specs->flag_accuracy = 1;
      flag = 0;
      format++;
    }
    if (*format >= 48 && *format <= 57) {
      res = res * 10 + (*format - 48);
      if (flag_accuracy == 0) {
        specs->width = res;
        specs->inwidth = 1;
      } else {
        specs->accuracy = res;
      }
      format++;
    } else if (*format == '.') {
      flag_accuracy = 1;
      specs->flag_pointer = 1;
      res = 0;
      format++;
    } else {
      flag = 0;
    }
  }
  return format;
}

const char *parsing_lengh(const char *format, Spec *specs) {
  if (*format == 'l') {
    specs->length = 'l';
    format++;
  } else if (*format == 'L') {
    specs->length = 'L';
    format++;
  } else if (*format == 'h') {
    specs->length = 'h';
    format++;
  }
  return format;
}

char *parsing_specifier(const char *format, Spec specs, char *str,
                        va_list *arg) {
  if (specs.flag_wight == 1) {
    specs.width = va_arg(*arg, int);
  }
  if (specs.flag_accuracy == 1) {
    specs.accuracy = va_arg(*arg, int);
  }
  if (*format == 'c') {
    if (specs.length == 'l') {
      setlocale(LC_ALL, "");
      char buf[9];
      wchar_t wstr = va_arg(*arg, wchar_t);
      const wchar_t *wcsIndirectString = &wstr;
      wcsrtombs(buf, &wcsIndirectString, 8, NULL);
      specs.spec = 's';
      buf[5] = '\0';
      str = add_string(str, buf, specs);
    } else {
      char a = va_arg(*arg, int);
      specs.spec = 'c';
      specs.zero = 0;
      str = add_char(str, specs, a);
    }
  } else if (*format == 's') {
    specs.spec = 's';
    char buf[2048];
    char *buff = buf;
    if (specs.length == 'l') {
      setlocale(LC_ALL, "");
      wchar_t *wstr = va_arg(*arg, wchar_t *);
      const wchar_t *wcsIndirectString = wstr;
      wcsrtombs(buff, &wcsIndirectString, 2048, NULL);
    } else {
      char *buf1 = va_arg(*arg, char *);
      buff = buf1;
    }
    str = add_string(str, buff, specs);
  } else if (*format == 'd' || *format == 'i') {
    specs.spec = 'd';
    str = addinttostr(arg, specs, str);
  } else if (*format == 'u') {
    specs.spec = 'u';
    specs.plus = 0;
    specs.space = 0;
    str = addunsignedtostr(arg, specs, str);
  } else if (*format == 'o' || *format == 'x' || *format == 'X') {
    specs.spec = *format;
    specs.plus = 0;
    specs.space = 0;
    str = addunsignedtostr(arg, specs, str);
  } else if (*format == 'p') {
    specs.spec = *format;
    if (specs.width < specs.accuracy) {
      specs.width = specs.accuracy;
      specs.length = 'm';
    }
    str = addpointertostr(arg, specs, str);
  } else if (*format == 'f') {
    specs.spec = 'f';
    str = FloatToString(arg, str, specs);
  } else if (*format == 'e' || *format == 'E') {
    specs.spec = *format;
    str = exp_view(arg, specs, str);
  } else if (*format == 'G' || *format == 'g') {
    specs.spec = *format;
    specs.gg = 1;
    str = gG_view(arg, specs, str);
  } else if (*format == '%') {
    specs.spec = 'c';
    str = add_char(str, specs, '%');
  }
  return str;
}

char *add_char(char *str, Spec specs, char a) {
  if (specs.width == 0 || specs.width == 1) {
    *str = a;
    str++;
  } else if (specs.minus == 0) {
    if (specs.zero == 0) {
      for (int i = 0; i < specs.width - 1; i++) {
        *str = ' ';
        str++;
      }
    } else {
      str = paddingzeros(str, specs, 1);
    }
    *str = a;
    str++;
  } else if (specs.minus == 1) {
    *str = a;
    str++;
    for (int i = 0; i < specs.width - 1; i++) {
      *str = ' ';
      str++;
    }
  }
  return str;
}

char *add_string(char *str, char *buff, Spec specs) {
  int lenspace;
  if (specs.accuracy == -1 || (int)s21_strlen(buff) < specs.accuracy) {
    lenspace = specs.width - s21_strlen(buff);
  } else {
    lenspace = specs.width - specs.accuracy;
  }
  str = help_add(lenspace, specs, str, buff);
  return str;
}

char *addinttostr(va_list *arg, Spec specs, char *str) {
  long int number = 0;
  int flag_minus = 0;
  int lengh_int;
  if (specs.length == 'l') {
    number = va_arg(*arg, long int);
  } else if (specs.length == 'h') {
    number = (short)va_arg(*arg, int);
  } else {
    number = (int)va_arg(*arg, int);
  }
  char *buff = '\0';
  if (number < 0) {
    flag_minus = 1;
    number = -number;
  }
  lengh_int = size_decimal(number);
  char buf[512];
  anti_sega(&specs);
  buff = inttostr(number, specs, buf, lengh_int, flag_minus);
  str = add_int(str, buff, specs);
  return str;
}

void anti_sega(Spec *specs) {
  if (specs->accuracy > 510) {
    specs->accuracy = 500;
  }
  if (specs->width > 500) {
    specs->width = 510;
  }
}

char *addunsignedtostr(va_list *arg, Spec specs, char *str) {
  int lengh_u = 0, flag_minus = 0;
  unsigned long int number = 0;
  if (specs.length == 'l') {
    number = (unsigned long int)va_arg(*arg, unsigned long int);
  } else if (specs.length == 'h') {
    number = (short unsigned int)va_arg(*arg, int);
  } else {
    number = (unsigned int)va_arg(*arg, unsigned int);
  }
  char *buff = '\0';
  lengh_u = size_decimal(number);
  char buf[512];
  anti_sega(&specs);
  buff = inttostr(number, specs, buf, lengh_u, flag_minus);
  str = add_int(str, buff, specs);
  return str;
}

char *addpointertostr(va_list *arg, Spec specs, char *str) {
  unsigned long int number = (unsigned long int)va_arg(*arg, unsigned long int);
  char *buff = '\0';
  int lengh_p = size_decimal(number);
  anti_sega(&specs);
  if (specs.width > lengh_p) {
    lengh_p = specs.width;
  }
  char buf[512];
  if (number != 0) {
    buff = pointertostr(number, specs, buf);
    if (specs.space == 1 && specs.width > (int)s21_strlen(buff)) {
      specs.space = 0;
    }
    str = add_int(str, buff, specs);
  } else {
    str = add_string(str, "(nil)", specs);
  }
  return str;
}

char *FloatToString(va_list *arg, char *str, Spec specs) {
  int isNegative = 0;
  long double number = 0.0;
  if (specs.length == 'L') {
    number = va_arg(*arg, long double);
  } else {
    number = (double)va_arg(*arg, double);
  }
  if (specs.accuracy == -1 && specs.flag_pointer == 1) {
    specs.accuracy = 0;
  }
  specs.accuracy = (specs.accuracy == -1) ? 6 : specs.accuracy;
  str = float_help(specs, number, str, isNegative);
  return str;
}

char *float_help(Spec specs, long double f, char *str, int isNegative) {
  long double ipart;
  long double fpart;
  int lengh_f = 0;
  anti_sega(&specs);
  int size_buf = specs.width;
  help_float(specs, f, &isNegative, &ipart, &fpart, &size_buf);
  char buff[512];
  char *buf = buff;
  buf = float_inttostr(ipart, &lengh_f, buf, isNegative, specs);
  if (specs.gg) {
    specs.space = 0;
  }
  backwards_str(buf, lengh_f);
  buf = float_floattostr(buf, &lengh_f, specs, fpart);
  *buf = '\0';
  buf = buf - lengh_f;
  if (specs.gg == 1 && specs.inwidth == 0) {
    specs.width = s21_strlen(buf);
  }
  str = add_int(str, buf, specs);
  return str;
}

char *exp_view(va_list *arg, Spec specs, char *str) {
  long double number = 0.0;
  int counter = 0;
  int isNegative = 0;
  if (specs.length == 'L') {
    number = (long double)va_arg(*arg, long double);
  } else {
    number = (double)va_arg(*arg, double);
  }
  if (number < 0) {
    isNegative = 1;
    number = -number;
  }
  if (specs.accuracy == -1 && specs.flag_pointer == 1) {
    specs.accuracy = 0;
  }
  specs.accuracy = (specs.accuracy == -1) ? 6 : specs.accuracy;
  counter = counter_for_pow(&number);
  str = exp_help(specs, number, str, counter, isNegative);

  return str;
}

char *exp_help(Spec specs, long double number, char *str, int counter,
               int isNegative) {
  long double ipart, fpart;
  int size_buf = 0, lengh_f = 0;
  anti_sega(&specs);
  help_float(specs, number, &isNegative, &ipart, &fpart, &size_buf);
  if (ipart >= 10.0) {
    ipart = ipart / 10;
    counter++;
  }
  char buff[512];
  char *buf = buff;
  buf = float_inttostr(ipart, &lengh_f, buf, isNegative, specs);
  if (specs.gg == 1) {
    specs.space = 0;
  }
  backwards_str(buf, lengh_f);
  buf = float_floattostr(buf, &lengh_f, specs, fpart);
  buf = add_exptostr(specs, buf, counter, &lengh_f);
  if (specs.gg == 1 && specs.inwidth == 0) {
    specs.width = s21_strlen(buf);
  }
  str = add_int(str, buf, specs);
  return str;
}

char *gG_view(va_list *arg, Spec specs, char *str) {
  if (specs.width == 0) {
    specs.width = 6;
  }
  long double number = 0.0;
  int counter = 0;
  int isNegative = 0;
  if (specs.length == 'L') {
    number = va_arg(*arg, long double);
  } else {
    number = (double)va_arg(*arg, double);
  }
  if (number < 0) {
    isNegative = 1;
    number = -number;
  }
  long double number1 = number;
  counter = counter_for_pow(&number);
  if ((counter >= -5 && counter <= 5) ||
      (specs.accuracy > counter && counter > 0)) {
    if ((specs.accuracy == -1 || specs.accuracy == 0) &&
        specs.flag_pointer == 1) {
      specs.accuracy = 1 - counter - 1;
      specs.flag_accuracy = 1;
    } else if (specs.accuracy == -1) {
      specs.accuracy = 6 - counter - 1;
    } else {
      specs.accuracy = specs.accuracy - counter - 1;
      specs.flag_accuracy = 1;
    }
    specs.spec = 'f';
    double fuck = pow(10, specs.accuracy);
    number1 = round(number1 * fuck) / fuck;
    str = float_help(specs, number1, str, isNegative);
  } else {
    if (specs.spec == 'g') {
      specs.spec = 'e';
    } else {
      specs.spec = 'E';
    }
    if (specs.accuracy == -1) {
      specs.accuracy = 5;
    } else {
      specs.accuracy = specs.accuracy - 1;
      specs.flag_accuracy = 1;
    }
    double fuck = pow(10, specs.accuracy);
    number = round(number * fuck) / fuck;
    str = exp_help(specs, number, str, counter, isNegative);
  }
  return str;
}

char *paddingzeros(char *buf, Spec specs, int lengh_int) {
  int width = specs.width;
  if (specs.gg == 1) {
    width = lengh_int;
    lengh_int = 0;
  }
  if (specs.spec == 'p') {
    width = specs.accuracy;
  }
  if (specs.length == 'm') {
    lengh_int = lengh_int - 2;
  }
  while (width != lengh_int) {
    lengh_int++;
    *buf = '0';
    buf++;
  }
  return buf;
}

char *help_add(int lenspace, Spec specs, char *str, char *buff) {
  if (specs.gg == 1 && specs.minus == 0 && lenspace != 0 && specs.zero == 1) {
    specs.accuracy = specs.width - s21_strlen(buff);
  }
  if (lenspace > 0) {
    if (specs.minus == 0 &&
        (specs.zero == 0 ||
         (specs.zero == 1 && specs.minus == 0 && specs.gg != 1))) {
      str = spacetostr(str, lenspace);
      str = buftostr(str, buff, specs);
    } else if (specs.minus == 0 && specs.zero == 1 &&
               (specs.spec == 's' || specs.gg == 1)) {
      str = paddingzeros(str, specs, specs.accuracy);
      str = buftostr(str, buff, specs);
    } else {
      str = buftostr(str, buff, specs);
      str = spacetostr(str, lenspace);
    }
  } else {
    str = buftostr(str, buff, specs);
  }
  return str;
}

char *spacetostr(char *str, int lenspace) {
  for (int i = 0; i < lenspace; i++) {
    *str = ' ';
    str++;
  }
  return str;
}

char *buftostr(char *str, char *buff, Spec specs) {
  int i = -1;
  int res = -10;
  if (specs.accuracy != -1) {
    i = 1;
    res = 0;
  }
  if (buff != s21_NULL) {
    if (specs.accuracy < (int)s21_strlen(buff) &&
        (specs.spec == 'd' || specs.spec == 'u' || specs.spec == 'o' ||
         specs.spec == 'x' || specs.spec == 'X' || specs.spec == 'p' ||
         specs.spec == 'f' || specs.spec == 'e' || specs.spec == 'E')) {
      specs.accuracy = s21_strlen(buff);
    }
    while (*buff && res < specs.accuracy) {
      if (specs.spec == 'X' && (*buff >= 97 && *buff <= 102)) {
        *buff = *buff - 32;
      }
      *str = *buff;
      res = res + i;
      str++;
      buff++;
    }
  }
  return str;
}

int size_decimal(long int num) {
  int i = 0;
  if (num == 0) {
    i = 1;
  } else {
    while (num > 0) {
      num = num / 10;
      i++;
    }
  }
  return i;
}

char *inttostr(long int num, Spec specs, char *buf, int lengh_int1,
               int flag_minus) {
  int lengh_int = lengh_int1;
  int flag_zero = 0;
  if (num == 0) {
    flag_zero = 1;
  }
  buf = help_addinttostr(specs, num, buf, &lengh_int);
  buf = helpaddzero_addinttostr(specs, buf, &lengh_int, flag_minus);
  buf = helpaddflagtostr_addinttostr(flag_minus, buf, &lengh_int, specs,
                                     flag_zero);
  *buf = '\0';
  buf = buf - lengh_int;
  backwards_str(buf, lengh_int);
  return buf;
}

char *help_addinttostr(Spec specs, long int num, char *buf, int *lengh_int) {
  int notation = 10;
  int lengh_no10notation = 0;
  if (specs.spec == 'o') {
    notation = 8;
  } else if (specs.spec == 'x' || specs.spec == 'X') {
    notation = 16;
  }
  if (num < 9) {
    *buf = num + '0';
    lengh_no10notation++;
    buf++;
  } else {
    while (num > 0) {
      *buf = transnumber[num % notation];
      buf++;
      num = num / notation;
      lengh_no10notation++;
    }
  }
  if (specs.spec != 'd') {
    *lengh_int = lengh_no10notation;
  }
  return buf;
}

char *helpaddzero_addinttostr(Spec specs, char *buf, int *lengh_int,
                              int flag_minus) {
  if (specs.accuracy > *lengh_int) {
    while (specs.accuracy != *lengh_int) {
      *lengh_int = *lengh_int + 1;
      *buf = '0';
      buf++;
    }
  } else if (specs.zero == 1 && specs.width != 0 && specs.minus == 0 &&
             specs.accuracy == -1) {
    int k = 0;
    if (flag_minus == 1 || specs.plus == 1) {
      k = 1;
    }
    buf = paddingzeros(buf, specs, *lengh_int + k);
    *lengh_int = specs.width - k;
  }
  return buf;
}

char *helpaddflagtostr_addinttostr(int flag_minus, char *buf, int *lengh_int,
                                   Spec specs, int flag_zero) {
  if (flag_minus == 1) {
    *buf = '-';
    (*lengh_int)++;
    buf++;
  } else if (flag_minus == 0 && specs.plus == 1) {
    *buf = '+';
    (*lengh_int)++;
    buf++;
  }
  if (specs.decpoint == 1 && flag_zero == 0 && specs.spec == 'o') {
    *buf = '0';
    (*lengh_int)++;
    buf++;
  } else if (specs.decpoint == 1 && flag_zero == 0 &&
             (specs.spec == 'x' || specs.spec == 'X')) {
    *buf = specs.spec;
    buf++;
    *buf = '0';
    buf++;
    *lengh_int = *lengh_int + 2;
  }
  return buf;
}

void backwards_str(char *buf, int lengh_int) {
  char *start = buf;
  char *end = buf + lengh_int - 1;
  while (start < end) {
    char temp = *start;
    *start = *end;
    *end = temp;
    start++;
    end--;
  }
}

char *add_int(char *str, char *buff, Spec specs) {
  // для линукса вроде
  if (buff[0] == '0' && specs.accuracy == 0 && s21_strlen(buff) == 1 &&
      specs.spec != 'f') {
    buff++;
  } else if (buff[0] == '+' && buff[1] == '0' && specs.accuracy == 0) {
    buff++;
    *buff = '+';
  }
  int lenspace = 0;
  if (specs.space == 1 && specs.plus == 0 && buff[0] != '-' &&
      specs.width <= (int)s21_strlen(buff)) {
    lenspace = 1;
    specs.minus = 0;

  } else if (specs.width > (int)s21_strlen(buff)) {
    lenspace = specs.width - s21_strlen(buff);
  }
  str = help_add(lenspace, specs, str, buff);
  return str;
}

char *pointertostr(unsigned long int num, Spec specs, char *buf) {
  int notation = 16;
  int lengh = 2;
  if (specs.plus == 1) {
    lengh++;
  }
  if (num < 9) {
    *buf = num + '0';
    lengh++;
    buf++;
  } else {
    while (num > 0) {
      *buf = transnumber[num % notation];
      buf++;
      num = num / notation;
      lengh++;
    }
  }
  if (specs.zero == 1 && specs.accuracy == -1) {
    specs.accuracy = specs.width;
  }
  if (specs.accuracy > lengh && (specs.zero == 1 || specs.length == 'm')) {
    buf = paddingzeros(buf, specs, lengh);
    if (specs.width > lengh) {
      lengh = specs.width;
    }
    if (specs.length == 'm') {
      lengh = lengh + 2;
    }
  }
  *buf = 'x';
  buf++;
  *buf = '0';
  buf++;
  if (specs.plus == 1) {
    *buf = '+';
    buf++;
  }
  *buf = '\0';
  buf = buf - lengh;
  backwards_str(buf, lengh);
  return buf;
}

void help_float(Spec specs, long double f, int *isNegative, long double *ipart,
                long double *fpart, int *size_buf) {
  double fact = pow(10, specs.accuracy);
  f = round(f * fact) / fact;
  if (f < 0) {
    *isNegative = 1;
    f = -f;
  }
  *fpart = modfl(f, ipart);
  int len_d = size_decimal(*ipart);
  if ((len_d + specs.accuracy + 3) > specs.width) {
    *size_buf = len_d + specs.accuracy + 3;
  }
}

char *float_inttostr(long int ipart, int *lengh_f, char *buf, int isNegative,
                     Spec specs) {
  if (ipart == 0) {
    *lengh_f = *lengh_f + 1;
    *buf = ipart % 10 + '0';
    buf++;
  }
  if (specs.decpoint == 1 || specs.accuracy != 0) {
    *lengh_f = *lengh_f + 1;
  }
  if (isNegative || specs.plus == 1) {
    *lengh_f = *lengh_f + 1;
  }
  while (ipart != 0) {
    *lengh_f = *lengh_f + 1;
    *buf = ipart % 10 + '0';
    buf++;
    ipart /= 10;
  }
  int len = specs.accuracy + *lengh_f;
  if (specs.spec == 'e' || specs.spec == 'E') {
    len = len + 4;
  }
  if (specs.width > len && specs.minus == 0 && specs.zero == 1) {
    while (specs.width != len) {
      *buf = '0';
      buf++;
      len++;
      *lengh_f = *lengh_f + 1;
    }
  }
  if (isNegative) {
    *buf = '-';
    buf++;
  } else if (specs.plus == 1) {
    *buf = '+';
    buf++;
  } else if (specs.space == 1 && specs.gg == 1) {
    *buf = ' ';
    specs.space = 0;
    buf++;
    *lengh_f = *lengh_f + 1;
  }
  if (specs.decpoint == 1 || specs.accuracy != 0) {
    *lengh_f = *lengh_f - 1;
  }
  buf = buf - *lengh_f;
  return buf;
}

char *float_floattostr(char *buf, int *lengh_f, Spec specs, long double fpart) {
  buf = buf + *lengh_f;
  int acc = 0;
  long double fpart1 = fpart;
  if (specs.gg == 1 &&
      (specs.spec == 'e' || specs.spec == 'E' || specs.spec == 'f')) {
    for (int j = 0; j < specs.accuracy; j++) {
      fpart1 = fpart1 + 0.000000000001;
      fpart1 *= 10;
      int digit = fpart1;
      if (digit != 0) {
        acc = j + 1;
      }
      fpart1 -= digit;
    }
    if (acc == 0 && specs.decpoint == 1 && specs.gg == 1 &&
        specs.accuracy != 0 && specs.flag_accuracy) {
      acc = 1;
    }
    if (specs.decpoint == 1 && specs.gg == 1 &&
        ((int)s21_strlen(buf) >= specs.width || specs.inwidth == 1)) {
      acc = specs.accuracy;
    }
    specs.accuracy = acc;
  }
  if (specs.accuracy != 0 || specs.decpoint == 1) {
    *lengh_f = *lengh_f + 1;
    *buf = '.';
    buf++;
  }
  int her = 11;
  if (specs.spec == 'f' || specs.accuracy >= 16) {
    her = 10;
  }
  for (int j = 0; j < specs.accuracy; j++) {
    *lengh_f = *lengh_f + 1;

    if (specs.accuracy >= 10) {
      fpart = fpart + pow(10, -(her + (specs.accuracy - 10)));
    } else {
      fpart = fpart + 0.0000000001;
    }
    fpart *= 10;
    int digit = fpart;
    if (digit == 0) {
      *buf = '0';
    } else {
      *buf = digit + '0';
    }
    buf++;
    fpart -= digit;
  }
  return buf;
}

int counter_for_pow(long double *number) {
  int counter = 0;
  if (*number != 0) {
    if (*number < 1) {
      while (*number < 1) {
        *number = *number * 10;
        counter = counter - 1;
      }
    } else {
      while (*number >= 10) {  //
        *number = *number / 10;
        counter++;
      }
    }
  }
  return counter;
}

char *add_exptostr(Spec specs, char *buf, int counter, int *lengh_f) {
  *buf = specs.spec;
  buf++;
  *lengh_f = *lengh_f + 2;
  if (counter >= 0) {
    *buf = '+';
    buf++;
  } else {
    counter = -counter;
    *buf = '-';
    buf++;
  }
  if (counter >= 100) {
    *buf = '0' + counter / 100;
    counter = counter - (counter / 100) * 100;
    buf++;
    *lengh_f = *lengh_f + 1;
  }
  *buf = '0' + counter / 10;
  buf++;
  *buf = '0' + counter % 10;
  buf++;
  *lengh_f = *lengh_f + 2;
  *buf = '\0';
  buf = buf - *lengh_f;
  return buf;
}
