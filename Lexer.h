#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct token {
    string lexeme;
    string type;
};

bool tokenizeFile(const string& filename, vector<token>& tokens);

#endif