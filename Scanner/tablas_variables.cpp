#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include "../Scanner/reservadas.cpp"
#include "../Scanner/parser.cpp"

using namespace std;

std::string inferType(const std::string &value) {
    if (value.empty()) return "unknown";
    if (std::all_of(value.begin(), value.end(), ::isdigit)) return "integer";
    if (value.find('.') != std::string::npos && 
        std::all_of(value.begin(), value.end(), [](char c) { return ::isdigit(c) || c == '.'; })) 
        return "float";
    if (value.front() == '"' && value.back() == '"') return "string";
    if (value.front() == '\'' && value.back() == '\'') return "char";
    return "unknown"; // Por defecto
}


struct Variables_por_tabla{
    std::string name;
    std::string type;
    std::string value;
    bool typeMismatch;

    //constructor
    Variables_por_tabla(const std::string &n, const std::string &t, const std::string &v): name(n), type(t), value(v), typeMismatch(false) {}
};

class tablas_variables_manager{
    private:

    struct tablas_variables
    {
        string table_name;
        vector<Variables_por_tabla> variables;
    };

    tablas_variables globalTable;
    vector<tablas_variables> tablas;
    
    public:

    tablas_variables_manager() : globalTable{"Global",{}}{}

    void addtable(const string table_name){
        tablas.emplace_back(tablas_variables{table_name,{}});
    }

    void addGlobalVariable(const std::string &name, const std::string &type, const std::string &value){

        auto it =find_if(globalTable.variables.begin(), globalTable.variables.end(),
                               [&name](const Variables_por_tabla &entry) { return entry.name == name; });

        if (it != globalTable.variables.end()) {
            // Variable global ya existe: verificar compatibilidad de tipos
            if (it->type != type) {
                it->typeMismatch = true;
                std::cerr << "Type mismatch for global variable '" << name 
                          << "': expected '" << it->type 
                          << "', got '" << type << "'\n";
            } else {
                it->typeMismatch = false; // Restaurar si el tipo coincide
            }
            it->value = value; // Actualizar valor
        } else {
            // Agregar nueva variable global
            globalTable.variables.emplace_back(name, type, value);
        }   
    }

    void addOrUpdateVariable(const std::string &functionName, const std::string &name, 
                         const std::string &type, const std::string &value) {
        // Verificar primero en la tabla global
        auto itGlobal = std::find_if(globalTable.variables.begin(), globalTable.variables.end(),
                                    [&name](const Variables_por_tabla &entry) { return entry.name == name; });

        if (itGlobal != globalTable.variables.end()) {
            // Variable ya existe en la tabla global
            if (itGlobal->type != type) {
                itGlobal->typeMismatch = true;
                std::cerr << "Type mismatch for global variable '" << name 
                        << "': expected '" << itGlobal->type 
                        << "', got '" << type << "'\n";
            } else {
                itGlobal->typeMismatch = false;
            }
            itGlobal->value = value;
            return;
        }

        if (functionName.empty()) {
            // Sin función actual, manejar como variable global
            addGlobalVariable(name, type, value);
        } else {
            // Buscar si la variable ya existe en la tabla de la función actual
            for (auto &table : tablas) {
                if (table.table_name == functionName) {
                    auto it = std::find_if(table.variables.begin(), table.variables.end(),
                                        [&name](const Variables_por_tabla &entry) { return entry.name == name; });

                    if (it != table.variables.end()) {
                        // Variable ya existe en la función
                        if (it->type != type) {
                            it->typeMismatch = true;
                            std::cerr << "Type mismatch for variable '" << name 
                                    << "' in function '" << functionName 
                                    << "': expected '" << it->type 
                                    << "', got '" << type << "'\n";
                        } else {
                            it->typeMismatch = false;
                        }
                        it->value = value;
                        return;
                    }
                }
            }

            // Si no se encuentra en la tabla actual, buscar en otras tablas
            for (const auto &table : tablas) {
                auto it = std::find_if(table.variables.begin(), table.variables.end(),
                                    [&name](const Variables_por_tabla &entry) { return entry.name == name; });

                if (it != table.variables.end()) {
                    // La variable ya existe en otra tabla
                    if (it->type != type) {
                        std::cerr << "Variable '" << name 
                                << "' already declared in table '" << table.table_name 
                                << "' with a different type: '" << it->type 
                                << "' (current attempt type: '" << type << "')\n";
                    } else {
                        std::cerr << "Variable '" << name 
                                << "' already declared in table '" << table.table_name 
                                << "' with the same type.\n";
                    }
                    return;
                }
            }

            // Si no se encuentra en ninguna tabla, agregar a la tabla actual
            for (auto &table : tablas) {
                if (table.table_name == functionName) {
                    table.variables.emplace_back(name, type, value);
                    return;
                }
            }
        }
}




    void printTables() const {

        cout<<"nombre: "<<globalTable.table_name<<endl;
        for(const auto &variable : globalTable.variables){
            cout<<"Variable: "<<variable.name<<endl
            <<"Type: "<<variable.type<<endl
            <<"Value:"<<variable.value<<endl
            <<(variable.typeMismatch ? "[Type Mismatch] " : " ")
            <<endl;
        }

        cout<<endl<<endl;

        for (const auto &tabla : tablas)
        {
            cout<<"nombre: "<<tabla.table_name<<endl;
            for(const auto &variable : tabla.variables){
                cout<<"Variable: "<<variable.name<<endl
                <<"Type: "<<variable.type<<endl
                <<"Value:"<<variable.value<<endl
                <<(variable.typeMismatch ? "[Type Mismatch] " : " ")
                <<endl;
            }

            cout<<endl<<endl;
        }
        
    }
};




template <typename NodeType>
void Build_Tabla_Variables(const typename ASTree::ASTNode<NodeType>::SharedPtr &node,
tablas_variables_manager &manager,
const string &currentname= "")
{
    if (!node) return;

    if (node->getText() == "FUNCTION")
    {
        string nombre_tabla=node->getChild(1)->getText();
        manager.addtable(nombre_tabla);  

        for (const auto &child : node->getChilds())
        {
            Build_Tabla_Variables<TokenType>(child,manager,nombre_tabla);
        }
    }else if (node->getText() == "VAR_DECL") {
        // Extraer información de la variable
        string varType = node->getChild(0)->getText();  // tipo de la variable
        string varName;
        string varValue;
        if (node->getChild(1)->getText()=="ASSIGN")
        {
            varName=node->getChild(1)->getChild(0)->getText();
            varValue=node->getChild(1)->getChild(1)->getText();
        }else{
            varName = node->getChild(1)->getText();  // Nombre de la variable
        }
        
       
        //std::string varValue = node->getChild(2)->getText(); // Valor asignado (si existe)

        manager.addOrUpdateVariable(currentname,varName,varType,varValue);
    }else if (node->getText() == "ASSIGN"){

        string varName = node->getChild(0)->getText();
        string varValue = node->getChild(1)->getText();
        string varType = inferType(varValue);

        manager.addOrUpdateVariable(currentname,varName,varType,varValue);
    }else {
         for (const auto &child : node->getChilds()) {
            Build_Tabla_Variables<NodeType>(child, manager, currentname);
        }
    }
}