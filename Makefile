compiler=g++
cflags=-O0 -march=sandybridge -Wpedantic -fsanitize=address -g -fstack-protector -Wextra -Wall -Wextra
builddir=build
includes=/usr/include/mysql/
objectdir=/usr/lib/mysql/
objects=mariadbcpp
C_version=c++23
file=main

lint:
	cppcheck main.cpp; cppcheck include/ConfigParser.hpp; cppcheck include/ParserExceptions.hpp cppcheck test_parser.cpp
	cpplint main.cpp; cpplint include/ConfigParser.hpp; cpplint include/ParserExceptions.hpp cpplint test_parser.cpp

format:
	clang-format -i include/ConfigParser.hpp
	clang-format -i include/ParserExceptions.hpp
	clang-format -i main.cpp
	clang-format -i test_parser.cpp


make:
	echo "Making example file in ${builddir}/${file} the test files"
	${compiler} ${cflags} ${file}.cpp -std=${C_version} -I ./include/ -I ${includes} -L ${objectdir}  -l ${objects} -o ${builddir}/${file}

test:
	g++ -O3 -march=sandybridge -fprofile-arcs -ftest-coverage -Wpedantic -fsanitize=address -g -fstack-protector -Wextra -Wall -Wextra -std=c++23 -I ./include/ -I /usr/src/googletest/googletest/include/ test_parser.cpp -o build/test_parser -lgtest -lgtest_main 
	build/test_parser -myflag -setting1="value1"

coverage:
	rm documentation/html/coverage.html
	gcovr --html documentation/html/coverage.html
	gcovr

cleantest:
	rm build/*parser.gcno
	rm build/*parser.gcda
	#rm *.gcov

documentation:
	doxygen ConfigParser_doxygen.cfg

clean:
	rm build/*
	
