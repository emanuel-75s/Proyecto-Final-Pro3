// Arbol avl.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <queue>
#include <iomanip>
#include <cmath>
using namespace std;
struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;
    Nodo(int val) : dato(val), izq(nullptr), der(nullptr) {}
};

struct NodoAVL {
    int dato, altura;
    NodoAVL* izq, * der;
    NodoAVL(int val) : dato(val), altura(1), izq(nullptr), der(nullptr) {}
};
class AVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* nodo) {
        return nodo ? nodo->altura : 0;
    }

    int balanceFactor(NodoAVL* nodo) {
        return altura(nodo->der) - altura(nodo->izq);
    }

    void actualizarAltura(NodoAVL* nodo) {
        nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    }

    NodoAVL* rotarDerecha(NodoAVL* y) {
        NodoAVL* x = y->izq;
        y->izq = x->der;
        x->der = y;
        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    NodoAVL* rotarIzquierda(NodoAVL* x) {
        NodoAVL* y = x->der;
        x->der = y->izq;
        y->izq = x;
        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    NodoAVL* balancear(NodoAVL* nodo) {
        actualizarAltura(nodo);
        int bf = balanceFactor(nodo);

        if (bf == 2) {
            if (balanceFactor(nodo->der) < 0)
                nodo->der = rotarDerecha(nodo->der);
            return rotarIzquierda(nodo);
        }
        if (bf == -2) {
            if (balanceFactor(nodo->izq) > 0)
                nodo->izq = rotarIzquierda(nodo->izq);
            return rotarDerecha(nodo);
        }
        return nodo;
    }

    NodoAVL* insertar(NodoAVL* nodo, int val) {
        if (!nodo) return new NodoAVL(val);

        if (val < nodo->dato) {
            nodo->izq = insertar(nodo->izq, val);
        }
        else if (val > nodo->dato) {
            nodo->der = insertar(nodo->der, val);
        }
        else {
            return nodo; // No se permiten duplicados
        }

        return balancear(nodo);
    }

    NodoAVL* encontrarMinimo(NodoAVL* nodo) {
        while (nodo && nodo->izq) nodo = nodo->izq;
        return nodo;
    }

    NodoAVL* eliminar(NodoAVL* nodo, int val) {
        if (!nodo) return nodo;

        if (val < nodo->dato) {
            nodo->izq = eliminar(nodo->izq, val);
        }
        else if (val > nodo->dato) {
            nodo->der = eliminar(nodo->der, val);
        }
        else {
            if (!nodo->izq || !nodo->der) {
                NodoAVL* temp = nodo->izq ? nodo->izq : nodo->der;
                if (!temp) {
                    temp = nodo;
                    nodo = nullptr;
                }
                else {
                    *nodo = *temp;
                }
                delete temp;
            }
            else {
                NodoAVL* temp = encontrarMinimo(nodo->der);
                nodo->dato = temp->dato;
                nodo->der = eliminar(nodo->der, temp->dato);
            }
        }

        if (!nodo) return nodo;

        return balancear(nodo);
    }

    bool buscar(NodoAVL* nodo, int val) {
        if (!nodo) return false;
        if (nodo->dato == val) return true;
        if (val < nodo->dato) return buscar(nodo->izq, val);
        return buscar(nodo->der, val);
    }

    void mostrarInOrden(NodoAVL* nodo) {
        if (!nodo) return;
        mostrarInOrden(nodo->izq);
        cout << nodo->dato << " ";
        mostrarInOrden(nodo->der);
    }

    void mostrarPreOrden(NodoAVL* nodo) {
        if (!nodo) return;
        cout << nodo->dato << " ";
        mostrarPreOrden(nodo->izq);
        mostrarPreOrden(nodo->der);
    }

    void mostrarPostOrden(NodoAVL* nodo) {
        if (!nodo) return;
        mostrarPostOrden(nodo->izq);
        mostrarPostOrden(nodo->der);
        cout << nodo->dato << " ";
    }

    int tamano(NodoAVL* nodo) {
        if (!nodo) return 0;
        return 1 + tamano(nodo->izq) + tamano(nodo->der);
    }

    void destruirArbol(NodoAVL* nodo) {
        if (nodo) {
            destruirArbol(nodo->izq);
            destruirArbol(nodo->der);
            delete nodo;
        }
    }

public:
    AVL() : raiz(nullptr) {}
    ~AVL() { destruirArbol(raiz); }

    void insertar(int val) {
        raiz = insertar(raiz, val);
    }

    void eliminar(int val) {
        raiz = eliminar(raiz, val);
    }

    bool buscar(int val) {
        return buscar(raiz, val);
    }

    void mostrar(const string& tipoRecorrido) {
        cout << "Recorrido " << tipoRecorrido << ": ";
        if (tipoRecorrido == "InOrden") mostrarInOrden(raiz);
        else if (tipoRecorrido == "PreOrden") mostrarPreOrden(raiz);
        else if (tipoRecorrido == "PostOrden") mostrarPostOrden(raiz);
        cout << endl;
    }



    void mostrarArbolConRamas() {
        if (!raiz) {
            cout << "El árbol está vacío." << endl;
            return;
        }

        queue<NodoAVL*> q;
        q.push(raiz);
        int nivel = 0;
        int maxNivel = altura(raiz);
        int ancho = pow(2, maxNivel + 1);

        while (!q.empty()) {
            int cantidad = q.size();
            int espacio = ancho / pow(2, nivel + 1);

            for (int i = 0; i < cantidad; ++i) {
                NodoAVL* temp = q.front();
                q.pop();

                cout << setw(espacio) << "";
                if (temp) {
                    cout << temp->dato;
                    q.push(temp->izq);
                    q.push(temp->der);
                }
                else {
                    cout << " ";
                    q.push(nullptr);
                    q.push(nullptr);
                }
                cout << setw(espacio) << "";
            }
            cout << "\n";


            if (nivel < maxNivel - 1) {
                for (int j = 0; j < cantidad; ++j) {
                    cout << setw(espacio - 1) << "";
                    cout << (j % 2 == 0 ? "/" : "\\");
                    cout << setw(espacio + 1) << "";
                }
                cout << "\n";
            }

            nivel++;
            if (nivel > maxNivel + 1) break;
        }

        cout << endl;
    }





    int obtenerAltura() {
        return altura(raiz);
    }

    int obtenerTamano() {
        return tamano(raiz);
    }

    bool estaVacio() {
        return raiz == nullptr;
    }

    void limpiar() {
        destruirArbol(raiz);
        raiz = nullptr;
    }

    bool estaBalanceado() {
        if (!raiz) return true;
        int bf = balanceFactor(raiz);
        return bf >= -1 && bf <= 1;
    }
};



void menuAVL() {
    AVL arbol;
    int opcion, valor;

    do {
        cout << " ___________________________________" << endl;
        cout << " |       MENU ARBOL AVL            |" << endl;
        cout << " |_________________________________|" << endl;
        cout << " | 1. Insertar elemento            | " << endl;
        cout << " | 2. Eliminar elemento            |" << endl;
        cout << " | 3. Buscar elemento              |" << endl;
        cout << " | 4. Mostrar recorrido PreOrden   |" << endl;
        cout << " | 5. Mostrar recorrido InOrden    |" << endl;
        cout << " | 6. Mostrar recorrido PostOrden  |" << endl;
        cout << " | 7. Obtener altura del arbol     |" << endl;
        cout << " | 8. Obtener tamano del arbol     |" << endl;
        cout << " | 9. Verificar balanceo           |" << endl;
        cout << " | 10. Verificar si esta vacio     |" << endl;
        cout << " | 11. Limpiar arbol               |" << endl;
        cout << " | 12. Mostrar arbol con ramas     |" << endl;
        cout << " | 0. Regresar al menu principal   |" << endl;
        cout << " |_________________________________|" << endl;
        cout << " |    Seleccione una opcion:       |" << endl;
        cout << " |_________________________________|" << endl;
        cin >> opcion;
        system("cls");

        switch (opcion) {
        case 1:
            cout << "Ingrese el valor a insertar: ";
            cin >> valor;
            arbol.insertar(valor);
            cout << "Valor insertado. Arbol " << (arbol.estaBalanceado() ? "balanceado" : "no balanceado") << endl;
            break;
        case 2:
            cout << "Ingrese el valor a eliminar: ";
            cin >> valor;
            arbol.eliminar(valor);
            cout << "Valor eliminado. Arbol " << (arbol.estaBalanceado() ? "balanceado" : "no balanceado") << endl;
            break;
        case 3:
            cout << "Ingrese el valor a buscar: ";
            cin >> valor;
            cout << (arbol.buscar(valor) ? "Encontrado" : "No encontrado") << endl;
            break;
        case 4:
            arbol.mostrar("PreOrden");
            break;
        case 5:
            arbol.mostrar("InOrden");
            break;
        case 6:
            arbol.mostrar("PostOrden");
            break;
        case 7:
            cout << "Altura del arbol: " << arbol.obtenerAltura() << endl;
            break;
        case 8:
            cout << "Tamano del arbol: " << arbol.obtenerTamano() << endl;
            break;
        case 9:
            cout << (arbol.estaBalanceado() ? "El arbol esta balanceado" : "El arbol NO esta balanceado") << endl;
            break;
        case 10:
            cout << (arbol.estaVacio() ? "El arbol esta vacio" : "El arbol no esta vacio") << endl;
            break;
        case 11:
            arbol.limpiar();
            cout << "Arbol limpiado correctamente" << endl;
            break;
        case 12:
            arbol.mostrarArbolConRamas();
            break;

        case 0:
            cout << "Regresando al menu principal..." << endl;
            break;
        default:
            cout << "Opcion no valida!" << endl;
        }
    } while (opcion != 0);
}
int main()
{
    menuAVL();
}



// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
