#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "../Scanner/reservadas.cpp"
#include "../Scanner/astree.hpp"


using namespace std;

extern size_t currentTokenIndex; // Índice actual en la lista de tokens
extern Token currentToken;



void error(const string &mensaje);
void nextToken();
bool match(TokenType expected);
void Program();
/*std::shared_ptr<ASTNode> Declaration();
std::shared_ptr<ASTNode> ProgramPrime();
std::shared_ptr<ASTNode> VarDecl();
std::shared_ptr<ASTNode> Function();
std::shared_ptr<ASTNode> Type();
std::shared_ptr<ASTNode> VarDeclTail();
std::shared_ptr<ASTNode> Expression();
std::shared_ptr<ASTNode> Params();
std::shared_ptr<ASTNode> StmrList();
std::shared_ptr<ASTNode> TypePrime();
void ParamsTail(std::shared_ptr<ASTNode> paramsNode);
std::shared_ptr<ASTNode> PrintStmt();
std::shared_ptr<ASTNode> Statement();
void StmtListPrime(std::shared_ptr<ASTNode> stmtListNode);
std::shared_ptr<ASTNode> StmtList();
bool nextTokenIsStatement();
std::shared_ptr<ASTNode> ForStmt();
std::shared_ptr<ASTNode> ReturnStmt();
std::shared_ptr<ASTNode> ExprStmt();
std::shared_ptr<ASTNode> ExprList();
std::shared_ptr<ASTNode> OrExpr();
std::shared_ptr<ASTNode> AndExpr();
std::shared_ptr<ASTNode> OrExprPrime(std::shared_ptr<ASTNode> leftNode);
std::shared_ptr<ASTNode> EqExpr();
std::shared_ptr<ASTNode> AndExprPrime(std::shared_ptr<ASTNode> leftNode) ;
std::shared_ptr<ASTNode> EqExprPrime(std::shared_ptr<ASTNode> leftNode);
std::shared_ptr<ASTNode> RelExpr();
std::shared_ptr<ASTNode> Expr();
std::shared_ptr<ASTNode> RelExprPrime(std::shared_ptr<ASTNode> leftNode);
std::shared_ptr<ASTNode> Term();
std::shared_ptr<ASTNode> ExprPrime(std::shared_ptr<ASTNode> leftNode);
std::shared_ptr<ASTNode> Unary();
std::shared_ptr<ASTNode> TermPrime(std::shared_ptr<ASTNode> leftNode);
std::shared_ptr<ASTNode> Factor();
std::shared_ptr<ASTNode> Primary();
std::shared_ptr<ASTNode> FactorPrime(std::shared_ptr<ASTNode> leftNode);
std::shared_ptr<ASTNode> PrimaryTail(std::shared_ptr<ASTNode> idNode);
std::shared_ptr<ASTNode> IfStmt();
std::shared_ptr<ASTNode> ElseIfList();
std::shared_ptr<ASTNode> extra();*/