#pragma once
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include "../Scanner/reservadas.cpp"
#include "../Scanner/parser.h"

size_t currentTokenIndex = 0; // Índice actual en la lista de tokens
Token currentToken;
bool error_bool=0;

using namespace std;




void error(const string &mensaje) {
    cout << "Error: " << mensaje << " en la línea " << currentToken.fila << ", columna " << currentToken.columna << ". Token encontrado: " << currentToken.value << endl;
    error_bool=1;
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
        nextToken(); // Avanzamos al siguiente token si es correcto
        return true;
    }
    return false;
}


void Program() {
    Declaration();
    ProgramPrime();
    if (!error_bool){
        cout<<"\nPROGRAMA ACEPTADO\n";
    }
    
}

void ProgramPrime() {
    if (currentToken.token == TOKEN_VAR || currentToken.token == TOKEN_FUNCTION) {
        Declaration();
        ProgramPrime();
    }
    // Si el token actual no es un inicio válido de Declaración, se permite epsilon ('') vacío
}


void Declaration() {
    if (match(TOKEN_VAR)) {
        VarDecl();
    } else if (match(TOKEN_FUNCTION)) {
        Function();
    } else {
        Expression();
        if (!match(TOKEN_DELIM_SC)) { // Esperamos un ';'
            error("Se esperaba ';' después de la expresión.");
        }
    }
}

void VarDecl() {
    Type();  // Procesamos el tipo de la variable
    if (!match(TOKEN_ID)) {  // Verificamos si hay un identificador después del tipo
        error("Se esperaba un identificador después del tipo.");
    }
    VarDeclTail();  // Procesamos el tail de VarDecl
}

void VarDeclTail() {
    if (match(TOKEN_DELIM_SC)) {  // Verificamos si tenemos ';'
        // Declaración completa sin inicialización
        return;
    } else if (match(TOKEN_ASIG)) {  // Verificamos si hay '=' para inicialización
        Expression();  // Procesamos la expresión de inicialización
        if (!match(TOKEN_DELIM_SC)) {  // Verificamos si hay ';' al final
            error("Se esperaba ';' después de la inicialización.");
        }
    } else {
        error("Se esperaba ';' o '=' en la declaración de la variable.");
    }
}

void Function() {
    if (!match(TOKEN_FUNCTION)) {  // Verificamos si empieza con la palabra clave 'function'
        error("Se esperaba 'function'.");
    }

    Type();  // Procesamos el tipo de retorno de la función

    if (!match(TOKEN_ID)) {  // Verificamos si hay un identificador (nombre de la función)
        error("Se esperaba un identificador para la función.");
    }

    if (!match(TOKEN_DELIM_P_O)) {  // Verificamos si hay un paréntesis de apertura '('
        error("Se esperaba '(' después del identificador de la función.");
    }

    Params();  // Procesamos la lista de parámetros

    if (!match(TOKEN_DELIM_P_C)) {  // Verificamos si hay un paréntesis de cierre ')'
        error("Se esperaba ')' después de los parámetros de la función.");
    }

    if (!match(TOKEN_LL_O)) {  // Verificamos si hay una llave de apertura '{'
        error("Se esperaba '{' al inicio del cuerpo de la función.");
    }

    StmtList();  // Procesamos la lista de sentencias (cuerpo de la función)

    if (!match(TOKEN_LL_C)) {  // Verificamos si hay una llave de cierre '}'
        error("Se esperaba '}' al final del cuerpo de la función.");
    }
}



void Type() {
    if (match(TOKEN_INTEGER)) {
        TypePrime();  // Procesamos la posible secuencia de corchetes
    } else if (match(TOKEN_BOOLEAN)) {
        TypePrime();
    } else if (match(TOKEN_CHAR)) {
        TypePrime();
    } else if (match(TOKEN_STRING)) {
        TypePrime();
    } else if (match(TOKEN_VOID)) {
        // VOID no tiene arreglo, no necesitamos llamar a TypePrime()
    } else {
        error("Se esperaba un tipo válido (integer, boolean, character, string, void).");
    }
}

void TypePrime() {
    if (match(TOKEN_DELIM_B_O)) {  // Verificamos si hay un corchete de apertura '['
        if (!match(TOKEN_DELIM_B_C)) {  // Verificamos si hay un corchete de cierre ']'
            error("Se esperaba ']' después del '['.");
        }
        TypePrime();  // Llamada recursiva para procesar arreglos múltiples (como int[][])
    }
    // Si no hay corchetes, el tipo es simple (por ejemplo, `int` o `int[]`)
}



void Params() {
    Type();  // Procesamos el tipo del primer parámetro

    if (!match(TOKEN_ID)) {  // Verificamos si hay un identificador (nombre del parámetro)
        error("Se esperaba un identificador después del tipo en los parámetros.");
    }

    ParamsTail();  // Procesamos la cola de parámetros (si hay más)
}

void ParamsTail() {
    if (match(TOKEN_COMA)) {  // Si encontramos una coma, esperamos otro parámetro
        Params();  // Llamada recursiva para manejar el siguiente parámetro
    };
    // Si no hay coma, terminamos la lista de parámetros
}

void StmtList() {
    Statement();  // Procesamos una sentencia
    StmtListPrime();  // Procesamos la lista de sentencias restantes
}

void StmtListPrime() {
    if (nextTokenIsStatement()) {  // Verificamos si el siguiente token puede ser una sentencia válida
        Statement();  // Procesamos otra sentencia
        StmtListPrime();  // Llamada recursiva para continuar procesando más sentencias
    }
    // Si no hay más sentencias, terminamos la lista de sentencias
}

bool nextTokenIsStatement() {
    return (currentToken.token == TOKEN_IF || currentToken.token == TOKEN_WHILE ||
            currentToken.token == TOKEN_RETURN || currentToken.token == TOKEN_ID ||
            currentToken.token == TOKEN_PRINT || currentToken.token == TOKEN_VAR);
}

void Statement() {
    if (match(TOKEN_VAR)) {  // Si encontramos 'VAR', procesamos una declaración de variable
        VarDecl();  
    } 
    /*else if (match(TOKEN_IF)) {  // Si encontramos 'IF', procesamos una sentencia condicional
        IfStmt();
    } */
    else if (match(TOKEN_FOR)) {  // Si encontramos 'FOR', procesamos una sentencia de bucle for
        ForStmt();
    } 
    else if (match(TOKEN_RETURN)) {  // Si encontramos 'RETURN', procesamos una sentencia de retorno
        ReturnStmt();
    } 
    else if (match(TOKEN_PRINT)) {  // Si encontramos 'PRINT', procesamos una sentencia de impresión
        PrintStmt();
    } 
    else if (match(TOKEN_LL_O)) {  // Si encontramos '{', procesamos un bloque de código (lista de sentencias)
        StmtList();
        if (!match(TOKEN_LL_C)) {
            error("Se esperaba '}' después de la lista de sentencias.");
        }
    } 
    else {  // De lo contrario, consideramos que es una expresión
        ExprStmt();
    }
}


void ForStmt() {
    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba '(' después de 'for'.");
    };

    // Procesamos la inicialización, que es una sentencia de expresión
    ExprStmt();

    // Procesamos la condición de continuación del bucle
    Expression();

    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la condición.");
    }

    // Procesamos la actualización
    ExprStmt();

    if (!match(TOKEN_DELIM_P_C)) {
        error("Se esperaba ')' después de los elementos del for.");
    }

    // Procesamos el cuerpo del bucle
    Statement();
}

void ReturnStmt() {
    Expression();  // Procesamos la expresión de retorno

    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la expresión de retorno.");
    }
}

void PrintStmt() {
    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba '(' después de 'print'.");
    }

    ExprList();  // Procesamos la lista de expresiones

    if (!match(TOKEN_DELIM_P_O)) {
        error("Se esperaba ')' después de la lista de expresiones.");
    }

    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la lista de expresiones.");
    }
}

void ExprList() {
    Expression();  // Procesamos la primera expresión
    while (match(TOKEN_COMA)) {  // Permitimos múltiples expresiones separadas por comas
        Expression();
    }
}


void ExprStmt() {
    if (match(TOKEN_DELIM_SC)) {
        // Caso de un punto y coma vacío, no hay nada que procesar
        return;
    }

    // Si no es un punto y coma vacío, procesamos la expresión
    Expression();

    // Aseguramos que la expresión termine con un punto y coma
    if (!match(TOKEN_DELIM_SC)) {
        error("Se esperaba ';' después de la expresión.");
    }
}







void Expression() {
    if (match(TOKEN_ASIG)) { 
        if (!match(TOKEN_ID)) {
            error("Se esperaba un identificador después de 'ASSIGN'.");
        }
        if (!match(TOKEN_ASIG)) {
            error("Se esperaba '=' después del identificador.");
        }
        Expression(); // Recursivamente procesamos la expresión
    } else {
        OrExpr(); // Comenzamos con la expresión lógica
    }
}

void OrExpr() {
    AndExpr();   // Procesamos el lado izquierdo (AndExpr)
    OrExprPrime(); // Continuamos con la parte recursiva
}

void OrExprPrime() {
    if (match(TOKEN_OPER_OR)) {  // Verificamos si tenemos '||'
        AndExpr();          // Si es así, procesamos otra AndExpr
        OrExprPrime();      // Continuamos con la recursividad
    }
    // Caso epsilon: no hacer nada si no hay '||'
}


void AndExpr() {
    EqExpr();     // Procesamos el lado izquierdo (EqExpr)
    AndExprPrime(); // Continuamos con la parte recursiva
}

void AndExprPrime() {
    if (match(TOKEN_OPER_AND)) {   // Verificamos si tenemos '&&'
        EqExpr();             // Si es así, procesamos otra EqExpr
        AndExprPrime();       // Continuamos con la recursividad
    }
    // Caso epsilon: no hacer nada si no hay '&&'
}


void EqExpr() {
    RelExpr();    // Procesamos el lado izquierdo (RelExpr)
    EqExprPrime(); // Continuamos con la parte recursiva
}

void EqExprPrime() {
    if (match(TOKEN_OPER_COMP_E)) {     // Verificamos si tenemos '=='
        RelExpr();             // Procesamos la RelExpr
        EqExprPrime();         // Continuamos recursivamente
    } else if (match(TOKEN_OPER_COMP_D)) { // Verificamos si tenemos '!='
        RelExpr();             // Procesamos la RelExpr
        EqExprPrime();         // Continuamos recursivamente
    }
    // Caso epsilon: no hacer nada si no hay '==' o '!='
}


void RelExpr() {
    Expr();      // Procesamos la primera expresión
    RelExprPrime(); // Continuamos con la parte recursiva
}

void RelExprPrime() {
    if (match(TOKEN_OPER_MENOR)) {   // Menor que '<'
        Expr();              // Procesamos otra Expr
        RelExprPrime();      // Continuamos recursivamente
    } else if (match(TOKEN_OPER_MAYOR)) {  // Mayor que '>'
        Expr(); 
        RelExprPrime();
    } else if (match(TOKEN_OPER_MEN_E)) {  // Menor o igual '<='
        Expr();
        RelExprPrime();
    } else if (match(TOKEN_OPER_MAY_E)) {  // Mayor o igual '>='
        Expr();
        RelExprPrime();
    }
    // Caso epsilon: no hacer nada si no hay '<', '
}

void Expr() {
    Term();        // Procesamos el primer término
    ExprPrime();   // Continuamos con la parte recursiva
}

void ExprPrime() {
    if (match(TOKEN_OPER_SUM)) {  // Verificamos si tenemos '+'
        Term();               // Procesamos otro término
        ExprPrime();          // Continuamos recursivamente
    } else if (match(TOKEN_OPER_REST)) {  // Verificamos si tenemos '-'
        Term();               
        ExprPrime();          
    }
    // Caso epsilon: no hacer nada si no hay '+' o '-'
}


void Term() {
    Unary();         // Procesamos la operación unaria
    TermPrime();     // Continuamos con la parte recursiva
}

void TermPrime() {
    if (match(TOKEN_OPER_MUL)) {  // Verificamos si tenemos '*'
        Unary();                  // Procesamos la operación unaria
        TermPrime();              // Continuamos recursivamente
    } else if (match(TOKEN_OPER_DIV)) {  // Verificamos si tenemos '/'
        Unary();
        TermPrime();
    } else if (match(TOKEN_OPER_MOD)) {     // Verificamos si tenemos '%'
        Unary();
        TermPrime();
    }
    // Caso epsilon: no hacer nada si no hay '*', '/', '%'
}


void Unary() {
    if (match(TOKEN_OPER_EX)) {  // Verificamos si tenemos '!'
        Unary();             // Procesamos otra operación unaria recursivamente
    } else {
        Factor();            // Si no, procesamos un factor
    }
}


void Factor() {
    Primary();        // Procesamos el elemento primario
    FactorPrime();    // Continuamos con la parte recursiva
}

void FactorPrime() {
    if (match(TOKEN_DELIM_B_O)) {  // Verificamos si tenemos '['
        Expression();             // Procesamos la expresión dentro de los corchetes
        if (!match(TOKEN_DELIM_B_C)) { // Verificamos si hay un ']'
            error("Se esperaba ']' después de la expresión.");
        }
        FactorPrime();            // Continuamos recursivamente
    }
    // Caso epsilon: no hacer nada si no hay '['
}


void Primary() {
    if (match(TOKEN_INTEGER)) {  // Verificamos si es un número entero
        // Procesamos entero
    } else if (match(TOKEN_CHAR)) {  // Caracter
        // Procesamos caracter
    } else if (match(TOKEN_STRING)) {  // Cadena de texto
        // Procesamos cadena
    } else if (match(TOKEN_BOOLEAN)) {  // Booleano
        // Procesamos booleano
    } else if (match(TOKEN_DELIM_P_O)) {  // '('
        Expression();  // Procesamos la expresión dentro de los paréntesis
        if (!match(TOKEN_DELIM_P_C)) { // Verificamos si hay ')'
            error("Se esperaba ')' después de la expresión.");
        }
    } else if (match(TOKEN_ID)) {  // Identificador
        PrimaryTail();  // Procesamos el tail de Primary
    } else {
        error("Se esperaba un valor primario.");
    }
}

void PrimaryTail() {
    if (match(TOKEN_DELIM_P_O)) {  // Verificamos si tenemos '('
        ExprList();  // Procesamos la lista de expresiones
        if (!match(TOKEN_DELIM_P_C)) {  // Verificamos si hay ')'
            error("Se esperaba ')' después de la lista de expresiones.");
        }
    }
    // Caso epsilon: no hacer nada si no hay '('
}

