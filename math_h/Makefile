CC = gcc 
FLAGS = -Wall -Werror -Wextra
MATH = s21_*.c
OBJECTS=*.o
TEST_LIB = ./tests/test_*.c
GCOV_FLAGS=-fprofile-arcs -ftest-coverage



all: s21_math.a test gcov_report

s21_math.a:
	$(CC) $(FLAGS) -c $(MATH)
	ar rcs s21_math.a $(OBJECTS)

test:
	$(CC) $(FLAGS) $(MATH) ./tests/*.c -lcheck -o test

clean: 
	rm -fr *.o *.a *.gch *.gcno *.gcda test gcov_test s21_test.info ./report

gcov_report: 
	$(CC) $(FLAGS) $(GCOV_FLAGS) ${TEST_LIB} ${MATH} -o test -lcheck
	./test
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info

style:
	clang-format -style=google -n *.c *.h ./tests/*.*

format:
	clang-format -style=google -i *.c *.h ./tests/*.*
