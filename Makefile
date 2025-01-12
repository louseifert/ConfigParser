compiler=g++
cflags=-O3  -Wpedantic -fsanitize=address -g -fstack-protector -Wextra -Wall -Wextra
builddir=build
C_version=c++23
file=main


make:
	echo "Making example file in ${builddir}/${file} the test files, please review"
	${compiler} ${cflags} ${file}.cpp -std=${C_version} -I ./include/ -o ${builddir}/${file}


lint:
	cppcheck main.cpp
	cppcheck include/ConfigParser.hpp
	cppcheck include/ParserExceptions.hpp 
	cppcheck test_parser.cpp
	cpplint --filter=-whitespace/comments,-readability/todo main.cpp
	cpplint --filter=-whitespace/comments,-readability/todo include/ConfigParser.hpp
	cpplint --filter=-whitespace/comments,-readability/todo include/ParserExceptions.hpp
	cpplint --filter=-whitespace/comments,-readability/todo test_parser.cpp

format:
	clang-format -i include/ConfigParser.hpp
	clang-format -i include/ParserExceptions.hpp
	clang-format -i main.cpp
	clang-format -i test_parser.cpp
	sed  -i 's/^public:/ public:/g' include/*
	sed  -i 's/^private:/ private:/g' include/*


test:
	echo "------>warning<------ You must change the ownership and redability to 755 and root:executor_user for the tests to work, if you do not they will fail, this is works and intended."
	g++ -O3 -fsanitize-address-use-after-scope -fprofile-arcs -ftest-coverage -Wpedantic -fsanitize=address -g -fstack-protector -Wextra -Wall -Wextra -std=c++23 -I ./include/ -I /usr/src/googletest/googletest/include/ test_parser.cpp -o build/test_parser -lgtest -lgtest_main 
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
	
