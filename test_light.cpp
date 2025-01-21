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
#include <ConfigParser.hpp>     //NOLINT
#include <ParserExceptions.hpp> //NOLINT
#include <cstdlib>              //NOLINT
#include <exception>            //NOLINT
#include <gtest/gtest.h>        //NOLINT
#include <iostream>             //NOLINT
#include <string>               //NOLINT

namespace std { // NOLINT
TEST(Single_char_delimiter, BasicTest) { //NOLINT
  std::string_ops sops;
  std::string result("extracted_value");
  std::string singleqoute("'");
  std::string delv = "<del>";
  std::string test1("ThisSingle1'extracted_value'text");
  std::string test2("string<del>extracted_value<del>has<del>custom_delimiters");
  vector<std::string> res = sops.split(&test1, &singleqoute);
  vector<std::string> res2 = sops.split(&test2, &delv);
  EXPECT_EQ(res[1], result);
  EXPECT_EQ(res2[1], result);
}
TEST(string_delimiter, BasicTest) { //NOLINT
  std::string_ops sops;
  std::string start("<body>");
  std::string end("</body>");
  std::string res("extracted_value");
  std::string test1("<body>extracted_value</body>");
  std::string_ops sop1(test1);
  EXPECT_EQ(sops.string_between(&test1, &start, &end), res);
  EXPECT_EQ(sop1.string_between(&start, &end), res);
  EXPECT_EQ(sop1.string_between(start, end), res);
}

TEST(string_ops, BasicTest) {
  std::string_ops sop;
  std::string test1("thisIsATest");
  std::string test2("THISISATEST");
  std::string test3("thisisatest");
  std::string test4("this ,is,a, test");
  sop.tupper(&test1);
  EXPECT_EQ(test1, test2);
  sop.tlower(&test1);
  EXPECT_EQ(test1, test3);
  vector<std::string> test5 = std::string_ops::split(test4, ',');
  vector<std::string> test6({"this", "is", "a", "test"});
  for (size_t i = 0; i < test5.size(); i++) {
    std::string t = test5[i];
    string_ops::trim(&t);
    EXPECT_EQ(string_ops::trim(t), test6.at(i));
  }
}

/**
 * Tests opening a file that is owned and writable by the executor
 */
TEST(OpenWritable, BasicTest) {
  ConfigParser *config = new ConfigParser();
  try {
    config->load_ini("writeable");
  } catch (security_exception &e) {
    EXPECT_STREQ(e.what(),
                 "Config files should not be writeable by the executor");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "The world falls apart here");
  }
  delete (config);
}

/**
 * Tries to load a file that does not exist, to test the exception
 */
TEST(Load_bad_file, BasicTest) {
  ConfigParser config;
  try {
    config.load_ini("read_only");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(),
                 "file either does not exist or is not a regular file");
  }
}

TEST(TestSpacesInArgs, BasicTest) {
  char **arr = new char *[3]; // NOLINT
  arr[0] = new char[strlen("-setting1=value1 is a string with spaces") + 1];
  arr[1] = new char[strlen("-flag") + 1];
  arr[2] = new char[strlen("-setting2 without assigner") + 1];
  strcpy(arr[0], "-setting1=value1 is a string with spaces"); // NOLINT
  strcpy(arr[1], "-flag");                                    // NOLINT
  strcpy(arr[2], "-setting2 without assigner");               // NOLINT
  ConfigParser config(3, arr);
  try {
    EXPECT_EQ(config.has_flag("flag"), true);
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "myflag check");
  }
  try {
    std::string ret = config.get_string("setting2");
    EXPECT_EQ(ret, "without assigner");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "The key does not exist.");
  }
  try {
    string ret = config.get_string("setting1");
    EXPECT_EQ("value1 is a string with spaces", ret);
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "setting1 check shouldn't throw");
  }
  delete[] arr[2];
  delete[] arr[1];
  delete[] arr[0];
  delete[] arr;
}

/**
 * Creates a basic object with a mockclp
 */
TEST(Create_with_CLP_ARGS, BasicTest) {
  char **arr = new char *[2];
  arr[0] = new char[strlen("-myflag") + 1];
  arr[1] = new char[strlen("-setting1=value1") + 1];
  strcpy(arr[0], "-myflag");          // NOLINT
  strcpy(arr[1], "-setting1=value1"); // NOLINT

  ConfigParser config(2, arr);
  try {
    EXPECT_EQ(config.has_flag("myflag"), true);
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "myflag check");
  }
  try {
    string ret = config.get_string("asdf");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "The key does not exist.");
  }
  try {
    string ret = config.get_string("setting1");
    EXPECT_EQ("value1", ret);
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "setting1 check shouldn't throw");
  }
  delete[] arr[1];
  delete[] arr[0];
  delete[] arr;
}


/**
 * Gets a restricted string seperated, as multiple test targets are difficult to
 * troubleshoot.
 */
TEST(Check_restricted, BasicTest) {
  ConfigParser *config = new ConfigParser();
  try {
    config->get_restricted_string("");
  } catch (security_exception &e) {
    EXPECT_STREQ(e.what(), "Allow_restricted is set to false by default, "
                           "enable it to use restricted");
  }
  delete (config);
}


/**
 * Tests numeric type conversions
 */
TEST(Test_numeric_types, BasicTest) {
  char **arr = new char *[4];
  string sfloat("2342.234234");
  string slong("23328374");
  string ssfloat = "-float=" + sfloat;
  string sslong = "-long=" + slong;
  string NaN = "-NaN=not_a_number";
  string fl = "--flag1";
  arr[0] = new char[ssfloat.size() + 1];
  arr[1] = new char[sslong.size() + 1];
  arr[2] = new char[NaN.size() + 1];
  arr[3] = new char[fl.size() + 1];

  strcpy(arr[0], ssfloat.c_str()); // NOLINT
  strcpy(arr[1], sslong.c_str());  // NOLINT
  strcpy(arr[2], NaN.c_str());     // NOLINT
  strcpy(arr[3], fl.c_str());      // NOLINT

  ConfigParser cp(4, arr);

  string noexist = "The key does not exist.";
  try {
    EXPECT_EQ(cp.has_flag("flag1"), true);
    EXPECT_EQ(cp.get_float("float"), stof(sfloat));
    EXPECT_EQ(cp.get_double("float"), stod(sfloat));
    EXPECT_EQ(cp.get_longlong("long"), stoll(slong));
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "Expect good result");
  }
  try {
    cp.get_longlong("nonexistant");
  } catch (exception &e) {
    EXPECT_EQ(e.what(), noexist);
  }
  try {
    cp.get_double("nonexistant");
  } catch (exception &e) {
    EXPECT_EQ(e.what(), noexist);
  }
  try {
    cp.get_float("nonexistant");
  } catch (exception &e) {
    EXPECT_EQ(e.what(), noexist);
  }
  try {
    cp.get_longlong("NaN");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), NAN);
  }

  try {
    cp.get_double("NaN");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), NAN);
  }
  try {
    cp.get_float("NaN");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), NAN);
  }
  try {
    cp.get_int("NaN");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), NAN);
  }
  delete[] arr[3];
  delete[] arr[2];
  delete[] arr[1];
  delete[] arr[0];
  delete[] arr;
}

} // namespace std
int main(int c, char **arg) {
  ::testing::InitGoogleTest(&c, arg);
  return RUN_ALL_TESTS();
}
