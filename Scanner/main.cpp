#include <iostream>
#include <fstream>
#include <string>
#include "../Scanner/reservadas.cpp"

using namespace std;

const char * peekChar(const char * word, int camino){
    string palabra = "";
    palabra += word;
    word++;
    if(*word == '\0') return word;  
    if(camino == 0){
        while(*word != ' '){
            palabra += *word;
            word++;
            if(*word == '+') break;
            else if(*word == '-') break;
        }
        if(reservedWord(palabra)) cout << "palabra reservada" << endl;
        word++;
        return word;
    }
    else if(camino == 1){
        while(*word != ' '){
            palabra += *word;
            word++;
            int sigNumero = *word;
            if(97 <= sigNumero <= 122 || 65 <= sigNumero <= 90) cout << "Es letra en lugar de numero" << endl;
        }
        word++;
        return word;
    }
    else{
        if(*word == ':'){
            palabra += *word;
            cout << token[palabra] << endl;
            word++;
        }
        else if(*word == '='){
            palabra += *word;
            cout << token[palabra] << endl;
            word++;
        }
        else if(*word == ';'){
            palabra += *word;
            word++;
            if(*word == ' '){
                cout << token[palabra] << endl;
                word++;
                return word;
            }
            else{
                cout << "ERROR" << endl;
                word++;
                return word;
            }

        }
        else if(*word == '+'){
            palabra += *word;
            word++;
            if(*word == ' '){
                cout << token[palabra] << endl;
                word++;
                return word;
            }
            else if(*word == '+'){
                cout << token[palabra] << endl;
                word += 2;
                return word;
            }
        }
    }
}

void getChar(string line)
{
    const char * iniValue = line.c_str();

    bool accepted = 1;

    while(*iniValue != '\0'){
        int value = *iniValue;
        if(97 <= value <= 122 || 65 <= value <= 90){ // Detección del primer digito [a-z] o [A-Z]
            iniValue = peekChar(iniValue, 0);
        }
        else if(48 <= value <= 57){ // Detección del primer dígito numérico (Preguntar si vale que empiece con cero o es un error).
            iniValue = peekChar(iniValue, 1);
        }
        else{
            iniValue = peekChar(iniValue, 2);
        }
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

    return 0;
}