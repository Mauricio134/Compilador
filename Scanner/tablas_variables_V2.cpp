#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "../Scanner/reservadas.cpp"
#include "../Scanner/parser.cpp"

using namespace std;

std::string inferType(const std::string &value) {
    if (value.empty()) 
        return "unknown";

    // Verificar si es un número entero
    if ((value[0] == '-' && value.size() > 1 && 
        std::all_of(value.begin() + 1, value.end(), ::isdigit)) || 
        std::all_of(value.begin(), value.end(), ::isdigit)) {
        return "integer";
    }

    // Verificar si es un número flotante
    bool hasDot = std::count(value.begin(), value.end(), '.') == 1;
    if (hasDot) {
        size_t dotPos = value.find('.');
        bool validFloat = (dotPos != std::string::npos) &&
                          (dotPos > 0 && value[dotPos - 1] != '-') && // Asegurarse de que hay algo antes y después del punto
                          (std::all_of(value.begin(), value.begin() + dotPos, [](char c) { return c == '-' || ::isdigit(c); }) &&
                           std::all_of(value.begin() + dotPos + 1, value.end(), ::isdigit));
        if (validFloat)
            return "float";
    }

    // Verificar si es un string
    if (value.front() == '"' && value.back() == '"') 
        return "string";

    // Verificar si es un char
    if (value.front() == '\'' && value.back() == '\'' && value.size() == 3) 
        return "char";

    return "unknown"; // Por defecto
}


struct Variables_por_tabla {
    std::string name;
    std::string type;
    std::string value;
    bool typeMismatch = false;

    Variables_por_tabla(const std::string &name, const std::string &type, const std::string &value)
        : name(name), type(type), value(value) {}
};

struct TablaVariables {
    std::string tableName;
    std::vector<Variables_por_tabla> variables;

    TablaVariables(const std::string &name) : tableName(name) {}

    Variables_por_tabla* findVariable(const std::string &name) {
        auto it = std::find_if(variables.begin(), variables.end(),
                               [&name](const Variables_por_tabla &var) { return var.name == name; });
        return it != variables.end() ? &(*it) : nullptr;
    }

    void addOrUpdateVariable(const std::string &name, const std::string &type, const std::string &value) {
        auto var = findVariable(name);
        if (var) {
            if (var->type != type) {
                var->typeMismatch = true;
                std::cerr << "Type mismatch for variable '" << name
                          << "': expected '" << var->type
                          << "', got '" << type << "'\n";
            } else {
                var->typeMismatch = false;
            }
            var->value = value;
        } else {
            variables.emplace_back(name, type, value);
        }
    }
};

class tablas_variables_manager {
private:
    TablaVariables globalTable{"Global"};
    std::vector<TablaVariables> scopeStack;  // Cambiado a vector
    std::vector<TablaVariables> finalTables;

public:
    tablas_variables_manager() {
        scopeStack.push_back(globalTable);  // Usar push_back
    }

    void enterScope(const std::string &scopeName) {
        scopeStack.push_back(TablaVariables(scopeName));  // Usar push_back
    }

    void exitScope() {
        if (scopeStack.size() > 1) {
            finalTables.push_back(scopeStack.back());  // Usar back
            scopeStack.pop_back();  // Usar pop_back
        } else {
            std::cerr << "Error: Attempted to exit global scope.\n";
        }
    }

    void addOrUpdateVariable(const std::string &name, const std::string &type, const std::string &value) {
        if (scopeStack.empty()) {
            std::cerr << "Error: No active scope for variable '" << name << "'.\n";
            return;
        }
        scopeStack.back().addOrUpdateVariable(name, type, value);  // Usar back
    }

    Variables_por_tabla* findVariable(const std::string &name) {
        // Iterar sobre el vector desde el final hacia el principio
        for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
            Variables_por_tabla *var = it->findVariable(name);
            if (var) {
                return var;
            }
        }
        return nullptr;  // No se encontró la variable
    }

    void finalizeGlobalTable() {
        if (!scopeStack.empty() && scopeStack.back().tableName == "Global") {
            finalTables.push_back(scopeStack.back());  // Usar back
        }
    }

    void printTables() const {
        std::cout << "=== Final Tables ===\n";
        for (const auto &table : finalTables) {
            std::cout << "Table: " << table.tableName << "\n";
            for (const auto &var : table.variables) {
                std::cout << "  Variable: " << var.name
                          << ", Type: " << var.type
                          << ", Value: " << var.value
                          << (var.typeMismatch ? " [Type Mismatch]" : "")
                          << "\n";
            }
            std::cout << "---------------------------\n";
        }
    }
};

template <typename NodeType>
void Build_Tabla_Variables(const typename ASTree::ASTNode<NodeType>::SharedPtr &node,
                           tablas_variables_manager &manager,
                           const std::string &currentScope = "Global") {
    if (!node) return;

    if (node->getText() == "FUNCTION") {
        std::string functionName = node->getChild(1)->getText();
        manager.enterScope(functionName);

        for (const auto &child : node->getChilds()) {
            Build_Tabla_Variables<NodeType>(child, manager, functionName);
        }

        manager.exitScope();
    } else if (node->getText() == "VAR_DECL") {
        std::string varType = node->getChild(0)->getText();
        std::string varName;
        std::string varValue;

        if (node->getChild(1)->getText() == "ASSIGN") {
            varName = node->getChild(1)->getChild(0)->getText();
            varValue = node->getChild(1)->getChild(1)->getText();
        } else {
            varName = node->getChild(1)->getText();
        }

        manager.addOrUpdateVariable(varName, varType, varValue);
    } else if (node->getText() == "ASSIGN") {
        std::string varName = node->getChild(0)->getText(); // Nombre de la variable
        std::string newValue = node->getChild(1)->getText(); // Nuevo valor

        Variables_por_tabla *var = manager.findVariable(varName);
        if (var) {
            std::string newType = inferType(newValue);
            if (var->type == newType) {
                
                var->typeMismatch = false;
                var->value = newValue; // Actualiza el valor directamente
            } else {
                
                if(newType=="unknown"){
                    Variables_por_tabla *auxiliar_tipo_variable_existente=manager.findVariable(newValue);
                    if(auxiliar_tipo_variable_existente && auxiliar_tipo_variable_existente->type==var->type){
                        var->value = auxiliar_tipo_variable_existente->value;
                        var->typeMismatch=false;
                    }else{
                        var->typeMismatch = true;
                        var->value = newValue; // Actualiza el valor directamente
                        std::cout << "Error: Type mismatch for variable '" << varName
                                << "': expected '" << var->type
                                << "', got '" << newType << "'\n";}
                }else{
                    var->typeMismatch = true;
                    var->value = newValue; // Actualiza el valor directamente
                    std::cout << "Error: Type mismatch for variable '" << varName
                            << "': expected '" << var->type
                            << "', got '" << newType << "'\n";}
            }
        } else {
            std::cout << "Error: Variable '" << varName << "' not declared before assignment.\n";
        }
    }else if (node ->getText()=="IfStmt")
    {
        manager.enterScope("IF_Table");
        Build_Tabla_Variables<NodeType>(node->getChild(1), manager, "IF_Table");
        manager.exitScope();
    }
    else if (node ->getText()=="ForStmt")
    {
        manager.enterScope("FOR_Table");

        for (const auto &child : node->getChilds()) {
            Build_Tabla_Variables<NodeType>(child, manager, "FOR_Table");
        }

        manager.exitScope();
    }
    else {
        for (const auto &child : node->getChilds()) {
            Build_Tabla_Variables<NodeType>(child, manager, currentScope);
        }
    }
}