#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>

using namespace std;

int columna = 0;
int fila = 0;
string word;
const char * getcharcurrent = nullptr;
const char * peekcharcurrent = nullptr;

enum TokenType {
    TOKEN_ID, TOKEN_DECLAR, TOKEN_ASIG, TOKEN_DELIM_SC, TOKEN_DELIM_B_O,
    TOKEN_DELIM_B_C, TOKEN_DELIM_P_O, TOKEN_DELIM_P_C, TOKEN_OPER_SUM,
    TOKEN_OPER_REST, TOKEN_OPER_EX, TOKEN_OPER_EXP, TOKEN_OPER_MUL,
    TOKEN_OPER_DIV, TOKEN_OPER_MOD, TOKEN_OPER_MENOR, TOKEN_OPER_MAYOR,
    TOKEN_OPER_S_SUMA, TOKEN_OPER_S_RESTA, TOKEN_OPER_MAY_E, TOKEN_OPER_MEN_E,
    TOKEN_OPER_COMP_E, TOKEN_OPER_COMP_D, TOKEN_OPER_AND, TOKEN_OPER_OR,
    TOKEN_COMEN, TOKEN_COMEN_OPEN, TOKEN_COMEN_CLOSE, TOKEN_UNKNOWN, TOKEN_EOF,TOKEN_INTEGER,
    TOKEN_LL_O, TOKEN_LL_C, TOKEN_COMA,TOKEN_ARRAY, TOKEN_BOOLEAN,TOKEN_CHAR,TOKEN_ELSE,TOKEN_FALSE,TOKEN_FOR,TOKEN_FUNCTION,
    TOKEN_IF,TOKEN_PRINT,TOKEN_RETURN,TOKEN_STRING,TOKEN_TRUE,TOKEN_VOID,TOKEN_WHILE
};

class Token{
public:
    TokenType token;
    string value;
    int fila;
    int columna;
};

vector<Token> tokens;

set<string> reservadas = {"array" , "boolean" , "char" , "else" , "false" , "for" , "function" , "if" , "integer" , "print" , "return" , "string" , "true" , "void" , "while"};

map<string, TokenType> Mapa_tokens ={
    {"array", TOKEN_ARRAY},
    {"boolean", TOKEN_BOOLEAN},
    {"char", TOKEN_CHAR},
    {"else", TOKEN_ELSE},
    {"false", TOKEN_FALSE},
    {"for", TOKEN_FOR},
    {"function", TOKEN_FUNCTION},
    {"if", TOKEN_IF},
    {"integer", TOKEN_INTEGER},
    {"print", TOKEN_PRINT},
    {"return", TOKEN_RETURN},
    {"string", TOKEN_STRING},
    {"true", TOKEN_TRUE},
    {"void", TOKEN_VOID},
    {"while", TOKEN_WHILE}
};

/* {":", "DECLAR"},
    {"=", "ASSIG"},
    {";", "DELIM_SC"},
    {"[", "DELIM_B_O"},
    {"]", "DELIM_B_C"},
    {"(", "DELIM_P_O"},
    {")", "DELIM_P_C"},
    {"+", "OPER_SUM"},
    {"-", "OPER_REST"},
    {"!", "OPER_EX"},
    {"^", "OPER_EXP"},
    {"*", "OPER_MUL"},
    {"/", "OPER_DIV"},
    {"%", "OPER_MOD"},
    {"<", "OPER_MENOR"},
    {">", "OPER_MAYOR"},
    {"++", "OPER_S_SUMA"},
    {"--", "OPER_S_RESTA"},
    {">=", "OPER_MAY_E"},
    {"<=", "OPER_MEN_E"},
    {"==", "OPER_COMP_E"},
    {"!=", "OPER_COMP_D"},
    {"&&", "OPER_AND"},
    {"||", "OPER_OR"},
    {"//", "COMEN"},
    {"nn", "COMEN_OPEN"},
    {"/", "COMEN_CLOSED"}
*/

bool reservedWord(string word){
    if(reservadas.count(word) == true) return true;
    return false;
}