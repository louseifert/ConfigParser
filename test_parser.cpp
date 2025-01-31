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
#include <cassert>              //NOLINT
#include <cstdlib>              //NOLINT
#include <exception>            //NOLINT
#include <gtest/gtest.h>        //NOLINT
#include <iostream>             //NOLINT
#include <string>               //NOLINT
#include <test_expect_equals.h> //NOLINT
std::string ret;
std::string field = "nonexistant";
std::string noexist = "The key does not exist.: ";
std::string noexistans = noexist + field;

namespace std { // NOLINT

TEST(Override_path, BasicTest) {
  std::string_ops sops;
  char **arr = new char *[1]; // NOLINT
  arr[0] = new char[strlen("-config path/to/some/loc") + 1];
  strcpy(arr[0], "-config path/to/some/loc"); // NOLINT
  ConfigParser config(1, arr, false, true, true);
  config.load_ini("writeable");
  EXPECTEQUAL(config.has_key("config"), true);
  delete[] arr[0];
  delete[] (arr);
  EXPECTEQUAL(config.has_key("config"), true);
  if (config.has_key("config"), true) {
    EXPECTEQUAL(config.get_string("config"), "path/to/some/loc");
  }
}
TEST(Override_no_ini, BasicTest) {
  std::string_ops sops;
  char **arr = new char *[1]; // NOLINT
  arr[0] = new char[strlen("-config ./path/to/some/loc") + 1];
  strcpy(arr[0], "-config ./path/to/some/loc"); // NOLINT
  ConfigParser config(1, arr, false, true, true);
  delete[] arr[0];
  delete[] arr;

  EXPECTEQUAL(config.has_key("config"), true);
  if (config.has_key("config"), true) {
    EXPECTEQUAL(config.get_string("config"), "./path/to/some/loc");
  }
}
TEST(Override_no_ini_c, BasicTest) {
  std::string_ops sops;
  char **arr = new char *[1];                             // NOLINT
  arr[0] = new char[strlen("-c ./path/to/some/loc") + 1]; // NOLINT
  strcpy(arr[0], "-c ./path/to/some/loc");                // NOLINT
  ConfigParser config(1, arr, false, true, true);
  EXPECTEQUAL(config.has_key("c"), true);
  delete[] arr[0];
  delete[] arr;

  if (config.has_key("c"), true) {
    EXPECTEQUAL(config.get_string("c"), "./path/to/some/loc");
  }
}
TEST(MultiValueTest, BasicTest) {
  std::string_ops sops;
  std::string ans0 = "./path/to/some/loc ";
  std::string arg0 = "-path " + ans0;
  char **arr = new char *[2]; // NOLINT
                              //

  // create array of size of both strings
  arr[0] = new char[arg0.size() + 1];
  strcpy(arr[0], arg0.c_str()); // NOLINT
  std::string ans1 = "./path2/another/loc ";
  std::string arg1 = "-path " + ans1;

  // create an array of size of both strings
  arr[1] = new char[arg1.size() + 1];
  strcpy(arr[1], arg1.c_str()); // NOLINT
  ConfigParser config(2, arr, false, true, true);
  std::vector<std::string> res = config.get_strings("path");
  int c = 0;
  for (size_t i = 0; i < res.size(); i++) {
    // std::cout << "->" << res[i] << std::endl;
    if (res[i] == ans0 || res[i] == ans1) {
      c++;
    }
  }
  EXPECTEQUAL(2, config.key_count("path"));
  delete[] arr[1];
  delete[] arr[0];
  delete[] arr;
}
TEST(MultiValueTest_hang, BasicTest) {
  std::string_ops sops;
  std::string ans0 = "./path/to/some/loc ";
  std::string arg0 = "-path " + ans0;
  char **arr = new char *[1]; // NOLINT
                              //
  arr[0] = new char[arg0.size() + 1];
  strcpy(arr[0], arg0.c_str()); // NOLINT
  std::string ret;
  ConfigParser *config = NULL;
  try {
    config = new ConfigParser(1, arr);
  } catch (exception &e) {
    if (config != NULL) {
      delete (config);
    }
    ret = e.what();
  }

  delete (config);
  delete[] arr[0];
  delete[] arr;
  EXPECTEQUAL("", ret);
}
TEST(MultiValueException, BasicTest) {
  std::string_ops sops;
  std::string ans0 = "./path/to/some/loc ";
  std::string arg0 = "-path " + ans0;
  char **arr = new char *[2]; // NOLINT
                              //

  // create array of size of both strings
  arr[0] = new char[arg0.size() + 1];
  strcpy(arr[0], arg0.c_str()); // NOLINT
  std::string ans1 = "./path2/another/loc ";
  std::string arg1 = "-path " + ans1;

  // create an array of size of both strings
  arr[1] = new char[arg1.size() + 1];
  strcpy(arr[1], arg1.c_str()); // NOLINT
  std::string ret;
  try {
    ConfigParser config(2, arr, false, false, false);
  } catch (exception &e) {
    // std::cerr << e.what() << std::endl;
    delete[] arr[1];
    delete[] arr[0];
    delete[] arr;
    std::string ans = ret = e.what();
  }
  std::string ans =
      "allow identicle keys _allow_multi is set to false duplicate key";
  EXPECTEQUAL(ans, ret);
}

TEST(MultiValueMulti_V_I_Test, BasicTest) {
  std::string arg0a = "-path";
  std::string arg0b = " ./path/to/loc ";
  std::string arg0c = " ./other/path1 ";
  string_ops so;
  char **arr = new char *[4];          // NOLINT
  arr[0] = new char[arg0a.size() + 1]; // NOLINT
  arr[1] = new char[arg0b.size() + 1]; // NOLINT
  arr[2] = new char[arg0a.size() + 1]; // NOLINT
  arr[3] = new char[arg0c.size() + 1]; // NOLINT
  strcpy(arr[0], arg0a.c_str());       // NOLINT
  strcpy(arr[1], arg0b.c_str());       // NOLINT
  strcpy(arr[2], arg0a.c_str());       // NOLINT
  strcpy(arr[3], arg0c.c_str());       // NOLINT
  ConfigParser config(4, arr, false, true, true);
  std::vector<std::string> res = config.get_strings("path");
  int c = 0;
  for (size_t i = 0; i < res.size(); i++) {
    // std::cout << "ele:" << res[i] << std::endl;
    // std::cout << res[i] << " " << arg0c << " " << arg0b << std::endl;
    if (res[i] == so.trim(arg0c) || res[i] == so.trim(arg0b)) {
      c++;
    }
    // if (res[i]==so.trim(arg0b)){std::cout << "res[i] matches argb" ;}
    // if (res[i]==so.trim(arg0c)){std::cout << "res[i] matches argc" ;}
  }
  int m = 0;
  m = config.key_count("path");
  EXPECTEQUAL(2, m);
  EXPECTEQUAL(c, 2);
  delete[] arr[3];
  delete[] arr[2];
  delete[] arr[1];
  delete[] arr[0];
  delete[] arr;
}

TEST(Single_char_delimiter, BasicTest) { // NOLINT
  std::string_ops sops;
  std::string result("extracted_value");
  std::string singleqoute("'");
  std::string delv = "<del>";
  std::string test1("ThisSingle1'extracted_value'text");
  std::string test2("string<del>extracted_value<del>has<del>custom_delimiters");
  vector<std::string> res = sops.split(&test1, &singleqoute);
  vector<std::string> res2 = sops.split(&test2, &delv);
  EXPECTEQUAL(res[1], result);
  EXPECTEQUAL(res2[1], result);
}
TEST(string_delimiter, BasicTest) { // NOLINT
  std::string_ops sops;
  std::string start("<body>");
  std::string end("</body>");
  std::string res("extracted_value");
  std::string test1("<body>extracted_value</body>");
  std::string_ops sop1(test1);
  EXPECTEQUAL(sops.string_between(&test1, &start, &end), res);
  EXPECTEQUAL(sop1.string_between(&start, &end), res);
  EXPECTEQUAL(sop1.string_between(start, end), res);
}

TEST(string_ops, BasicTest) {
  std::string_ops sop;
  std::string test1("thisIsATest");
  std::string test2("THISISATEST");
  std::string test3("thisisatest");
  std::string test4("this ,is,a, test");
  sop.tupper(&test1);
  EXPECTEQUAL(test1, test2);
  sop.tlower(&test1);
  EXPECTEQUAL(test1, test3);
  vector<std::string> test5 = std::string_ops::split(test4, ',');
  vector<std::string> test6({"this", "is", "a", "test"});
  for (size_t i = 0; i < test5.size(); i++) {
    std::string t = test5[i];
    string_ops::trim(&t);
    EXPECTEQUAL(string_ops::trim(t), test6.at(i));
  }
}
/**
 * Tests standard readonly open
 */
TEST(OpenReadOnly, BasicTest) {
  std::string ret = "";
  ConfigParser *config = new ConfigParser();
  try {
    config->load_ini("readonly");
    EXPECTEQUAL(config->has_key("Main.setting1"), true);
  } catch (exception &e) {
    ret = e.what();
  }
  EXPECTEQUAL(ret, "");
  delete (config);
}

/**
 * Tests opening a file that is owned and writable by the executor
 */
TEST(OpenWritable, BasicTest) {
  ConfigParser *config = new ConfigParser();
  try {
    config->load_ini("writeable");
  } catch (exception &e) {
    ret = e.what();
  }
  delete (config);
  EXPECTEQUAL(ret, "");
}

/**
 * Tries to load a file that does not exist, to test the exception
 */
TEST(Load_bad_file, BasicTest) {
  ConfigParser config;
  try {
    config.load_ini("read_only");
  } catch (exception &e) {
    ret = e.what();
  }
  EXPECTEQUAL(ret, "file either does not exist or is not a regular file");
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
    EXPECTEQUAL(config.has_flag("flag"), true);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "myflag check");
  }

  try {
    std::string ret = config.get_string("setting2");
    EXPECTEQUAL(ret, "without assigner");
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "The key does not exist.");
  }
  try {
    string ret = config.get_string("setting1");
    EXPECTEQUAL("value1 is a string with spaces", ret);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "setting1 check shouldn't throw");
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
    EXPECTEQUAL(config.has_flag("myflag"), true);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "myflag check");
  }
  try {
    string ret = config.get_string(field);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), noexistans);
  }
  try {
    string ret = config.get_string("setting1");
    EXPECTEQUAL("value1", ret);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "setting1 check shouldn't throw");
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
    EXPECTEQUAL(e.what(), "load ini for read on the get int, should not throw");
  }
  EXPECTEQUAL(true, config.has_flag("flag1"));
  try {
    EXPECTEQUAL(config.get_int("someint"), 3);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "someint check shouldn't throw");
  }
  try {
    config.get_int(field);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), noexistans);
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
    EXPECTEQUAL(e.what(), "Allow_restricted is set to false by default, "
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
    config->get_restricted_string(field);
  } catch (key_value_exception &e) {
    EXPECTEQUAL(e.what(), noexistans);
  }
  try {
    config->get_restricted_string("this");
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "Not everything works!");
  }
  vector<string> temp = config->get_restricted_keys();
  EXPECTEQUAL(temp.size(), (uint64_t)2);
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
    EXPECTEQUAL(e.what(), "opening file, shouldn't throw");
  }
  vector<string> temp = config.get_keys();
  EXPECTEQUAL((size_t)temp.size(), (size_t)4);
  EXPECTEQUAL((size_t)config.get_parm_count(), (size_t)4);
  EXPECTEQUAL((size_t)config.get_flags_count(), (size_t)1);
}
TEST(Open_more_than_one_ini, BasicTest) {
  ConfigParser config;
  try {
    config.load_ini("readonly");
    config.load_ini("readonly");
  } catch (exception &e) {
    ret = e.what();
  }
  EXPECTEQUAL(ret, "you can only load an ini file once");
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
  try {
    EXPECTEQUAL(cp.has_flag("flag1"), true);
    EXPECTEQUAL(cp.get_float("float"), stof(sfloat));
    EXPECTEQUAL(cp.get_double("float"), stod(sfloat));
    EXPECTEQUAL(cp.get_longlong("long"), stoll(slong));
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), "Expect good result");
  }
  try {
    cp.get_longlong(field);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), noexistans);
  }
  try {
    cp.get_double(field);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), noexistans);
  }
  try {
    cp.get_float(field);
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), noexistans);
  }
  try {
    cp.get_longlong("NaN");
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), NAN);
  }

  try {
    cp.get_double("NaN");
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), NAN);
  }
  try {
    cp.get_float("NaN");
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), NAN);
  }
  try {
    cp.get_int("NaN");
  } catch (exception &e) {
    EXPECTEQUAL(e.what(), NAN);
  }
  delete[] arr[3];
  delete[] arr[2];
  delete[] arr[1];
  delete[] arr[0];
  delete[] arr;
}
TEST(test_duplicate_add_ini, BasicTest) {
  ConfigParser config;
  std::string rval;
  try {
    config.load_ini("duplicate.ini");
  } catch (exception &e) {
    rval = e.what();
  }
  EXPECTEQUAL(rval,
              "Duplicate key is being added and allow multi is not set: var");
}
TEST(test_duplicate_inis, BasicTest) {
  ConfigParser config;
  std::string rval;
  try {
    config.load_ini("readonly");
    config.load_ini("readonly");
  } catch (exception &e) {
    rval = e.what();
  }
  EXPECTEQUAL(rval, "you can only load an ini file once");
}
} // namespace std
int main(int c, char **arg) {
  ::testing::InitGoogleTest(&c, arg);
  return RUN_ALL_TESTS();
}
