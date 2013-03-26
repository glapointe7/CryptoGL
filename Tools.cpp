/* Modifié par Gabriel Lapointe
* le 25 mars 2013
*/

#include "Tools.h"
#include <algorithm>
#include <bitset>
#include <sstream>
#include <iostream>

// Convertit chaque Majuscule et minuscule en un symbole quelconque du code ASCII.
// symbol doit contenir 2 caract�res : 0 = MAJUSCULE  1 = minuscule.
void convertMajMinToSymbol(string &text, const string symbol)
{
    // On remplace chaque caractère en majuscule par symbol[0].
    replace_if(text.begin(), text.end(), [](char c)
    {
        return (isupper(c));
    }, symbol[0]);
    
     // On remplace chaque caractère en minuscule par symbol[1].
    replace_if(text.begin(), text.end(), [](char c)
    {
        return (islower(c));
    }, symbol[1]);
}

// Retourne PGCD(a,b).
uint_fast32_t getPGCD(uint_fast32_t a, uint_fast32_t b)
{
    if(b == 0)
        return a;

    return getPGCD(b, a % b);
}

// On enl�ve les lettres doublons de la clef.
string removeRepeatedLetters(const string key)
{   
    unsigned short keylen = key.length();
    string result(key);

    for(unsigned short i = 0; i < keylen; i++)
    {
        for(unsigned short j = i + 1; j < keylen; j++)
        {
            if(result[i] == result[j])
            {
                result.erase(j, 1);
                keylen--;
            }
        }
    }

    return result;
}

// Construction d'une grille carrée (rows X rows) qui servira pour l'encodage
// et décodage de cipher demandant une grille de chiffrement.
vector<string> createGrid(const string chars, const unsigned char rows)
{
    string new_alpha(removeRepeatedLetters(chars));
    vector<string> grid;

    // S�paration de la chaine pour former une grille carrée (rows X rows) de
    // caractères.
    for(unsigned char i = 0; i < rows; i++)
    {
        grid.push_back(new_alpha.substr(i * rows, rows));
    }

    return grid;
}

unsigned int appendChar(string &text, const char c)
{
    unsigned int i = 0;
    if((text.length() % 2) != 0)
    {
        text += c;
        i++;
    }
    
    return i;
}

// Remplace dans "text" chaque occurence du caract�re "letter" par "rletter".
void replaceChar(string &text, const char letter, const char rletter)
{
    replace_if (text.begin(), text.end(), [letter](char c)
    {
        return (c == letter);
    }, rletter);
}

// Effectue, selon 'op', un XOR, NOT, AND, OR et retourne le r�sultat. 
string getBinary(const string &bin_text, const string &bin_to_compare, const unsigned char op)
{
	const size_t size = 500;
	bitset<size> Text(bin_text);
	bitset<size> Comp(bin_to_compare);
	bitset<size> res;

	switch(op)
	{
		// OR
		case '|': res = Text | Comp; break;
		// AND
		case '&': res = Text & Comp; break;
		// XOR
		case '^': res = Text ^ Comp; break;
		// NOT
		case '!': res = Text.flip(); break;
	}

	return res.to_string<char,char_traits<char>,allocator<char> >();
}

// D�cale les bits � gauche ou � droite selon 'is_right' de 'shift' positions.
string getBinaryShifter(const string &bin_text, const unsigned int shift, bool is_right = true)
{
	const size_t size = 500;
	bitset<size> Text(bin_text);
	bitset<size> res;

	if(is_right)
		res = Text >> shift;
	else
		res = Text << shift;

	return res.to_string<char,char_traits<char>,allocator<char> >();
}

// S�pare une cha�ne de caract�res par l'espace et met chaque mot dans un vecteur.
vector<string> split(string &text)
{
    istringstream iss(text);
    vector<string> split_text;

    do
    {
        string word;
        iss >> word;
	split_text.push_back(word);
    } while(iss);
    split_text.pop_back();

    return split_text;
}

// Retourne les caract�res pris � chaque 'step' position.
string getStepCharacters(string &text, unsigned short step)
{
    string message = "";
    unsigned long text_len = text.length();

    for(unsigned long i = step; i <= text_len - step; i += step)
    {
        message += text[i];
    }

    return message;
}