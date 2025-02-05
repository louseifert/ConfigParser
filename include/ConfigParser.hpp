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
 */

#ifndef _CONFIG_PARSER_HPP                             // NOLINT
#define _CONFIG_PARSER_HPP                             // NOLINT
#include <exception>                                   // NOLINT
#include <filesystem>                                  // NOLINT
#include <fstream>                                     // NOLINT
#include <iostream>                                    // NOLINT
#include <map>                                         // NOLINT
#include <sstream>                                     // NOLINT
#include <string_ops.hpp>                              // NOLINT
#include <vector>                                      // NOLINT
#pragma GCC diagnostic push                            // NOLINT
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized" // NOLINT
#include <ParserExceptions.hpp>                        // NOLINT
#include <cctype>                                      // NOLINT
// TODO replace regex create sanitizerclass and propriatary implementation to
// sub // NOLINT
#include <regex>                            // NOLINT
#include <set>                              // NOLINT
#include <string>                           // NOLINT
#define NOKEY "The key does not exist."     // NOLINT
#define STROPVARS "^[A-Z=a-z0-9\"\\-_.~]*$" // NOLINT
#define STROPFLAGS "^[A-Za-z0-9-_.]*"       // NOLINT
#define NAN "NaN: Not a number"             // NOLINT
namespace std {                             // NOLINT

/*******************************************************************
 * @class ConfigParser
 * @brief class is built for parsing a standard INI,
 * planned support for other file formats
 */
class ConfigParser {
private:
  /**
   * Allow multiple entries of the same flag type to be implemented future
   * feature
   */
  bool allow_multi = false;
  /**
   * allow the override of ini security checks
   */
  bool inisec_override = false;
  // TODO(louIII):change map to multi map or add multimap //NOLINT
  /** @brief map of options internal */
  multimap<std::string, std::string> options;
  /** @brief flags set*/
  /** #brief contains count for options for future function*/
  map<std::string, int16_t> options_count;
  set<string> flags;
  /** @brief default config file */
  string configFile = "";
  /** @brief map of restricted internal */
  multimap<std::string, std::string> restricted;
  /** @brief allows override of restricted lock */
  bool allow_override = false;
  /** @brief allows the parser to read
   * writeable files comes with security risks */
  bool allow_writeable = false;
  /** @brief allows access to restricted list */
  bool allow_restricted = false;
  /** @brief prevents more than one ini from being loaded per object */
  bool ini_loaded = false;
  /** @brief internal string ops */
  string_ops stringops;
  /** @brief keep track of params loaded by clp */
  int clp_parm_count = 0;
  /** @brief keep track of parm count vs restricted */
  int ini_parm_count = 0;
  /** @brief track restricted count */
  int restricted_count = 0;

public:
  enum FILETYPE { INI, JSON, XML, TEST };
  /**
   * @brief plain constructor
   */
  ConfigParser() {}

  /**
   * @brief parses standard c/c++ arguements in the constructor
   * constructors that allow override of default behavior
   * takes argc **argv
   * @param c (int argc,
   * @param *v[]  **argv)
   * */
  ConfigParser(int c, char *v[]) { parse(c, v); }

  /**
   * @brief By default the regex allows a limited superset of alpha num
   * Anything that has characters that fall outside of standard path and
   * alphanum is placed in restricted and not accessible except by specifying
   * that it is allowed in the constructor, this is so
   * if the object is passed to other parts in the code it can't
   * access them except through the interface.
   * @param _allow_restricted bool
   */
  explicit ConfigParser(bool _allow_restricted) {
    this->allow_restricted = _allow_restricted;
  }
  /**
   * @brief ini sec override constructor
   * @param bool allow restricted variable use
   * @param bool override INI security checks less secure
   */
  ConfigParser(bool _allow_restricted, bool _inisec_override) {
    allow_restricted = _allow_restricted;
    inisec_override = _inisec_override;
  }
  /**
   * @brief this allows processing of the command line and allowing access to
   * the restricted/filtered ini or clp settings. takes argc and **argv
   * @param c int argc from main
   * @param *v[] **argv from main
   * @param _allow_restricted
   */
  ConfigParser(int c, char *v[], bool _allow_restricted) {
    this->allow_restricted = _allow_restricted;
    parse(c, v);
  }
  ConfigParser(int c, char *v[], bool _allow_restricted, bool _inisec_override,
               bool _allow_multi) {
    this->allow_restricted = _allow_restricted;
    this->inisec_override = _inisec_override;
    this->allow_multi = _allow_multi;
    parse(c, v);
  }
  /**
   * @brief this allows processing of the command line and allowing access to
   * the restricted/filtered ini or clp settings.
   * @param c int argc from main clp
   * @param *v[] **argv from main clp
   * @param _allow_restricted allows access to restricted vars
   * @param string_ops_values the regex to determine valid string
   * @param string_ops_flags regex to determin valid flags TODO
   */
  ConfigParser(int c, char *v[], bool _allow_restricted,
               string string_ops_values, string string_ops_flags)
      : stringops(string_ops_values, string_ops_flags) {
    parse(c, v);
    this->allow_restricted = _allow_restricted;
  }

  /**
   * @brief parses both settings and clp options have precidence over ini on
   * identical identifiers takes filename type argc argv
   * TODO(lou): needs to be completed or removed.
   * @param filename that will be loaded
   * @param type of file for switching parsers
   * @param c int argc
   * @param *v[] **argv
   * */
  ConfigParser(string filename, FILETYPE type, int c, char *v[]) {
    parse(c, v);
    // TODO(lou): FINISH IMPLEMENTATION OF TYPES
    if (type == ConfigParser::FILETYPE::INI) {
      load_ini(filename);
    } else if (type == ConfigParser::FILETYPE::TEST) {
      cout << "Parser FILETYPE is TEST TYPE that prints this line" << endl;
    }
  }

  /**
   * @brief check permission of the file before opening it to make sure it is
   * read only
   */
  bool check_permission(std::string filename) {
    auto perm = std::filesystem::status(filename).permissions();
    if (!inisec_override && perm == std::filesystem::perms::others_write) {
      return false;
    }
    if (!inisec_override && perm == std::filesystem::perms::group_write) {
      return false;
    }
    if (!inisec_override && perm == std::filesystem::perms::owner_write) {
      return false;
    }
    if (!inisec_override && perm == std::filesystem::perms::group_exec) {
      return false;
    }
    if (!inisec_override && perm == std::filesystem::perms::owner_exec) {
      return false;
    }
    if (!inisec_override && perm == std::filesystem::perms::others_exec) {
      return false;
    }
    if (!inisec_override && perm == std::filesystem::perms::others_read) {
      return false;
    }
    return true;
  }

  /**
   * @brief Parses a config file with basic INI structure lines starting with ;
   * and # are comments, category or sections are bracketed [section] when
   * accessing them by the get_string or get int function variables under
   * section heads [title] are accessed by title.variablename See test cases for
   * basic usage This function will throw an exception if the file doesn't
   * exist, or the file is writeable by the executor This enforced /etc/
   * security best practice.
   * @param filename of file to be loaded for parameters
   */
  void load_ini(string filename) {
    if (ini_loaded) {
      throw security_exception("you can only load an ini file once");
    }
    /**
     * Don't try and open the file, and throw an exception,
     * if the file does not exist or is not a regular file.
     */
    if (!std::filesystem::exists(filename) ||
        !std::filesystem::is_regular_file(filename)) {
      throw file_access_exception(
          (const string) "file either does not exist or is not a regular file");
    }

    // Throw a security exception if the file is writable
    if (!check_permission(filename)) {
      throw security_exception(
          (const string) "Setting config files to read only is more secure, or "
                         "use override constructor");
    }
    std::string line;

    std::ifstream file(filename, std::ios::in);
    string section_head = "";
    while (std::getline(file, line)) {
      if (line.empty()) {
        continue;
      }
      while (line[0] == ' ' || line[0] == '\t') {
        line.erase(0, 1);
      }
      if (line.size() == 0 || line[0] == '#') {
        continue;
      }
      if (line[0] == ';' ||
          (line.length() > 2 && line[0] == '/' && line[1] == '/')) {
        continue;
      }
      /*TODO add regex check here*/
      if (line[0] == '[') {
        section_head = line.substr(1, line.find_first_of(']') - 1) + ".";
        continue;
      }

      const size_t s = line.find_first_of("=", 0);
      if (s == line.npos) {
        flags.insert(line);
        continue;
      }

      string key = line.substr(0, s);
      string value = line.substr(s + 1, line.length());
      string_ops::strip_qoutes(&key);
      string_ops::strip_qoutes(&value);
      key = section_head + key;

      if (!stringops.strip_check(&key) || !stringops.strip_check(&value)) {
        restricted.emplace(key, value);
        restricted_count++;
      } else {
        if (options.contains(key)) {
          if (allow_multi) {
            options_count[key] = options_count[key] + 1;
            options.emplace(key, value);
          } else {
            throw security_exception(
                "Duplicate key is being added and allow multi is not set: " +
                key);
          }
        } else {
          options.emplace(key, value);
          ini_parm_count++;
        }
      }
    }
    ini_loaded = true;
  }

  /**
   * @brief returns the number of flags that have been parsed from the command
   * line
   * @returns uint32_t
   */
  uint32_t get_flags_count() { return flags.size(); }

  /**
   * @brief returns the number of keys that have been parsed from the ini file
   * and clp arguments
   * @returns uint32_t count
   */
  uint32_t get_parm_count() { return ini_parm_count + clp_parm_count; }

  /**
   * @brief returns a vector of keys, if there is alot of variables parsed this
   * might In most cases you aren't going to be dynamically finding keys.
   * @returns vector<string> list of keys
   */
  vector<string> get_keys() {
    vector<string> r;
    for (const auto &[key, value] : options) {
      r.push_back(key);
    }
    return r;
  }

  /**
   * @brief This parses main(c,arg) command line argument arrays. it will grab
   * -f=as and put it in the options as f -f without an equals are treated as
   * flags, same as -flag, these are easy to check for and have an O(log(n))
   * lookup. Where as getting specific keys will have the same lookup but are
   * stored in a map Typically called explicitly but can be called after
   * construction
   * @param c int argc
   * @param *v[] **argv
   */
  void parse(int c, char *v[]) {
    string prev = "";
    string_ops sops;
    int x = 0;
    while (x < c) {
      // std::cout << "-------------------->x=" << x << " c=" << c <<
      // "-------------------------" << std::endl;
      string str(sops.trim(v[x]));
      string key;
      string value;
      size_t flag = str.find_first_of('-', 0);
      size_t assigner = str.find_first_of('=', 0);
      size_t space = str.find_first_of(' ', 1);
      if ((flag == 0 && (space != str.npos || assigner != str.npos)) ||
          (flag == 0 && x + 1 < c && v[x + 1][0] != '-')) {
        int t = x;
        // std::cout << "f-"<<flag<<" a-"<<assigner << " s-" << space <<
        // std::endl;
        //  if there is an equal sign in the string split it and copy the values
        if (assigner != str.npos) {
          key = str.substr(1, assigner - 1);
          x++;
          std::string val = str.substr(assigner + 1, str.size());
          string_ops::trim(&val);
          string_ops::strip_qoutes(&val);
          value.append(val);

        } else if (space != str.npos) {
          key = str.substr(1, space - 1);
          x++;
          std::string val = str.substr(space, str.size());
          string_ops::trim(&val);
          string_ops::strip_qoutes(&val);
          value.append(val);
        }
        if (space == str.npos && assigner == str.npos && str[0] == '-') {
          key = str;
        }
        sops.trim(&key);
        while (key[0] == '-') {
          key.erase(0, 1);
        }
        // std::cout << "---->" << key << "<--->" << value << "<----While" <<
        // std::endl;
        while (t + 1 < c && v[t + 1][0] != '-') {
          value.append(" ");
          value.append(sops.strip_qoutes(sops.trim(v[t + 1])));
          t++;
        }

        // std::cout << "---->" << key << "<--->" << value << "<----if" <<
        // std::endl;
        /*if (t + 1 == c - 1 && v[t + 1][0] != '-') {
          value.append(" ");
          value.append(sops.strip_qoutes(sops.trim(v[t + 1])));
          t++;
        }*/
        x = t;
        if (!options.contains(key)) {
          options.emplace(string_ops::trim(key), string_ops::trim(value));
          options_count.emplace(key, 1);
          // std::cout << "opcount: " << options.size() <<  " size: " <<
          // options.size() << "---->" << key << "<--->" << value <<
          //"<----does!contain" << std::endl;
          x++;
          continue;
        }
        if (allow_multi && options.contains(key)) {
          options.emplace(string_ops::trim(key), string_ops::trim(value));
          options_count[key] = options_count[key] + 1;
          // std::cout << "opcount: " << options.size() <<  " size: " <<
          // options.size() << "---->" << key << "<--->" << value <<
          //  "<----does!contain" << std::endl;
          x++;
          continue;
        } else {
          throw security_exception("allow identicle keys _allow_multi is set "
                                   "to false duplicate key");
        }
        // std::cout << "--->" << options.size() << "<---" << std::endl;

      } else {
        if (flag != str.npos) {
          while (str[0] == '-') {
            str.erase(0, 1);
          }
          flags.insert(str);
        }
      }
      x++;
    }
  }

  /**
   * @brief Checks to see if the key exists before accessing it.
   * @param key to check
   * @returns bool
   * */
  bool has_key(string key) { return options.find(key) != options.end(); }
  /**
   * @brief returns true if the flag exists
   * @param flag to check
   * @returns bool
   */
  bool has_flag(string flag) { return flags.contains(flag); }
  /**
   * @brief Returns the value of the key pair in string form
   * @param key to return the value of
   * @returns string value of the key pair
   */
  string get_string(string key) {
    if (auto v = options.find(key); v != options.end()) {
      return v->second;
    } else {
      throw key_value_exception((const string)NOKEY + ": " + key);
    }
  }
  /**
   * @brief returns the keys of the restricted list
   * this can be done with allow_restrictions
   * but accessing them requires the flag
   * @returns vector<string> list of restricted keys
   */
  vector<string> get_restricted_keys() {
    vector<string> r;
    for (const auto &[key, value] : restricted) {
      r.push_back(key);
    }
    return r;
  }
  vector<std::string> get_strings(std::string key) {
    vector<std::string> retvec;
    auto range = options.equal_range(key);
    for (auto &i = range.first; i != range.second; i++) {
      if (i->first == key) {
        retvec.push_back(i->second);
      }
    }
    return retvec;
  }
  /**
   * @brief Restricted options are set when during parsing the variable or
   * variable name doesn't meet the regex this returns a restricted variable
   * only if allow restricted is enabled. considering allowing for some
   * authorization check
   * @param key (string)
   * @return string of the restricted value
   */
  string get_restricted_string(string key) {
    if (allow_restricted) {
      if (auto v = restricted.find(key); v != restricted.end()) {
        return v->second;
      }
    } else {
      throw security_exception("Allow_restricted is set to false by default, "
                               "enable it to use restricted");
    }
    std::string msg = NOKEY;
    msg.append(": " + key);
    throw key_value_exception(msg);
  }

  /**
   * @brief Gets an option or variable by key but attempts to return it as an
   * int type. throws key_value_exception if the key does not exist or invalid
   * argument NaN
   * @param key key of the int
   * @returns int64_t longlong
   */
  int64_t get_longlong(string key) {
    if (auto v = options.find(key); v != options.end()) {
      try {
        int64_t r = stoll(v->second);
        return r;
      } catch (exception &e) {
        throw key_value_exception(NAN);
      }
    } else {
      std::string msg = NOKEY;
      msg.append(": " + key);

      throw key_value_exception(msg);
    }
  }

  /**
   * @brief gets long option if NaN or key does not exist throws and exception
   * @param key of the int
   * @return int32 resulting number
   */
  int32_t get_long(string key) {
    if (auto v = options.find(key); v != options.end()) {
      try {
        int32_t r = stol(v->second);
        return r;
      } catch (exception &e) {
        throw key_value_exception(NAN);
      }
    } else {
      throw key_value_exception((const string)NOKEY + ": " + key);
    }
  }
  /**
   * @brief gets an int
   * @param key string
   * return int32_t return value
   */
  int32_t get_int(string key) {
    if (auto v = options.find(key); v != options.end()) {
      try {
        int r = stoi(v->second);

        return r;
      } catch (exception &e) {
        throw key_value_exception(NAN);
      }
    } else {
      throw key_value_exception((const string)NOKEY + ": " + key);
    }
  }
  int16_t key_count(std::string key) { return get_strings(key).size(); }
  /**
   * @brief gets double option if NaN or key does not exist throws and exception
   * @param key string
   * @returns double value
   */
  double get_double(string key) {
    if (auto v = options.find(key); v != options.end()) {
      try {
        double r = stod(v->second);
        return r;
      } catch (exception &e) {
        throw key_value_exception(NAN);
      }
    } else {
      throw key_value_exception((const string)NOKEY + ": " + key);
    }
  }

  /**
   * @brief gets float option if NaN or key does not exist throws and exception
   * @param key string
   * @return float value
   */
  float get_float(string key) {
    if (auto v = options.find(key); v != options.end()) {
      try {
        float r = stof(v->second);
        return r;
      } catch (exception &e) {
        throw key_value_exception(NAN);
      }
    } else {
      throw key_value_exception((const string)NOKEY + ": " + key);
    }
  }
};
} // namespace std
#pragma GCC diagnostic pop
#endif //_CONFIG_PARSER_HPP //NOLINT
