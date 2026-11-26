#include <iostream>
#include <fstream>
#include "Lexer.h"

string operatorType(string op)
{
    if (op == "=")
    {
        return "Assignment Operator";
    }
    else if (op == "+" || op == "-" || op == "*" ||
        op == "/" || op == "%")
    {
        return "Arithmetic Operator";
    }
    else if (op == "==" || op == "!=" || op == "<" ||
        op == ">" || op == "<=" || op == ">=")
    {
        return "Relational Operator";
    }
    else if (op == "&&" || op == "||" || op == "!")
    {
        return "Logical Operator";
    }
    else
    {
        return "Unknown Operator";
    }
}

bool isOperator(string op)
{
    if (operatorType(op) != "Unknown Operator")
    {
        return true;
    }

    return false;
}

bool isKeyword(string str)
{
    string keywords[] = { "let", "if", "else", "while", "func",
                          "return", "print", "read", "int", "float" };

    for (string keyword : keywords)
    {
        if (str == keyword)
        {
            return true;
        }
    }

    return false;
}

bool isDelimiter(string d)
{
    if (d == "(" || d == ")" || d == "{" || d == "}" ||
        d == "[" || d == "]" || d == ";" || d == "," || d == ":")
    {
        return true;
    }

    return false;
}

bool isIdentifier(string word)
{
    if (word == "" || isKeyword(word))
    {
        return false;
    }

    if (!((word[0] >= 'A' && word[0] <= 'Z') ||
        (word[0] >= 'a' && word[0] <= 'z') ||
        word[0] == '_'))
    {
        return false;
    }

    for (char c : word)
    {
        if (!((c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '_'))
        {
            return false;
        }
    }

    return true;
}

string intOrFloat(string str)
{
    if (str == "")
    {
        return "Unknown";
    }

    bool hasDecimal = false;

    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '.')
        {
            if (hasDecimal)
            {
                return "Unknown";
            }

            hasDecimal = true;
        }
        else if (str[i] < '0' || str[i] > '9')
        {
            return "Unknown";
        }
    }

    if (str[0] == '.' || str[str.length() - 1] == '.')
    {
        return "Unknown";
    }

    return hasDecimal ? "Float" : "Integer";
}

string lexemeType(string lexeme)
{
    if (isKeyword(lexeme))
    {
        return "Keyword";
    }
    else if (intOrFloat(lexeme) == "Integer")
    {
        return "Integer";
    }
    else if (intOrFloat(lexeme) == "Float")
    {
        return "Float";
    }
    else if (isIdentifier(lexeme))
    {
        return "Identifier";
    }
    else
    {
        return "Unknown";
    }
}

bool tokenizeFile(const string& filename, vector<token>& tokens)
{
    ifstream file(filename);

    if (!file)
    {
        return false;
    }

    string current = "";
    char ch;
    token t;

    while (file.get(ch))
    {
        if (ch == '#')
        {
            if (current != "")
            {
                t.lexeme = current;
                t.type = lexemeType(current);
                tokens.push_back(t);
                current = "";
            }

            string comment = "#";

            while (file.get(ch) && ch != '\n')
            {
                comment += ch;
            }

            t.lexeme = comment;
            t.type = "Comment";
            tokens.push_back(t);

            continue;
        }

        if (ch == ' ' || ch == '\t' || ch == '\n')
        {
            if (current != "")
            {
                t.lexeme = current;
                t.type = lexemeType(current);
                tokens.push_back(t);
                current = "";
            }

            continue;
        }

        string oneChar(1, ch);

        if (isDelimiter(oneChar))
        {
            if (current != "")
            {
                t.lexeme = current;
                t.type = lexemeType(current);
                tokens.push_back(t);
                current = "";
            }

            t.lexeme = oneChar;
            t.type = "Delimiter";
            tokens.push_back(t);

            continue;
        }

        if (isOperator(oneChar))
        {
            if (current != "")
            {
                t.lexeme = current;
                t.type = lexemeType(current);
                tokens.push_back(t);
                current = "";
            }

            string next = "";

            if (file.peek() != EOF)
            {
                char nextChar = file.peek();
                next = string(1, nextChar);
            }

            string twoChar = oneChar + next;

            if (isOperator(twoChar))
            {
                file.get(ch);

                t.lexeme = twoChar;
                t.type = operatorType(twoChar);
                tokens.push_back(t);
            }
            else
            {
                t.lexeme = oneChar;
                t.type = operatorType(oneChar);
                tokens.push_back(t);
            }

            continue;
        }

        current += ch;
    }

    if (current != "")
    {
        t.lexeme = current;
        t.type = lexemeType(current);
        tokens.push_back(t);
    }

    file.close();

    t.lexeme = "EOF";
    t.type = "EOF";
    tokens.push_back(t);

    return true;
}