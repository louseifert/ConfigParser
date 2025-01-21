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

#ifndef __SPLITSTRING__HPP // NOLINT
#define __SPLITSTRING__HPP // NOLINT
#include <string>
#include <vector>
/**
 * @brief splits a string by delimiter and returns an array
 * @param *string p for principle the subject of the split.
 * @param *string d for delimiter what to split by
 */
std::vector<std::string> splitstring(std::string *p, std::string *d) {
  std::vector<std::string> rvec;
  size_t last = 0;
  for (size_t i = 0; i < p->size(); i++) {
    if (p->at(i) != d->at(0)) {
      continue;
    } else {
      if (d->size() == 1) {
        rvec.push_back(p->substr(last, i - last));
        last = i + d->size();
        i = last;

        continue;
      } else {
        for (size_t j = 0; j < d->size(); j++) {
          if (j + i >= p->size()) {
            rvec.push_back(p->substr(last, p->size()));
          }
          if (p->at(i + j) != d->at(j)) {
            continue;
          } else {
            rvec.push_back(p->substr(last, i - last));
            last = i + d->size();
            i = i + d->size();
            continue;
          }
        }
      }
    }
  }
  if (last != p->size()) {
    rvec.push_back(p->substr(last, p->size() - last));
  }
  return rvec;
}
/**
 * @brief returns everything betwee the first of start and last of end.
 * @param *string p for principle the string to be searched and substr
 * @param *string start is the starting tag finds the first of.
 * @param *string finds the last of
 */
std::string str_between(std::string *p, std::string *start, std::string *end) {
  size_t s = p->find(*start);
  size_t e = p->find(*end);
  return p->substr(s + start->size(), e - s - start->size());
}
#endif // INCLUDE_SPLITSTRING_HPP_
