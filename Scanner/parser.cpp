#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include "../Scanner/reservadas.cpp"
#include "../Scanner/parser.h"
#include "../Scanner/astree.hpp"

size_t currentTokenIndex = 0; // Índice actual en la lista de tokens
Token currentToken;
bool error_bool=0;

using namespace std;

using ASTNode=ASTree::ASTNode<TokenType>;
auto syntaxTree = ASTNode::GetNewInstance(TokenType::TOKEN_PROGRAM, "HEAD_NODE");



std::shared_ptr<ASTNode> Declaration();
std::shared_ptr<ASTNode> ProgramPrime();
std::shared_ptr<ASTNode> VarDecl();
std::shared_ptr<ASTNode> Function();
std::shared_ptr<ASTNode> Type();
std::shared_ptr<ASTNode> VarDeclTail(std::shared_ptr<ASTNode> idNode);
std::shared_ptr<ASTNode> Expression();
std::shared_ptr<ASTNode> Params();
std::shared_ptr<ASTNode> StmrList();
std::shared_ptr<ASTNode> TypePrime(std::shared_ptr<ASTNode> baseTypeNode);
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
std::shared_ptr<ASTNode> extra();

Token auxiliar;


void error(const string &mensaje) {
    cout << "Error: " << mensaje << " en la línea " << currentToken.fila << ", columna " << currentToken.columna << ". Token encontrado: " << currentToken.value << endl;
    error_bool=1;
}

void sync() {
    set<TokenType> syncSet = { TOKEN_DELIM_SC };
    while (syncSet.find(currentToken.token) == syncSet.end() && currentToken.token != TOKEN_EOF) {
        nextToken();
    }
}


// Obtener el siguiente token
void nextToken() {
    //cout<<"Entro en la funcion";
    if (currentTokenIndex < tokens.size()) {
        currentToken = tokens[currentTokenIndex];
        currentTokenIndex++;
    } else {
        currentToken = {TOKEN_EOF, "", fila, columna}; // Asumimos que TOKEN_EOF es el fin del archivo
    }
}

// Verificar si el token actual es del tipo esperado
bool match(TokenType expected) {
    if (currentToken.token == expected) {
        auxiliar.value=currentToken.value;
        nextToken(); // Avanzamos al siguiente token si es correcto
        return true;
    }
    return false;
}


void Program() {
    auto declarationNode = Declaration();
    if (declarationNode != nullptr) {
        syntaxTree->addChild(declarationNode);
    }
    auto ProgramPrimeNode=ProgramPrime();
    if (ProgramPrimeNode!=nullptr){
        syntaxTree->addChild(ProgramPrimeNode);
    }
    if (!error_bool){
        cout<<"\nPROGRAMA ACEPTADO SIN ERRORES DE PARSING\n";
    }
    
}

std::shared_ptr<ASTNode> ProgramPrime() {
    auto programPrimeNode = ASTNode::GetNewInstance(TokenType::Default, "PROGRAM_PRIME");

    while (currentToken.token == TOKEN_VAR || currentToken.token == TOKEN_FUNCTION || currentToken.token == TOKEN_ASIG) {
        // Analizar una declaración y añadirla como hijo
        auto declarationNode = Declaration();
        if (declarationNode != nullptr) {
            programPrimeNode->addChild(declarationNode);
        }
    }

    // Si no hay declaraciones, devolvemos nullptr (epsilon)
    return (programPrimeNode->getChildsCount()==0) ? nullptr : programPrimeNode;
}


std::shared_ptr<ASTNode> Declaration() {
    if (match(TOKEN_VAR)) {
        return VarDecl();
    } else if (match(TOKEN_FUNCTION)) {
        return Function();
    } else {
        auto exprNode = Expression();
        if (!match(TOKEN_DELIM_SC)) { // Esperamos un ';'
            error("Se esperaba ';' después de la expresión.");
            sync();
        }

        return exprNode;
    }
}

std::shared_ptr<ASTNode> VarDecl() {
    auto varDeclNode = ASTNode::GetNewInstance(TokenType::Default, "VAR_DECL");

    // Procesamos el tipo de la variable y lo añadimos como hijo
    auto typeNode = Type();
    if (typeNode != nullptr) {
        varDeclNode->addChild(typeNode);
    }

    // Verificamos que haya un identificador
    if (!match(TOKEN_ID)) {
        error("Se esperaba un identificador después del tipo.");
        sync();
        return nullptr;
    }

    // Creamos un nodo para el identificador y lo añadimos
    auto idNode = ASTNode::GetNewInstance(TokenType::Default, auxiliar.value);
    varDeclNode->addChild(idNode);
    //nextToken();  // Avanzamos al siguiente token

    // Procesamos el tail de la declaración y obtenemos el nodo correspondiente
    auto varDeclTailNode = VarDeclTail(idNode);
    if (varDeclTailNode != nullptr) {
        varDeclNode->addChild(varDeclTailNode);
    }

    return varDeclNode;
}


std::shared_ptr<ASTNode> VarDeclTail(std::shared_ptr<ASTNode> idNode) {
    // Si hay un ';', la declaración es completa sin inicialización
    if (match(TOKEN_DELIM_SC)) {
        return nullptr;
    } 
    // Si hay una asignación '='
    else if (match(TOKEN_ASIG)) {
        // Creamos un nodo de asignación
        auto assignNode = ASTNode::GetNewInstance(TokenType::Default, "ASSIGN");

        // Añadimos el identificador como hijo izquierdo
        assignNode->addChild(idNode);

        // Procesamos la expresión de inicialización
        auto exprNode = Expression();
        if (exprNode != nullptr) {
            assignNode->addChild(exprNode);
        }

        // Verificamos que haya un ';' al final
        if (!match(TOKEN_DELIM_SC)) {
            error("Se esperaba ';' después de la inicialización.");
            sync();
        }

        return assignNode;
    } 
    // Error si no se encuentra ni ';' ni '='
    else {
        error("Se esperaba ';' o '=' en la declaración de la variable.");
        sync();
        return nullptr;
    }
}


std::shared_ptr<ASTNode> Function() {
    // Nodo principal para la función
    auto functionNode = ASTNode::GetNewInstance(TokenType::Default, "FUNCTION");

    // Procesamos el tipo de retorno y creamos un nodo para él
    auto returnTypeNode = Type();
    if (returnTypeNode != nullptr) {
        functionNode->addChild(returnTypeNode);
    }

    // Verificamos el identificador de la función
    if (!match(TOKEN_ID)) {
        error("Se esperaba un identificador para la función.");
        sync();
        return nullptr;
    }

    // Creamos un nodo para el identificador (nombre de la función)
    auto idNode = ASTNode::GetNewInstance(TokenType::Default, auxiliar.value);
    functionNode->addChild(idNode);

    // Verificamos el paréntesis de apertura '('
    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba '(' después del identificador de la función.");
        sync();
        return nullptr;
    }

    // Nodo para los parámetros
    auto paramsNode = Params();
    if (paramsNode != nullptr) {
        functionNode->addChild(paramsNode);
    }

    // Verificamos el paréntesis de cierre ')'
    if (!match(TOKEN_DELIM_P_C)) {
        error("Se esperaba ')' después de los parámetros de la función.");
        sync();
        return nullptr;
    }

    // Verificamos la llave de apertura '{'
    if (!match(TOKEN_LL_O)) {
        error("Se esperaba '{' al inicio del cuerpo de la función.");
        sync();
        return nullptr;
    }

    // Nodo para la lista de sentencias (cuerpo de la función)
    auto stmtListNode = StmtList();
    if (stmtListNode != nullptr) {
        functionNode->addChild(stmtListNode);
    }

    // Verificamos la llave de cierre '}'
    if (!match(TOKEN_LL_C)) {
        error("Se esperaba '}' al final del cuerpo de la función.");
        sync();
        return nullptr;
    }

    // Retornamos el nodo de la función para añadirlo al árbol sintáctico principal
    return functionNode;
}




std::shared_ptr<ASTNode> Type() {
    std::shared_ptr<ASTNode> typeNode = nullptr;

    if (match(TOKEN_INTEGER)) {
        typeNode = ASTNode::GetNewInstance(TokenType::Default, "integer");
        auto arrayNode = TypePrime(typeNode);
        if (arrayNode != nullptr) {
            return arrayNode;
        }
        return typeNode;
    } else if (match(TOKEN_BOOLEAN)) {
        typeNode = ASTNode::GetNewInstance(TokenType::Default, "boolean");
        auto arrayNode = TypePrime(typeNode);
        if (arrayNode != nullptr) {
            return arrayNode;
        }
        return typeNode;
    } else if (match(TOKEN_CHAR)) {
        typeNode = ASTNode::GetNewInstance(TokenType::Default, "char");
        auto arrayNode = TypePrime(typeNode);
        if (arrayNode != nullptr) {
            return arrayNode;
        }
        return typeNode;
    } else if (match(TOKEN_STRING)) {
        typeNode = ASTNode::GetNewInstance(TokenType::Default, "string");
        auto arrayNode = TypePrime(typeNode);
        if (arrayNode != nullptr) {
            return arrayNode;
        }
        return typeNode;
    } else if (match(TOKEN_VOID)) {
        return ASTNode::GetNewInstance(TokenType::Default, "void");
    } else {
        error("Se esperaba un tipo válido (integer, boolean, character, string, void).");
        sync();
        return nullptr;
    }
}

std::shared_ptr<ASTNode> TypePrime(std::shared_ptr<ASTNode> baseTypeNode) {
    // Verificamos si hay un corchete de apertura '['
    if (match(TOKEN_DELIM_B_O)) {
        // Creamos un nodo para el arreglo
        auto arrayNode = ASTNode::GetNewInstance(TokenType::Default, "array");

        // Añadimos el nodo base (tipo primitivo) como hijo del nodo arreglo
        arrayNode->addChild(baseTypeNode);

        // Verificamos si hay un corchete de cierre ']'
        if (!match(TOKEN_DELIM_B_C)) {
            error("Se esperaba ']' después del '['.");
            sync();
            return nullptr;
        }

        // Llamada recursiva para procesar arreglos múltiples (como int[][])
        return TypePrime(arrayNode);
    }

    // Si no hay corchetes, retornamos el nodo base (tipo simple)
    return baseTypeNode;
}



std::shared_ptr<ASTNode> Params() {
    // Creamos un nodo para la lista de parámetros
    auto paramsNode = ASTNode::GetNewInstance(TokenType::Default, "params");

    // Obtenemos el nodo del tipo
    auto typeNode = Type();
    if (typeNode == nullptr) {
        error("Error al procesar el tipo del parámetro.");
        return nullptr;
    }

    // Verificamos si hay un identificador (nombre del parámetro)
    if (!match(TOKEN_ID)) {
        error("Se esperaba un identificador después del tipo en los parámetros.");
        sync();
        return nullptr;
    }

    // Creamos un nodo para el parámetro y lo añadimos como hijo del nodo de parámetros
    auto paramNode = ASTNode::GetNewInstance(TokenType::Default, "param");
    paramNode->addChild(typeNode);
    paramNode->addChild(ASTNode::GetNewInstance(TokenType::Default, auxiliar.value));
    paramsNode->addChild(paramNode);

    // Procesamos la cola de parámetros (si hay más)
    ParamsTail(paramsNode);

    return paramsNode;
}

void ParamsTail(std::shared_ptr<ASTNode> paramsNode) {
    if (match(TOKEN_COMA)) {
        // Obtenemos el siguiente parámetro y lo añadimos al nodo de parámetros
        auto nextParam = Params();
        if (nextParam != nullptr) {

            paramsNode->addChild(nextParam);
        }
    }
    // Si no hay coma, terminamos la lista de parámetros
}


std::shared_ptr<ASTNode> StmtList() {
    // Creamos un nodo para la lista de sentencias
    auto stmtListNode = ASTNode::GetNewInstance(TokenType::Default, "StmtList");

    // Añadimos la primera sentencia
    auto stmtNode = Statement();
    if (stmtNode != nullptr) {
        stmtListNode->addChild(stmtNode);
    }

    // Procesamos las sentencias restantes
    StmtListPrime(stmtListNode);

    return stmtListNode;
}

void StmtListPrime(std::shared_ptr<ASTNode> stmtListNode) {
    if (nextTokenIsStatement()) {
        // Procesamos otra sentencia
        auto stmtNode = Statement();
        if (stmtNode != nullptr) {
            stmtListNode->addChild(stmtNode);
        }
        // Llamada recursiva para continuar procesando más sentencias
        StmtListPrime(stmtListNode);
    }
}

bool nextTokenIsStatement() {
    return (currentToken.token == TOKEN_IF || currentToken.token == TOKEN_WHILE ||
            currentToken.token == TOKEN_RETURN || currentToken.token == TOKEN_ID ||
            currentToken.token == TOKEN_PRINT || currentToken.token == TOKEN_VAR || 
            currentToken.token == TOKEN_FOR || currentToken.token == TOKEN_ASIG);
}

std::shared_ptr<ASTNode> Statement() {
    if (match(TOKEN_VAR)) {  // Si encontramos 'VAR', procesamos una declaración de variable
        return VarDecl();  
    } 
    else if (match(TOKEN_IF)) {  // Si encontramos 'IF', procesamos una sentencia condicional
        return IfStmt();
    } 
    else if (match(TOKEN_FOR)) {  // Si encontramos 'FOR', procesamos una sentencia de bucle for
        return ForStmt();
    } 
    else if (match(TOKEN_RETURN)) {  // Si encontramos 'RETURN', procesamos una sentencia de retorno
        return ReturnStmt();
    } 
    else if (match(TOKEN_PRINT)) {  // Si encontramos 'PRINT', procesamos una sentencia de impresión
        return PrintStmt();
    } 
    else if (match(TOKEN_LL_O)) {  // Si encontramos '{', procesamos un bloque de código (lista de sentencias)
        auto stmtListNode = StmtList();
        if (!match(TOKEN_LL_C)) {
            error("Se esperaba '}' después de la lista de sentencias.");
            sync();
        }
        return stmtListNode;
    } 
    else {  // De lo contrario, consideramos que es una expresión
        return ExprStmt();
    }
}


std::shared_ptr<ASTNode> IfStmt() {
    // Creamos un nodo para la sentencia 'if'
    auto ifNode = ASTNode::GetNewInstance(TokenType::Default, "IfStmt");

    // Verificamos que el siguiente token sea '('
    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba '(' después de 'if'.");
        sync();
        return nullptr;
    }

    // Procesamos la expresión condicional y la añadimos como hijo del nodo 'if'
    auto conditionNode = Expression();
    ifNode->addChild(conditionNode);

    // Verificamos que el siguiente token sea ')'
    if (!match(TOKEN_DELIM_P_C)) {
        error("Se esperaba ')' después de la expresión condicional.");
        sync();
        return nullptr;
    }

    // Procesamos el cuerpo de la sentencia 'if'
    auto ifBodyNode = Statement();
    ifNode->addChild(ifBodyNode);

    // Procesamos 'else if' o 'else'
    auto elseIfNode = ElseIfList();
    if (elseIfNode) {
        ifNode->addChild(elseIfNode);
    }

    return ifNode;
}



std::shared_ptr<ASTNode> extra() {
    if (match(TOKEN_IF)) {  // Si encontramos 'IF', procesamos una sentencia condicional
        return IfStmt();  // Devolvemos el nodo del AST para la sentencia 'if'
    } else if (match(TOKEN_LL_O)) {  // Si encontramos '{', procesamos un bloque de código (lista de sentencias)
        auto stmtListNode = StmtList();  // Procesamos la lista de sentencias
        if (!match(TOKEN_LL_C)) {  // Verificamos que haya una llave de cierre
            error("Se esperaba '}' después de la lista de sentencias.");
            sync();
        }
        return stmtListNode;  // Devolvemos el nodo de la lista de sentencias
    }
    return nullptr;  // Si no es 'if' ni un bloque, devolvemos nullptr
}

std::shared_ptr<ASTNode> ElseIfList() {
    if (match(TOKEN_ELSE)) {
        return extra();  // Llamamos a 'extra()' para procesar un posible 'if' o bloque
    }
    return nullptr;  // Si no encontramos un 'else', devolvemos nullptr
}


std::shared_ptr<ASTNode> ForStmt() {
    // Crear el nodo principal del AST para la sentencia 'for'
    auto forNode = ASTNode::GetNewInstance(TokenType::Default, "ForStmt");

    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba '(' después de 'for'.");
        sync();
        return nullptr;
    }

    // Procesamos la inicialización, que es una sentencia de expresión (por ejemplo, 'i = 0')
    auto initNode = ExprStmt();  
    if (initNode) {
        forNode->addChild(initNode);  // Añadir el nodo de inicialización al nodo del 'for'
    }

    // Procesamos la condición de continuación del bucle (por ejemplo, 'i < 10')
    auto conditionNode = Expression();
    forNode->addChild(conditionNode);  // Añadir el nodo de condición al nodo del 'for'

    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la condición.");
        sync();
    }

    // Procesamos la actualización (por ejemplo, 'i++')
    auto updateNode = ExprStmt();
    if (updateNode) {
        forNode->addChild(updateNode);  // Añadir el nodo de actualización al nodo del 'for'
    }

    if (!match(TOKEN_DELIM_P_C)) {
        error("Se esperaba ')' después de los elementos del for.");
        sync();
    }

    // Procesamos el cuerpo del bucle
    auto bodyNode = Statement();
    if (bodyNode) {
        forNode->addChild(bodyNode);  // Añadir el nodo del cuerpo al nodo del 'for'
    }

    return forNode;  // Devolver el nodo completo de la sentencia 'for'
}


std::shared_ptr<ASTNode> ReturnStmt() {
    // Crear el nodo principal para la sentencia de retorno
    auto returnNode = ASTNode::GetNewInstance(TokenType::Default, "ReturnStmt");

    // Procesamos la expresión de retorno
    auto returnExpr = Expression();
    returnNode->addChild(returnExpr);  // Añadir la expresión de retorno como hijo del nodo 'return'

    // Verificamos si hay un punto y coma al final
    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la expresión de retorno.");
        sync();
    }

    return returnNode;  // Devolver el nodo completo de la sentencia 'return'
}


std::shared_ptr<ASTNode> PrintStmt() {
    // Crear el nodo principal para la sentencia de impresión
    auto printNode = ASTNode::GetNewInstance(TokenType::Default, "PrintStmt");

    // Procesamos la lista de expresiones
    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba '(' después de 'print'.");
        sync();
    }

    auto exprListNode = ExprList();  // Procesamos las expresiones a imprimir
    printNode->addChild(exprListNode);

    if (!match(TOKEN_DELIM_P_C)) {
        error("Se esperaba ')' después de la lista de expresiones.");
        sync();
    }

    // Verificamos si hay un punto y coma al final
    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la lista de expresiones.");
        sync();
    }

    return printNode;  // Devolver el nodo completo de la sentencia 'print'
}


std::shared_ptr<ASTNode> ExprList() {
    auto exprListNode = ASTNode::GetNewInstance(TokenType::Default, "ExprList");

    // Procesamos la primera expresión
    auto firstExpr = Expression();
    exprListNode->addChild(firstExpr);  // Añadimos la primera expresión al nodo de la lista

    // Permitimos múltiples expresiones separadas por comas
    while (match(TOKEN_COMA)) {
        auto nextExpr = Expression();
        exprListNode->addChild(nextExpr);  // Añadimos las siguientes expresiones al nodo
    }

    return exprListNode;  // Devolvemos el nodo que contiene todas las expresiones
}



std::shared_ptr<ASTNode> ExprStmt() {
    auto exprStmtNode = ASTNode::GetNewInstance(TokenType::Default, "ExprStmt");

    // Procesamos la expresión
    auto exprNode = Expression();
    exprStmtNode->addChild(exprNode);  // Añadimos la expresión como hijo del nodo de la sentencia

    // Aseguramos que la expresión termine con un punto y coma
    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la expresión.");
        sync();
    }

    return exprStmtNode;  // Devolvemos el nodo de la sentencia
}




std::shared_ptr<ASTNode> Expression() {
    std::shared_ptr<ASTNode> exprNode = nullptr;

    if (match(TOKEN_ASIG)) {  // Si encontramos una asignación
        auto assignNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_COMP_E, "ASSIGN");

        if (!match(TOKEN_ID)) {  // Verificamos que haya un identificador después del '='
            error("Se esperaba un identificador después de 'ASSIGN'.");
            return nullptr;
        }
        
        // Creamos el nodo del identificador
        auto idNode = ASTNode::GetNewInstance(TokenType::Default, auxiliar.value);
        //idNode->setValue(currentToken.value);  // Establecemos el valor del identificador
        assignNode->addChild(idNode);

        if (!match(TOKEN_ASIG)) {  // Verificamos que haya un '=' después del identificador
            error("Se esperaba '=' después del identificador.");
            return nullptr;
        }

        // Procesamos la expresión en el lado derecho de la asignación
        auto rightExprNode = Expression();
        assignNode->addChild(rightExprNode);  // Añadimos la expresión al nodo de asignación
        
        return assignNode;  // Devolvemos el nodo de asignación

    } else {
        // Si no es una asignación, procesamos una expresión lógica
        exprNode = OrExpr();  // Procesamos la expresión lógica
        return exprNode;  // Devolvemos el nodo de la expresión lógica
    }
}

std::shared_ptr<ASTNode> OrExpr() {
    // Procesamos el lado izquierdo de la expresión OR
    auto leftNode = AndExpr();

    // Continuamos con la parte recursiva de la expresión OR
    auto orNode = OrExprPrime(leftNode);

    return orNode;
}

std::shared_ptr<ASTNode> OrExprPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_OR)) {  // Si encontramos el operador '||'
        // Creamos un nodo para la operación OR
        auto orOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_OR, "OR");
        
        // Añadimos el nodo izquierdo (que ya procesamos en OrExpr)
        orOpNode->addChild(leftNode);

        // Procesamos el lado derecho (AndExpr)
        auto rightNode = AndExpr();

        // Añadimos el nodo derecho
        orOpNode->addChild(rightNode);

        // Llamada recursiva para procesar más expresiones OR
        auto furtherOrNode = OrExprPrime(orOpNode);
        
        return furtherOrNode;  // Devolvemos el nodo completo
    }

    // Caso epsilon: si no hay más operadores '||', simplemente devolvemos el nodo actual
    return leftNode;
}


std::shared_ptr<ASTNode> AndExpr() {
    // Procesamos el lado izquierdo de la expresión AND
    auto leftNode = EqExpr();

    // Continuamos con la parte recursiva de la expresión AND
    auto andNode = AndExprPrime(leftNode);

    return andNode;
}

std::shared_ptr<ASTNode> AndExprPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_AND)) {  // Si encontramos el operador '&&'
        // Creamos un nodo para la operación AND
        auto andOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_AND, "AND");

        // Añadimos el nodo izquierdo (que ya procesamos en AndExpr)
        andOpNode->addChild(leftNode);

        // Procesamos el lado derecho (EqExpr)
        auto rightNode = EqExpr();

        // Añadimos el nodo derecho
        andOpNode->addChild(rightNode);

        // Llamada recursiva para procesar más expresiones AND
        auto furtherAndNode = AndExprPrime(andOpNode);

        return furtherAndNode;  // Devolvemos el nodo completo
    }

    // Caso epsilon: si no hay más operadores '&&', simplemente devolvemos el nodo actual
    return leftNode;
}

std::shared_ptr<ASTNode> EqExpr() {
    // Procesamos el lado izquierdo de la expresión de igualdad
    auto leftNode = RelExpr();

    // Continuamos con la parte recursiva de la expresión de igualdad
    auto eqNode = EqExprPrime(leftNode);

    return eqNode;
}


std::shared_ptr<ASTNode> EqExprPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_COMP_E)) {     // Si encontramos '=='
        // Creamos un nodo para la operación de comparación de igualdad
        auto eqOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_COMP_E, "==");

        // Añadimos el nodo izquierdo (que ya procesamos en EqExpr)
        eqOpNode->addChild(leftNode);

        // Procesamos la siguiente expresión (RelExpr)
        auto rightNode = RelExpr();

        // Añadimos el nodo derecho
        eqOpNode->addChild(rightNode);

        // Continuamos recursivamente si hay más operadores de comparación
        auto furtherEqNode = EqExprPrime(eqOpNode);

        return furtherEqNode;
    } 
    else if (match(TOKEN_OPER_COMP_D)) { // Si encontramos '!='
        // Creamos un nodo para la operación de comparación de desigualdad
        auto neqOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_COMP_E, "!=");

        // Añadimos el nodo izquierdo
        neqOpNode->addChild(leftNode);

        // Procesamos la siguiente expresión (RelExpr)
        auto rightNode = RelExpr();

        // Añadimos el nodo derecho
        neqOpNode->addChild(rightNode);

        // Continuamos recursivamente si hay más operadores de comparación
        auto furtherNeqNode = EqExprPrime(neqOpNode);

        return furtherNeqNode;
    }

    // Caso epsilon: si no hay más operadores de comparación, devolvemos el nodo actual
    return leftNode;
}

std::shared_ptr<ASTNode> RelExpr() {
    // Procesamos el lado izquierdo de la expresión relacional
    auto leftNode = Expr();

    // Continuamos con la parte recursiva de la expresión relacional
    auto relNode = RelExprPrime(leftNode);

    return relNode;
}


std::shared_ptr<ASTNode> RelExprPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_MENOR)) {   // Menor que '<'
        auto relOpNode = ASTNode::GetNewInstance(TokenType::Default, "<");

        // Añadimos el nodo izquierdo
        relOpNode->addChild(leftNode);

        // Procesamos la siguiente expresión (Expr)
        auto rightNode = Expr();

        // Añadimos el nodo derecho
        relOpNode->addChild(rightNode);

        // Continuamos recursivamente si hay más operadores relacionales
        auto furtherRelNode = RelExprPrime(relOpNode);

        return furtherRelNode;
    } 
    else if (match(TOKEN_OPER_MAYOR)) {  // Mayor que '>'
        auto relOpNode = ASTNode::GetNewInstance(TokenType::Default, ">");

        relOpNode->addChild(leftNode);

        auto rightNode = Expr();
        relOpNode->addChild(rightNode);

        auto furtherRelNode = RelExprPrime(relOpNode);

        return furtherRelNode;
    } 
    else if (match(TOKEN_OPER_MEN_E)) {  // Menor o igual '<='
        auto relOpNode = ASTNode::GetNewInstance(TokenType::Default, "<=");

        relOpNode->addChild(leftNode);

        auto rightNode = Expr();
        relOpNode->addChild(rightNode);

        auto furtherRelNode = RelExprPrime(relOpNode);

        return furtherRelNode;
    } 
    else if (match(TOKEN_OPER_MAY_E)) {  // Mayor o igual '>='
        auto relOpNode = ASTNode::GetNewInstance(TokenType::Default, ">=");

        relOpNode->addChild(leftNode);

        auto rightNode = Expr();
        relOpNode->addChild(rightNode);

        auto furtherRelNode = RelExprPrime(relOpNode);

        return furtherRelNode;
    }

    // Caso epsilon: no hacer nada si no hay operadores relacionales
    return leftNode;
}

std::shared_ptr<ASTNode> Expr() {
    auto leftNode = Term();  // Procesamos el primer término
    return ExprPrime(leftNode);  // Continuamos con la parte recursiva
}

std::shared_ptr<ASTNode> ExprPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_SUM)) {  // Verificamos si hay un operador '+'
        auto addOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_SUM, "+");

        addOpNode->addChild(leftNode);  // Añadimos el nodo izquierdo

        auto rightNode = Term();  // Procesamos el siguiente término
        addOpNode->addChild(rightNode);  // Añadimos el nodo derecho

        return ExprPrime(addOpNode);  // Continuamos recursivamente
    } 
    else if (match(TOKEN_OPER_REST)) {  // Verificamos si hay un operador '-'
        auto subOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_REST, "-");

        subOpNode->addChild(leftNode);  // Añadimos el nodo izquierdo

        auto rightNode = Term();  // Procesamos el siguiente término
        subOpNode->addChild(rightNode);  // Añadimos el nodo derecho

        return ExprPrime(subOpNode);  // Continuamos recursivamente
    }

    // Caso epsilon: no hay más operadores, devolvemos el nodo actual
    return leftNode;
}



std::shared_ptr<ASTNode> Term() {
    auto leftNode = Unary();  // Procesamos la operación unaria
    return TermPrime(leftNode);  // Continuamos con la parte recursiva
}

std::shared_ptr<ASTNode> TermPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_MUL)) {  // Verificamos si tenemos '*'
        auto mulOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_MUL, "*");

        // Añadimos el nodo izquierdo
        mulOpNode->addChild(leftNode);

        // Procesamos la operación unaria y añadimos el nodo derecho
        auto rightNode = Unary();
        mulOpNode->addChild(rightNode);

        // Continuamos recursivamente para procesar más multiplicaciones
        return TermPrime(mulOpNode);
    } 
    else if (match(TOKEN_OPER_DIV)) {  // Verificamos si tenemos '/'
        auto divOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_DIV, "/");

        divOpNode->addChild(leftNode);

        auto rightNode = Unary();
        divOpNode->addChild(rightNode);

        return TermPrime(divOpNode);
    } 
    else if (match(TOKEN_OPER_MOD)) {  // Verificamos si tenemos '%'
        auto modOpNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_MOD, "%");

        modOpNode->addChild(leftNode);

        auto rightNode = Unary();
        modOpNode->addChild(rightNode);

        return TermPrime(modOpNode);
    }

    // Caso epsilon: no hay más operadores de multiplicación/división/módulo
    return leftNode;
}


std::shared_ptr<ASTNode> Unary() {
    if (match(TOKEN_OPER_EX)) {  // Verificamos si tenemos '!'
        auto notNode = ASTNode::GetNewInstance(TokenType::Default, "!");
        auto operandNode = Unary();  // Procesamos el siguiente operando unario recursivamente
        notNode->addChild(operandNode);
        return notNode;
    } else {
        return Factor();  // Si no es un operador unario, procesamos un factor
    }
}



std::shared_ptr<ASTNode> Factor() {
    auto primaryNode = Primary();  // Procesamos el elemento primario
    return FactorPrime(primaryNode);  // Continuamos con la parte recursiva
}

std::shared_ptr<ASTNode> FactorPrime(std::shared_ptr<ASTNode> leftNode) {
    if (match(TOKEN_OPER_EXP)) {  // Verificamos si tenemos el operador de potencia '^'
        auto powerNode = ASTNode::GetNewInstance(TokenType::TOKEN_OPER_EXP, "^");

        // Añadimos el nodo izquierdo (base de la potencia)
        powerNode->addChild(leftNode);

        // Procesamos el operando derecho (exponente)
        auto rightNode = Primary();
        powerNode->addChild(rightNode);

        // Continuamos recursivamente para manejar potencias encadenadas
        return FactorPrime(powerNode);
    }

    // Caso epsilon: no hay más operadores de potencia
    return leftNode;
}


std::shared_ptr<ASTNode> Primary() {
    if (match(TOKEN_INTEGER)) {  // Verificamos si es un número entero
        // Creamos un nodo para el número entero
        //cout<<currentToken.value<<endl;
        return ASTNode::GetNewInstance(TokenType::TOKEN_INTEGER, auxiliar.value);
    } 
    else if (match(TOKEN_CHAR)) {  // Caracter
        return ASTNode::GetNewInstance(TokenType::TOKEN_CHAR, auxiliar.value);
    } 
    else if (match(TOKEN_STRING)) {  // Cadena de texto
        return ASTNode::GetNewInstance(TokenType::TOKEN_STRING, auxiliar.value);
        
    } 
    else if (match(TOKEN_BOOLEAN)) {  // Booleano
        return ASTNode::GetNewInstance(TokenType::TOKEN_BOOLEAN, auxiliar.value);
    } 
    else if (match(TOKEN_DELIM_P_O)) {  // '('
        auto exprNode = Expression();  // Procesamos la expresión dentro de los paréntesis
        if (!match(TOKEN_DELIM_P_C)) { // Verificamos si hay ')'
            error("Se esperaba ')' después de la expresión.");
        }
        return exprNode;  // Devolvemos la expresión procesada
    } 
    else if (match(TOKEN_ID)) {  // Identificador
        // Creamos un nodo para el identificador
        auto idNode = ASTNode::GetNewInstance(TokenType::TOKEN_ID, auxiliar.value);
        return PrimaryTail(idNode);  // Procesamos el tail del identificador
    } 
    else {
        error("Se esperaba un valor primario.");
        return ASTNode::GetNewInstance(TokenType::Default, "error");
    }
}


std::shared_ptr<ASTNode> PrimaryTail(std::shared_ptr<ASTNode> idNode) {
    if (match(TOKEN_DELIM_P_O)) {  // Verificamos si es una llamada a función
        // Creamos un nodo para la llamada a función
        auto callNode = ASTNode::GetNewInstance(TokenType::Default, "call");
        callNode->addChild(idNode);  // Añadimos el identificador como nombre de la función

        // Procesamos la lista de argumentos
        auto argListNode = ExprList();
        callNode->addChild(argListNode);

        if (!match(TOKEN_DELIM_P_C)) {  // Verificamos si hay ')'
            error("Se esperaba ')' después de la lista de expresiones.");
        }

        // Devolvemos el nodo de llamada a función
        return callNode;
    } else {
        // Si no es una llamada a función, verificamos si es un acceso a arreglo
        return FactorPrime(idNode);
    }
}

