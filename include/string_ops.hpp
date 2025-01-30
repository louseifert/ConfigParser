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

#ifndef _STRING_OPS_HPP                                // NOLINT
#define _STRING_OPS_HPP                                // NOLINT
#include <cassert>                                     //NOLINT
#include <exception>                                   // NOLINT
#include <map>                                         // NOLINT
#include <sstream>                                     // NOLINT
#include <vector>                                      // NOLINT
#pragma GCC diagnostic push                            // NOLINT
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized" // NOLINT
#include <ParserExceptions.hpp>                        // NOLINT
#include <algorithm>                                   // NOLINT
#include <cctype>                                      // NOLINT
#include <splitstring.hpp>                             // NOLINT
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

/**
 * @class string_ops
 * @brief string operations
 * String Helper and processing class, string_ops
 * contains functions that are in plenty of other libraries specifically
 * written for use with this library.
 */
class string_ops {
 private:
  regex re;
  regex flag;
  std::string value;

 public:
  /**
   * @brief sets the default regex for the class
   * this will supercede regex it is created with.
   * TODO(lou):remove this?
   */
  void set_default_regex() {
    regex re(STROPVARS);
    regex flag(STROPFLAGS);
    this->re = re;
  }

 public:
  /**
   * @brief Default constructor that sets the default pattern to match against
   */
  string_ops() : re(STROPVARS), flag(STROPFLAGS) {}

  /**
   * @brief Constructor that accepts a string pattern to match against
   * @param option_pattern regex string for options
   * @param flag_pattern regex string for flags
   */
  string_ops(string option_pattern, string flag_pattern)
      : re(option_pattern), flag(flag_pattern) {}
  /**
   * @brief Constructor that accepts a string for string ops to operate on
   * @param string the principle string
   */
  explicit string_ops(std::string principle) : re(STROPVARS), flag(STROPFLAGS) {
    value = principle;
  }
  /**
   * @brief Regex match function accepts a string pointer and a regex pattern
   * and returns true if the pattern matches standard alphanumeric
   * or what ever is specified when the string_ops object is created.
   * @param *s pointer to a string
   * @return bool
   */
  bool strip_check(string *s) {
    string_ops::trim(s);
    string_ops::strip_qoutes(s);
    return default_match(*s);
  }

  /**
   * @brief Regex match function accepts a string and a regex pattern
   * and returns true if the pattern matches the string.
   * @param s string
   * @return bool
   */
  bool default_match(const string &s) { return regex_match(s, re); }

  /**
   * @brief Right Trim Function accepts a pointer to a string;
   * @param *s pointer to a string
   */
  static void rtrim(string *s) {
    while (isspace(s->at(s->size() - 1))) {
      s->pop_back();
    }
  }
  /**
   * @brief Right Trim Function accepts a pointer to a string;
   */
  void rtrim() {
    while (isspace(value.at(value.size() - 1))) {
      value.pop_back();
    }
  }
  /**
   *
   * @brief Right trim function accepts a string and returns a trimmed string
   * consider using the pointer version for performance reasons.
   * @param s string
   * @return string
   */
  static string rtrim(string s) {
    rtrim(&s);
    return s;
  }

  /**
   * @brief Left trim function accepts a string pointer operated on the string
   * in place
   * @param *s pointer to a string
   */
  static void ltrim(string *s) {
    uint32_t pos = 0;
    while (isspace(s->at(pos)) && pos < s->size()) {
      pos++;
    }
    s->erase(0, pos);
  }
  /**
   * @brief Left trim function accepts a string pointer operated on the string
   * in place
   * operates on the internal value
   */
  void ltrim() {
    uint32_t pos = 0;
    while (isspace(value.at(pos)) && pos < value.size()) {
      pos++;
    }
    value.erase(0, pos);
  }

  /**
   * @brief Left trim function accepts a string and returns a trimmed string
   * consider using the pointer version of this function for performance
   * reasons.
   * @param s string
   * @return string
   */
  static string ltrim(string s) {
    ltrim(&s);
    return s;
  }

  /**
   * @brief Trim function accepts a string pointer and trims the string in place
   * @param *s pointer to a string
   */
  static void trim(string *s) {
    rtrim(s);
    ltrim(s);
  }
  /**
   * @brief Trim function accepts a string pointer and trims the string in place
   * @param *s pointer to a string
   */
  void trim() {
    rtrim();
    ltrim();
  }

  /**
   * @brief Trim function accepts a string and returns a trimmed string
   * consider using the pointer version of this function for performance
   * reasons.
   * @param s string
   * @returns string
   */

  static string trim(string s) {
    trim(&s);
    return s;
  }

  /**
   *strips qoutes internal value
   *
   */
  void strip_qoutes() { strip_qoutes(&value); }

  /**
   * @brief Strips one set of qoutes on both ends of the variable
   * @param *s pointer to a string
   */
  static void strip_qoutes(string *s) {
    if (s->empty()) {
      return;
    }
    if ((s->at(0) == '"' && s->at(s->length() - 1) == '"') ||
        (s->at(0) == '\'' && s->at(s->length() - 1) == '\'')) {
      s->erase(0, 1);
      s->pop_back();
    }
  }

  /**
   * @brief Splits a string based on the delimiter given
   * @param string delimiter the character or string to split on it is removed
   * @param string principle is the string being operated on
   */
  static vector<std::string> split(std::string principle, char delimiter) {
    std::stringstream ss(principle);
    vector<std::string> rvector;
    std::string temp;
    if (principle.find(delimiter) == principle.npos) {
      rvector.push_back(principle);
      return rvector;
    }

    while (std::getline(ss, temp, delimiter)) {
      rvector.push_back(temp);
    }
    return rvector;
  }
  static std::string between_first_of(std::string p, std::string st,
                                      std::string en) {
    return p.substr(p.find(st) + st.size(), p.find(en));
  }

  std::string between_first_of(std::string a, std::string b) {
    return between_first_of(value, a, b);
  }

  static vector<std::string> split(std::string *p, std::string *d) {
    return splitstring(p, d);
  }
  /**
   * @brief for immutable form, pointer ops operat on the principle
   * @param give a copy to make sure the form is not mutable
   * @param delimiter
   */
  static vector<std::string> split(std::string principle, string delimiter) {
    return splitstring(&principle, &delimiter);
  }
  /**
   * @brief returns a vector of strings split member value from a delimiter of
   * size 1-size_t
   * @param string* delimiter, a pointer to a delimiter
   */
  vector<std::string> split(std::string *delimiter) {
    return splitstring(&value, delimiter);
  }
  /**
   * @brief returns a vector of strings from a split member by delimiter
   * @param delimiter to split
   */
  vector<std::string> split(std::string delimiter) {
    return splitstring(&value, &delimiter);
  }
  /**
   * @brief returns the string between two similar or dissimilar strings
   * from the first of start to the last of end so in a <div><div></div></div>
   * if you provided a start of <div> and and end of </div> you would get
   * the middle <div></div>  It does not parse xml fully, it would also grab the
   * part of a SQL statment if you had it grab between BEGIN and END;
   * @param std::string* principle, the string to search and find the tags in
   * @param std::string* start the value that it finds the first of
   * @param std::string* value it find the last of
   */
  static std::string string_between(std::string *principle, std::string *start,
                                    std::string *end) {
    return str_between(principle, start, end);
  }
  /**
   * @brief returns the string between two similar or dissimilar strings
   * from the first of start to the last of end so in a <div><div></div></div>
   * if you provided a start of <div> and and end of </div> you would get
   * the middle <div></div>  It does not parse xml fully, it would also grab the
   * part of a SQL statment if you had it grab between BEGIN and END;
   * @param std::string* principle, the string to search and find the tags in
   * @param std::string* start the value that it finds the first of
   * @param std::string* value it find the last of
   */

  std::string string_between(std::string *start, std::string *end) {
    return str_between(&value, start, end);
  }
  /**
   * @brief returns the string between two similar or dissimilar strings
   * from the first of start to the last of end so in a <div><div></div></div>
   * if you provided a start of <div> and and end of </div> you would get
   * the middle <div></div>  It does not parse xml fully, it would also grab the
   * part of a SQL statment if you had it grab between BEGIN and END;
   * @param std::string* principle, the string to search and find the tags in
   * @param std::string* start the value that it finds the first of
   * @param std::string* value it find the last of
   */
  std::string string_between(std::string start, std::string end) {
    return str_between(&value, &start, &end);
  }
  /**
   * @brief peforms to_upper on the internal value
   */
  void tupper() { tupper(&value); }
  /**
   * @brief performs to_lower on the internal value
   */
  void tlower() { tlower(&value); }
  /**
   * @brief performs to upper on the string provided
   * @param *stirng pointer to the string
   */
  static void tupper(std::string *str) {
    std::transform(str->begin(), str->end(), str->begin(), ::toupper);
  }
  /**
   * @brief performs to_lower on the string provided
   */
  static void tlower(std::string *str) {
    std::transform(str->begin(), str->end(), str->begin(), ::tolower);
  }
  /**
   * @brief returns a copy of the string in lower
   * @param string
   * @return string
   */
  static std::string tlower(std::string str) {
    tlower(&str);
    return str;
  }
  /**
   * @brief returns a copy of the string in upper
   * @param string input
   * @return string in upper
   */
  static std::string tupper(std::string str) {
    tupper(&str);
    return str;
  }
  /**
   * @brief Strips one set of qoutes on both ends of the string
   * @param s string
   * @return string
   */
  static string strip_qoutes(string s) {
    strip_qoutes(&s);
    return s;
  }
};
} // namespace std
#endif // INCLUDE_STRING_OPS_HPP_ //NOLINT
