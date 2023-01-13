/*
 string-utils.cpp     MindForger thinking notebook

 Copyright (C) 2016-2023 Martin Dvorak <martin.dvorak@mindforger.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "string_utils.h"

#include <cassert>

using namespace std;

namespace m8r {

bool stringStartsWith(const char* s, const char* prefix)
{
    assert(s);
    assert(prefix);

    size_t prefixLng = strlen(prefix), sLng = strlen(s);
    return sLng < prefixLng ? false : strncmp(prefix, s, prefixLng) == 0;
}

bool stringStartsWith(const string& s, const string& prefix)
{
    return stringStartsWith(s.c_str(), prefix.c_str());
}

bool stringStartsWith(const string& s, const char* prefix)
{
    return stringStartsWith(s.c_str(), prefix);
}

bool stringEndsWith(const string& s, const string& suffix)
{
    if(suffix.size() > s.size()) {
        return false;
    }
    return equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

bool stringEndsWith(const char* s, const char* suffix)
{
    if (!s || !suffix) {
        return false;
    }
    size_t sLng = strlen(s);
    size_t suffixLng = strlen(suffix);
    if (suffixLng >  sLng) {
        return false;
    }
    return strncmp(s + sLng - suffixLng, suffix, suffixLng) == 0;
}

bool stringEndsWith(const string& s, const char* suffix)
{
    if (!s.c_str() || !suffix) {
        return false;
    }
    size_t sLng = strlen(s.c_str());
    size_t suffixLng = strlen(suffix);
    if (suffixLng >  sLng) {
        return false;
    }
    return strncmp(s.c_str() + sLng - suffixLng, suffix, suffixLng) == 0;
}

/**
 * @brief Split spring using delimiter and return array of tokens.
 *
 * This function is outline metadata parser optimized - base size 15
 * and increment 5.
 */
char** stringSplit(const char* s, const char delimiter) {
    return stringSplit(s, delimiter, 15, 5);
}

/**
 * @brief Split spring using delimiter and return array of tokens.
 * @return Array of pointers to split items.
 */
char** stringSplit(
        const char* s,
        const char delimiter,
        u_int16_t resultBaseSize,
        u_int16_t resultIncSize)
{
    // IMPROVE remove if not needed
    UNUSED_ARG(resultIncSize);

    char **result;
    result = new char*[static_cast<unsigned int>(resultBaseSize)];
    const char delim[2] = { delimiter, 0};

    if(result) {
        size_t i  = 0;
        char* offset{};
#ifdef _WIN32
        char* copy = _strdup(s);
        char* token = strtok_s(copy, delim, &offset);
#else
        char* copy = strdup(s);
        char* token = strtok_r(copy, delim, &offset);
#endif
        while(token) {
            // strdup() on new operator
            size_t len = strlen(token);
            if(len) {
                result[i] = new char[len+1];
                for(size_t c=0; c<=len; c++) result[i][c]=token[c];
            } else {
                result[i] = nullptr;
            }
            i++;
#ifdef _WIN32
        token = strtok_s(nullptr, delim, &offset);
#else
        token = strtok_r(nullptr, delim, &offset);
#endif
            // TODO implement auto increase result
            assert(i <= static_cast<size_t>(resultBaseSize));
        }
        result[i] = nullptr;
        free(copy);
    }

    return result;
}

vector<string> stringSplit(const string s, const string regexDelimiter)
{
    regex regexz(regexDelimiter);
    vector<string> list(
        sregex_token_iterator(
            s.begin(), s.end(), regexz, -1
        ),
       sregex_token_iterator()
    );
    return list;
}

/**
 * @brief Normalize a string to NCName.
 *
 * @param name      the NCName
 * @param quoteChar the quote character
 * @return the normalized string
 */
string normalizeToNcName(string name, char quoteChar) {
    string result = name;
    if(!result.empty()) {
        if(!isalnum(result[0], locale())) {
            result.insert(0, 1, '_');
        }
        for(size_t i=0; i<result.size(); i++) {
            if(!isalnum(result[i],locale())) {
                result[i] = quoteChar;
            }
        }
    }
    return result;
}

void toString(const std::vector<std::string*>& ss, std::string& os)
{
    if(!ss.empty()) {
        for(std::string* s:ss) {
            os += *s;
            os += "\n";
        }
    }
}

void replaceAll(const std::string& old_s, const std::string& new_s, std::string& s)
{
    size_t from = 0;
    while((from = s.find(old_s, from)) != std::string::npos) {
             s.replace(from, old_s.length(), new_s);
             from += new_s.length();
    }
}

} /* namespace */
