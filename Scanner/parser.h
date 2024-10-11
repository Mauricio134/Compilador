#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "../Scanner/reservadas.cpp"


using namespace std;

extern size_t currentTokenIndex; // Índice actual en la lista de tokens
extern Token currentToken;



void error(const string &mensaje);
void nextToken();
bool match(TokenType expected);
void Program();
void Declaration();
void ProgramPrime();
void VarDecl();
void Function();
void Expression();
void Type();
void VarDeclTail();
void Expression();
void Params();
void StmrList();
void TypePrime();
void ParamsTail();
void PrintStmt();
void Statement();
void StmtListPrime();
void StmtList();
bool nextTokenIsStatement();
void ForStmt();
void ReturnStmt();
void ExprStmt();
void ExprStmt();
void ExprList();
void OrExpr();
void AndExpr();
void OrExprPrime();
void EqExpr();
void AndExprPrime();
void EqExprPrime();
void RelExpr();
void Expr();
void RelExprPrime();
void Term();
void ExprPrime();
void Unary();
void TermPrime();
void Factor();
void Primary();
void FactorPrime();
void PrimaryTail();