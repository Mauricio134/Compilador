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

map<TokenType, string> tokenMap = {
    {TOKEN_ID, "ID"}, {TOKEN_DECLAR, "DECLARACION"}, {TOKEN_ASIG, "ASIGNACION"}, {TOKEN_DELIM_SC, "DELIMITADOR_PUNTO_Y_COMA"},
    {TOKEN_DELIM_B_O, "DELIMITADOR_CORCHETE_ABIERTO"}, {TOKEN_DELIM_B_C, "DELIMITADOR_CORCHETE_CERRADO"}, {TOKEN_DELIM_P_O, "DELIMITADOR_PARENTESI_ABIERTO"},
    {TOKEN_DELIM_P_C, "DELIMITADOR_PARENTESIS_CERRADO"}, {TOKEN_OPER_SUM, "SUMA"}, {TOKEN_OPER_REST, "RESTA"},
    {TOKEN_OPER_EX, "EXCLUSION"}, {TOKEN_OPER_EXP, "EXPONENCIAL"}, {TOKEN_OPER_MUL, "MULTIPLICACION"},
    {TOKEN_OPER_DIV, "DIVISION"}, {TOKEN_OPER_MOD, "MODULO"}, {TOKEN_OPER_MENOR, "MENOR"},
    {TOKEN_OPER_MAYOR, "MAYOR"}, {TOKEN_OPER_S_SUMA, "SUPER_SUMA"}, {TOKEN_OPER_S_RESTA, "SUPER_RESTA"},
    {TOKEN_OPER_MAY_E, "MAYOR_IGUAL"}, {TOKEN_OPER_MEN_E, "MENOR_IGUAL"}, {TOKEN_OPER_COMP_E, "IGUAL"},
    {TOKEN_OPER_COMP_D, "OPER_COMP_D"}, {TOKEN_OPER_AND, "OPER_AND"}, {TOKEN_OPER_OR, "OPER_OR"},
    {TOKEN_COMEN, "COMENTARIO"}, {TOKEN_COMEN_OPEN, "COMENTARIO_ABIERTO"}, {TOKEN_COMEN_CLOSE, "COMENTARIO_CERRADO"},
    {TOKEN_UNKNOWN, "DESCONOCIDO"}, {TOKEN_EOF, "FINAL_DE_LINEA"}, {TOKEN_INTEGER, "INTEGER"},
    {TOKEN_LL_O, "LLAVE_ABIERTA"}, {TOKEN_LL_C, "LLAVE_CERRADA"}, {TOKEN_COMA, "COMA"}, {TOKEN_ARRAY, "ARRAY"},
    {TOKEN_BOOLEAN, "BOOLEAN"}, {TOKEN_CHAR, "CHARACTER"}, {TOKEN_ELSE, "ELSE"}, {TOKEN_FALSE, "FALSE"},
    {TOKEN_FOR, "FOR"}, {TOKEN_FUNCTION, "FUNCTION"}, {TOKEN_IF, "IF"}, {TOKEN_PRINT, "PRINT"},
    {TOKEN_RETURN, "RETURN"}, {TOKEN_STRING, "STRING"}, {TOKEN_TRUE, "TRUE"}, {TOKEN_VOID, "VOID"},
    {TOKEN_WHILE, "WHILE"}
};



bool reservedWord(string word){
    if(reservadas.count(word) == true) return true;
    return false;
}