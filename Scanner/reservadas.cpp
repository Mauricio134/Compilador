#pragma once
#include <iostream>
#include <set>
#include <map>

using namespace std;

set<string> reservadas = {"array" , "boolean" , "char" , "else" , "false" , "for" , "function" , "if" , "integer" , "print" , "return" , "string" , "true" , "void" , "while"};

set<string> elements = {
    ":", 
    "=", 
    ";",
    "[",
    "]",
    "(",
    ")",
    "+",
    "-",
    "!",
    "^",
    "*",
    "/",
    "%",
    "<",
    ">",
    "++",
    "--",
    ">=",
    "<=",
    "==",
    "!=",
    "&&",
    "||",
    "//",
    "/*",
    "*/"
};

map<string, string> token = {
    {"array", "ARRAY"},
    {"boolean", "BOOLEAN"},
    {"char", "CHARACTER"},
    {"else", "COND_ELSE"},
    {"false", "FALSE"},
    {"for", "LOOP_FOR"},
    {"function", "FUNCTION"},
    {"if", "COND_IF"},
    {"integer", "INTEGER"},
    {"print", "PRINT"},
    {"return", "RETURN"},
    {"string", "STRING"},
    {"true", "TRUE"},
    {"void", "VOID"},
    {"while", "LOOP_WHILE"},
    {":", "DECLAR"},
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
    {"/*", "COMEN_OPEN"},
    {"*/", "COMEN_CLOSED"}
};

bool reservedWord(string word){
    if(reservadas.count(word) == true) return true;
    return false;
}

bool elementos(string word){
    if(elements.count(word) == true) return true;
    return false;
}