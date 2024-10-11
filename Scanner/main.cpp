#include <iostream>
#include <fstream>
#include <string>
#include "../Scanner/reservadas.cpp"
#include "../Scanner/parser.cpp"
using namespace std;


void SkipWhiteSpace()
{
    // Ignorar espacios en blanco
    while (isspace(*peekcharcurrent))
    {
        if (*peekcharcurrent == '\n')
        {
            fila++;
            columna = 0;
        }
        peekcharcurrent++;
        columna++;
    }

    // Manejo de comentarios
    if (*peekcharcurrent == '/')
    {
        peekcharcurrent++;
        columna++;

        // Comentario de línea "//"
        if (*peekcharcurrent == '/')
        {
            while (*peekcharcurrent != '\n' && *peekcharcurrent != '\0')
            {
                peekcharcurrent++;
                columna++;
            }
            fila++;
        }
        // Comentario de bloque "/*"
        else if (*peekcharcurrent == '*')
        {
            // Marcar que estamos dentro de un comentario de bloque
            Comment_block = true;
            peekcharcurrent++;
            columna++;

            // Ignorar hasta encontrar "*/"
            while (*peekcharcurrent != '\0')
            {
                if (*peekcharcurrent == '*' && *(peekcharcurrent + 1) == '/')
                {
                    peekcharcurrent += 2; // Avanzar más allá del "*/"
                    columna += 2;
                    Comment_block = false; 
                    break;
                }

                if (*peekcharcurrent == '\n')
                {
                    fila++;
                    columna = 0;
                }
                else
                {
                    columna++;
                }
                peekcharcurrent++;
            }
        }
        else{
            peekcharcurrent--;
        }
    }
}


Token peekChar()
{
    word = "";
    int columnaActual = columna;
    SkipWhiteSpace();
    if (isdigit(*peekcharcurrent))
    {
        while (isdigit(*peekcharcurrent))
        {
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
        }
        if (isalpha(*peekcharcurrent))
        {
            while (isdigit(*peekcharcurrent) || isalpha(*peekcharcurrent))
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
            }
            return {TOKEN_UNKNOWN, word, fila, columnaActual};
        }
        SkipWhiteSpace();
        return {TOKEN_INTEGER, word, fila, columnaActual};
    }
    else if (isalpha(*peekcharcurrent) || *peekcharcurrent == '_')
    {
        while (isalpha(*peekcharcurrent) || *peekcharcurrent == '_')
        {
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
        }
        if (isdigit(*peekcharcurrent))
        {
            while (isdigit(*peekcharcurrent) || isalpha(*peekcharcurrent) || *peekcharcurrent == '_')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
            }
            return {TOKEN_UNKNOWN, word, fila, columnaActual};
        }
        else
        { // Revisar si lo que hemos leido es palabra reservada
            if (reservadas.count(word))
            {
                return {Mapa_tokens.find(word)->second, word, fila, columnaActual};
            };
        }
        SkipWhiteSpace();
        return {TOKEN_ID, word, fila, columnaActual};
    }
    else
    {
        bool doble = 0;
        int d = 0;
        switch (*peekcharcurrent)
        {
        case ':':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_DECLAR, word, fila, columnaActual};
        case '=':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            if (*peekcharcurrent == '=')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_COMP_E, word, fila, columnaActual};
            return {TOKEN_ASIG, word, fila, columnaActual};
        case ';':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_DELIM_SC, word, fila, columnaActual};
        case '[':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_DELIM_B_O, word, fila, columnaActual};
        case ']':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_DELIM_B_C, word, fila, columnaActual};
        case '(':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_DELIM_P_O, word, fila, columnaActual};
        case ')':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_DELIM_P_C, word, fila, columnaActual};
        case '+':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_SUM, word, fila, columnaActual};
        case '-':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_REST, word, fila, columnaActual};
        case '!':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            if (*peekcharcurrent == '=')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_COMP_D, word, fila, columnaActual};
            return {TOKEN_OPER_EX, word, fila, columnaActual};
        case '^':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_EXP, word, fila, columnaActual};
        case '*':
        {
            word += *peekcharcurrent; 
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_MUL, word, fila, columnaActual}; // Operador de multiplicación
        }

        case '/':
        {
            word += *peekcharcurrent; 
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_DIV, word, fila, columnaActual}; // Operador de división
        }

        case '%':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_MOD, word, fila, columnaActual};
        case '<':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            if (*peekcharcurrent == '=')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_MEN_E, word, fila, columnaActual};
            return {TOKEN_OPER_MENOR, word, fila, columnaActual};
        case '>':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            if (*peekcharcurrent == '=')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_MAY_E, word, fila, columnaActual};
            return {TOKEN_OPER_MAYOR, word, fila, columnaActual};
        case '&':
            word += *peekcharcurrent;
            peekcharcurrent++;
            if (*peekcharcurrent == '&')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_AND, word, fila, columnaActual};
            return {TOKEN_UNKNOWN, word, fila, columnaActual};
        case '|':
            word += *peekcharcurrent;
            peekcharcurrent++;
            if (*peekcharcurrent == '|')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_OR, word, fila, columnaActual};
            return {TOKEN_UNKNOWN, word, fila, columnaActual};
        case '{':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_LL_O, word, fila, columnaActual};
        case '}':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_LL_C, word, fila, columnaActual};
        case ',':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_COMA, word, fila, columnaActual};

        case '\'':
            word += *peekcharcurrent; 
            peekcharcurrent++;
            columna++;

            if (*peekcharcurrent != '\'' && *peekcharcurrent != '\0')
            {
                word += *peekcharcurrent; 
                peekcharcurrent++;
                columna++;
            }
            else
            {
                return {TOKEN_UNKNOWN, word, fila, columnaActual};
            }

            if (*peekcharcurrent == '\'')
            {
                word += *peekcharcurrent; 
                peekcharcurrent++;
                columna++;
            }
            else
            {
                return {TOKEN_UNKNOWN, word, fila, columnaActual};
            }

            SkipWhiteSpace();
            return {TOKEN_CHAR, word, fila, columnaActual};

        case '"':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;

            while (*peekcharcurrent != '"' && *peekcharcurrent != '\0')
            {
                word += *peekcharcurrent; 
                peekcharcurrent++;
                columna++;
            }

            if (*peekcharcurrent == '"')
            {
                word += *peekcharcurrent; 
                peekcharcurrent++;
                columna++;
            }
            else
            {
                return {TOKEN_UNKNOWN, word, fila, columnaActual};
            }

            SkipWhiteSpace();
            return {TOKEN_STRING, word, fila, columnaActual};
        default:
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_UNKNOWN, word, fila, columnaActual};
        }
    }
}

void getChar(string linea)
{
    getcharcurrent = peekcharcurrent = linea.c_str();

    while (*getcharcurrent != '\0')
    {
        // Si estamos dentro de un comentario de bloque, seguir ignorando hasta el cierre "*/"
        if (Comment_block)
        {
            while (*peekcharcurrent != '\0' && *peekcharcurrent!='\n')
            {
                if (*peekcharcurrent == '*' && *(peekcharcurrent + 1) == '/')
                {
                    peekcharcurrent += 2;
                    columna += 2;
                    Comment_block = false; // Salimos del comentario de bloque
                    break;
                }
                else
                {
                   columna++;
                    peekcharcurrent++;
                }
            }
            fila++;
            
            getcharcurrent = peekcharcurrent;
            continue;
        }

        while (*getcharcurrent != '\n' && *getcharcurrent != '\0')
        {
            SkipWhiteSpace(); // Ignorar espacios en blanco y comentarios

            if (*peekcharcurrent == '\0' || *peekcharcurrent == '\n') {
                break; 
            }

            tokens.push_back(peekChar());

            getcharcurrent = peekcharcurrent;
        }

        fila++;
        columna = 0;
    }
}


int main()
{
    string name = "B-minor.txt";

    ifstream archivo(name);

    if (!archivo.is_open())
    {
        cerr << "No se pudo abrir el archivo " << name << endl;
        return 1;
    }

    string linea;

    while (getline(archivo, linea))
    {
        getChar(linea);
    }
    cout <<"INFO SCAN - Start scanning…"<<endl;
    for (auto token : tokens)
    {
        if (tokenMap[token.token]!="DESCONOCIDO")
        {
            cout<<"DEBUG SCAN - "<<tokenMap[token.token]<<" ["<<token.value<<"] found at ("<<token.fila<<":"<<token.columna<<")"<<endl;
        }else{
            token_errores.push_back(token);
        }
    }
    cout <<endl<<"INFO SCAN - Completed with "<<token_errores.size()<<" errors"<<endl;

    for (auto token:token_errores)
    {
        cout<<"DEBUG SCAN ERROR - "<<tokenMap[token.token]<<" ["<<token.value<<"] found at ("<<token.fila<<":"<<token.columna<<")"<<endl;
    }
    



    return 0;
}