/* *
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

#include "include/ConfigParser.hpp" //NOLINT
#include <exception>                //NOLINT
#include <iostream>                 //NOLINT
using namespace std;                // NOLINT

/**
 * ! mainpage Main CPP and examples
 *
 * section intro_sec External resources
 * <a href="./coverage.html">Test Coverage</a>
 * If the options exist both in the ini and the command line the ini will
 * override the command line This is so default options can be specified by an
 * administrator or other access and not overridden
 * TODO: current implementation is the opposite of this.
 */

int main(int c, char **arg) {
  // ConfigParser *config= new ConfigParser(c,arg);
  /**
   * Create a config object and parse the command line options passed in main
   */
  std::ConfigParser *config = new std::ConfigParser(c, arg);
  /**
   * If the config file parameter was passed to the application via CLP open it
   */
  if (config->has_key("config_file")) {
    try {
      std::cout << "loading config: " << config->get_string("config_file")
                << std::endl;
      config->load_ini(config->get_string("config_file"));
    } catch (file_access_exception &e) {
      std::cout << e.what() << std::endl;
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }

  } else {
    try {
      /**
       * Just load the file directly
       */
      config->load_ini("config_file.ini");
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
    }
  }
  /**
   * check if it has a -flag you passed it
   */
  if (config->has_flag("command_flag")) {
    std::cout << "Has command flag" << std::endl;
  }
  /**
   * Access attributes by string name
   * keep in mind if you are resusing values it is better if you
   * grab it once
   */
  std::vector<string> olist = config->get_keys();
  for (uint32_t i = 0; i < olist.size(); i++) {
    std::cout << "key:" << olist[i] << " value:" << config->get_string(olist[i])
              << std::endl;
  }
  std::cout << "flag_count:" << config->get_flags_count() << std::endl;
  std::cout << "parm_count:" << config->get_parm_count() << std::endl;
  delete (config);
  return 0;
}
