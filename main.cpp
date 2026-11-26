#include <Lexer.h>

int main()
{
    vector<token> tokens;

    if (!tokenizeFile("source.txt", tokens))
    {
        cout << "Error, can't open this file" << endl;
        return 0;
    }

    cout << "Index:" << '\t' << "Lexemes:" << '\t' << "Tokens:" << endl;
    cout << "------------------------" << endl;

    for (int i = 0; i < tokens.size(); i++)
    {
        cout << i << '\t' << tokens[i].lexeme
            << '\t' << tokens[i].type << endl;
    }

    return 0;
}