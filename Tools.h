/* Modifié par Gabriel Lapointe
 * le 25 mars 2013
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

using namespace std;

// Prototypes pour Tools.cpp.
void convertMajMinToSymbol(string &text, const string symbol);
string removeRepeatedLetters(const string key);
void replaceChar(string &text, const char letter, const char rletter);
string getBinary(const string &bin_text, const string &bin_to_compare, const unsigned char op);
string getBinaryShifter(const string &bin_text, const unsigned int shift, bool is_right);
string convertBinaryToString(string &bin_text);
vector<string> split(string &text);
string getStepCharacters(string &text, unsigned short step);
vector<string> createGrid(const string chars, const unsigned char rows);
unsigned int appendChar(string &text, const char c);

// Fonctions mathématiques.
uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b);

#endif