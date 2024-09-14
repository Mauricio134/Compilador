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
        }
        SkipWhiteSpace();
        return { TOKEN_ID, word, fila, columnaActual };
    }
    else{
        word += *peekcharcurrent;
        peekcharcurrent++;
        columna++;
        SkipWhiteSpace();
        return { TOKEN_UNKNOWN, word, fila, columnaActual };
    }
}


void getChar(string linea){
    getcharcurrent = peekcharcurrent = linea.c_str();
    while(*getcharcurrent != '\0'){
        while(*getcharcurrent != '\n' && *getcharcurrent != '\0') {

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