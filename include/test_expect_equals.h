/**
 * This software has a Copyright 2024 to Louis C Seifert III all rights reserved
 * it is freely distributable under the MIT license. If you use this api:
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions: THE SOFTWARE IS
 * PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * */
/**
 * test_parser examples of most of the funcitonality
 *
 * section intro_sec External resources
 * <a href="./coverage.html">Test Coverage</a>
 * If the options exist both in the ini and the command line the ini will
 * override the command line This is so default options can be specified by an
 * administrator or other access and not overridden
 */
// NOLINT
#ifndef __EXPECT_EQUALS_TEST_EXCEPTION // NOLINT
#define __EXPECT_EQUALS_TEST_EXCEPTION // NOLINT
#include <cassert>                     //NOLINT
#include <cstdlib>                     //NOLINT
#include <exception>                   //NOLINT
#include <iostream>                    //NOLINT
#include <sstream>                     //NOLINT
#include <string>                      //NOLINT

/**
 * @brief creates an exception that is used in the test suite, replaces google
 * functions which seem to cause issues without reasonably quick and adequate
 * explaination.
 */
class testexception : public std::exception {
public:
  /**
   * @brief standard constructor
   * @param message
   */
  explicit testexception(const std::string &message) : _message(message) {}
  explicit testexception(std::string *message) : _message(*message) {}

  /**
   * @brief what message for exception
   * returns string
   */
  virtual const char *what() const noexcept { return _message.c_str(); }

private:
  std::string _message;
};

const static std::string expectequal_message =
    "EXPECTING EQUAL VALUES FOR "; // NOLINT
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param string a
 * @param string b
 */
void EXPECTEQUAL(std::string a, std::string b) {
  if (a.size() != b.size()) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
  for (size_t i = 0; i < a.size(); i++) {
    if (a[i] != b[i]) {
      std::stringstream s;
      s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
      throw testexception(s.str());
    }
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param size_t a
 * @param size_t b
 */
void EXPECTEQUAL(size_t a, size_t b) {
  if (a != b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param bool a
 * @param bool b
 */
void EXPECTEQUAL(bool a, bool b) {
  if (a != b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param long long a
 * @param long long b
 */
void EXPECTEQUAL(long long a, long long b) {
  if (a != b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param double a
 * @param double b
 */
void EXPECTEQUAL(double a, double b) {
  if (a != b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param uint32_t a
 * @param uint32_t b
 */
void EXPECTEQUAL(uint32_t a, uint32_t b) {
  if (a != b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param int32_t a
 * @param int32_t b
 */
void EXPECTEQUAL(int32_t a, int32_t b) {
  if (a != b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}
/**
 * @brief values A and B to see if they are the same throws and exception if
 * they are not
 * @param size_t a
 * @param uint32_t b
 */
void EXPECTEQUAL(size_t a, uint32_t b) {
  if ((uint32_t)a != (uint32_t)b) {
    std::stringstream s;
    s << expectequal_message << " : \na=" << a << " \nb=" << b << std::endl;
    throw testexception(s.str());
  }
}

#endif // NOLINT
