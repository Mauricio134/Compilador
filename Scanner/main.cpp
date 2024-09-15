#include <iostream>
#include <fstream>
#include <string>
#include "../Scanner/reservadas.cpp"

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
            // Ignorar hasta el final de la línea
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
                    Comment_block = false; // Fin del comentario de bloque
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
            if (*peekcharcurrent == '+')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_S_SUMA, word, fila, columnaActual};
            return {TOKEN_OPER_SUM, word, fila, columnaActual};
        case '-':
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
            if (*peekcharcurrent == '-')
            {
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                doble = 1;
            }
            SkipWhiteSpace();
            if (doble)
                return {TOKEN_OPER_S_RESTA, word, fila, columnaActual};
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
            word += *peekcharcurrent; // Agrega el carácter '*' al token
            peekcharcurrent++;
            columna++;
            SkipWhiteSpace();
            return {TOKEN_OPER_MUL, word, fila, columnaActual}; // Operador de multiplicación
        }

        case '/':
        {
            word += *peekcharcurrent; // Agrega el carácter '/' al token
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
            word += *peekcharcurrent; // Agregar la primera comilla simple a la palabra
            peekcharcurrent++;
            columna++;

            // Leer el carácter entre las comillas simples
            if (*peekcharcurrent != '\'' && *peekcharcurrent != '\0')
            {
                word += *peekcharcurrent; // Agregar el carácter a la palabra
                peekcharcurrent++;
                columna++;
            }
            else
            {
                // Error: no se encontró un carácter entre comillas simples
                return {TOKEN_UNKNOWN, word, fila, columnaActual};
            }

            // Esperar la comilla simple de cierre
            if (*peekcharcurrent == '\'')
            {
                word += *peekcharcurrent; // Agregar la comilla de cierre
                peekcharcurrent++;
                columna++;
            }
            else
            {
                // Error: no se encontró la comilla simple de cierre
                return {TOKEN_UNKNOWN, word, fila, columnaActual};
            }

            SkipWhiteSpace();
            return {TOKEN_CHAR, word, fila, columnaActual};

        case '"':
            word += *peekcharcurrent; // Agregar la primera comilla a la palabra
            peekcharcurrent++;
            columna++;

            // Recorremos los caracteres hasta encontrar la comilla de cierre o fin de archivo
            while (*peekcharcurrent != '"' && *peekcharcurrent != '\0')
            {
                word += *peekcharcurrent; // Agregar los caracteres de la cadena
                peekcharcurrent++;
                columna++;
            }

            if (*peekcharcurrent == '"')
            {
                word += *peekcharcurrent; // Agregar la comilla de cierre
                peekcharcurrent++;
                columna++;
            }
            else
            {
                // Si llegamos al final de la línea o archivo sin encontrar la comilla de cierre
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
                    // Encontramos el cierre del comentario de bloque "*/"
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

            getcharcurrent = peekcharcurrent;
            continue;
        }

        while (*getcharcurrent != '\n' && *getcharcurrent != '\0')
        {
            SkipWhiteSpace(); // Ignorar espacios en blanco y comentarios

            // Si SkipWhiteSpace ha ignorado todo (como comentarios), continuar
            if (*peekcharcurrent == '\0' || *peekcharcurrent == '\n') {
                break; 
            }

            // Procesar el siguiente token usando peekChar
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

    for (auto token : tokens)
    {
        cout << "Token: " << token.value << ", Tipo: " << tokenMap[token.token]
             << ", Linea: " << token.fila << ", Columna: " << token.columna << endl;
    }

    return 0;
}