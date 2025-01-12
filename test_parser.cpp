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

/**
 * Tests standard readonly open
 */
TEST(OpenReadOnly, BasicTest) {
  ConfigParser *config = new ConfigParser();
  try {
    config->load_ini("readonly");
    EXPECT_EQ(config->has_key("Main.setting1"), true);
  } catch (exception &e) {
    std::cout << e.what() << std::endl;
  }
  delete (config);
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
 * Creates a dobject with read only and tests get int and flag
 */
TEST(Get_int, BasicTest) {
  ConfigParser config;
  try {
    config.load_ini("readonly");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(),
                 "load ini for read on the get int, should not throw");
  }
  EXPECT_EQ(true, config.has_flag("flag1"));
  try {
    EXPECT_EQ(config.get_int("someint"), 3);
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "someint check shouldn't throw");
  }
  try {
    config.get_int("does_not_exist");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "The key does not exist.");
  }
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
 * This tests the get restricted and the
 * allow restricted override creation
 */
TEST(Check_restricted_with_allow, BasicTest) {
  ConfigParser *config = new ConfigParser(true);
  try {
    config->load_ini("readonly");
  } catch (std::exception &e) {
    std::cerr << e.what();
  }
  try {
    config->get_restricted_string("ths");
  } catch (key_value_exception &e) {
    EXPECT_STREQ(e.what(), "The key does not exist.");
  }
  try {
    config->get_restricted_string("this");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "Not everything works!");
  }
  vector<string> temp = config->get_restricted_keys();
  EXPECT_EQ(temp.size(), (uint64_t)2);
  delete (config);
}

/**
 * Standard readonly ini file open checks
 * the parameter counts and flags
 */
TEST(Check_counts, BasicTest) {
  ConfigParser config;
  try {
    config.load_ini("readonly");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "opening file, shouldn't throw");
  }
  vector<string> temp = config.get_keys();
  EXPECT_EQ(temp.size(), (uint32_t)4);
  EXPECT_EQ(config.get_parm_count(), (uint32_t)4);
  EXPECT_EQ(config.get_flags_count(), (uint32_t)1);
}
TEST(Open_more_than_one_ini, BasicTest) {
  ConfigParser config;
  try {
    config.load_ini("readonly");
    config.load_ini("readonly");
  } catch (exception &e) {
    EXPECT_STREQ(e.what(), "you can only load an ini file once");
  }
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
