#include <iostream>
#include <fstream>
#include <string>
#include "../Scanner/reservadas.cpp"

using namespace std;

void SkipWhiteSpace() {
    // Continúa leyendo mientras el carácter actual sea un espacio en blanco
    while (isspace(*peekcharcurrent)) {
        peekcharcurrent++;
        columna++;
    }
}

Token peekChar(){
    word = "";
    int columnaActual = columna;
    if (isdigit(*peekcharcurrent)) {
        while(isdigit(*peekcharcurrent)) {
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
        }
        if(isalpha(*peekcharcurrent)){
            while(isdigit(*peekcharcurrent) || isalpha(*peekcharcurrent)){
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
            }
            return { TOKEN_UNKNOWN, word, fila, columnaActual };
        }
        SkipWhiteSpace();
        return { TOKEN_INTEGER, word, fila, columnaActual };
    }
    else if(isalpha(*peekcharcurrent) || *peekcharcurrent == '_'){
        while(isalpha(*peekcharcurrent) || *peekcharcurrent == '_'){
            word += *peekcharcurrent;
            peekcharcurrent++;
            columna++;
        }
        if(isdigit(*peekcharcurrent)){
            while(isdigit(*peekcharcurrent) || isalpha(*peekcharcurrent) || *peekcharcurrent == '_'){
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
            }
            return { TOKEN_UNKNOWN, word, fila, columnaActual };
        }else{//Revisar si lo que hemos leido es palabra reservada
            int contador_palabras_reservadas=0;
            if(reservadas.count(word)){
                return{Mapa_tokens.find(word)->second,word,fila,columnaActual};
            };
            
        }
        SkipWhiteSpace();
        return { TOKEN_ID, word, fila, columnaActual };
    }
    else{
        bool doble = 0;
        int d = 0;
        switch(*peekcharcurrent){
            case ':':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_DECLAR, word, fila, columnaActual };
            case '=':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '='){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_COMP_E, word, fila, columnaActual };
                return { TOKEN_ASIG, word, fila, columnaActual };
            case ';':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_DELIM_SC, word, fila, columnaActual };
            case '[':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_DELIM_B_O, word, fila, columnaActual };
            case ']':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_DELIM_B_C, word, fila, columnaActual };
            case '(':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_DELIM_P_O, word, fila, columnaActual };
            case ')':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_DELIM_P_C, word, fila, columnaActual };
            case '+':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '+'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_S_SUMA, word, fila, columnaActual };
                return { TOKEN_OPER_SUM, word, fila, columnaActual };
            case '-':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '-'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_S_RESTA, word, fila, columnaActual };
                return { TOKEN_OPER_REST, word, fila, columnaActual };
            case '!':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '='){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_COMP_D, word, fila, columnaActual };
                return { TOKEN_OPER_EX, word, fila, columnaActual };
            case '^':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_OPER_EXP, word, fila, columnaActual };
            case '*':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '/'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_COMEN_CLOSE, word, fila, columnaActual };
                return { TOKEN_OPER_MUL, word, fila, columnaActual };
            case '/':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '*'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                else if(*peekcharcurrent == '/'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 2;
                }
                SkipWhiteSpace();
                if(d == 1) return { TOKEN_COMEN_OPEN, word, fila, columnaActual };
                else if(d == 2) return { TOKEN_COMEN, word, fila, columnaActual };
                return { TOKEN_OPER_DIV, word, fila, columnaActual };
            case '%':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_OPER_MOD, word, fila, columnaActual };
            case '<':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '='){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_MEN_E, word, fila, columnaActual };
                return { TOKEN_OPER_MENOR, word, fila, columnaActual };
            case '>':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                if(*peekcharcurrent == '='){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    columna++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_MAY_E, word, fila, columnaActual };
                return { TOKEN_OPER_MAYOR, word, fila, columnaActual };
            case '&':
                word += *peekcharcurrent;
                peekcharcurrent++;
                if(*peekcharcurrent == '&'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_AND, word, fila, columnaActual };
                return { TOKEN_UNKNOWN, word, fila, columnaActual };
            case '|':
                word += *peekcharcurrent;
                peekcharcurrent++;
                if(*peekcharcurrent == '|'){
                    word += *peekcharcurrent;
                    peekcharcurrent++;
                    doble = 1;
                }
                SkipWhiteSpace();
                if(doble) return { TOKEN_OPER_OR, word, fila, columnaActual };
                return { TOKEN_UNKNOWN, word, fila, columnaActual };
            case '{':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_LL_O, word, fila, columnaActual };
            case '}':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_LL_C, word, fila, columnaActual };
            case ',':
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_COMA, word, fila, columnaActual };
            default:
                word += *peekcharcurrent;
                peekcharcurrent++;
                columna++;
                SkipWhiteSpace();
                return { TOKEN_UNKNOWN, word, fila, columnaActual };
        }
    }
}


void getChar(string linea){
    getcharcurrent = peekcharcurrent = linea.c_str();
    while(*getcharcurrent != '\0'){
        while(*getcharcurrent != '\n' && *getcharcurrent != '\0') {
            SkipWhiteSpace();
            if (isalpha(*getcharcurrent) || *getcharcurrent == '_' || isdigit(*getcharcurrent)) {
                tokens.push_back(peekChar());
                getcharcurrent = peekcharcurrent;
            }
            else{
                tokens.push_back(peekChar());
                getcharcurrent = peekcharcurrent;
            }
        }
        fila++;
        columna = 0;
    }
}

int main()
{
    string name = "B-minor.txt";

    ifstream archivo(name);

    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo " << name << endl;
        return 1;
    }

    string linea;

    while (getline(archivo, linea))
    {
        getChar(linea);
    }

    for(auto token : tokens){
        cout << "Token: " << token.value << ", Tipo: " << token.token
        << ", Linea: " << token.fila << ", Columna: " << token.columna << endl;
    }

    return 0;
}