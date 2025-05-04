#include <iostream>
#include <vector>

using namespace std;

struct NodoB {
    bool hoja;
    vector<int> claves;
    vector<NodoB*> hijos;
    NodoB(bool esHoja) : hoja(esHoja) {}
};

class ArbolB {
private:
    NodoB* raiz;
    int grado;

    void dividirHijo(NodoB* padre, int indice) {
        NodoB* hijo = padre->hijos[indice];
        NodoB* nuevoHijo = new NodoB(hijo->hoja);

        for (int i = 0; i < grado - 1; i++) {
            nuevoHijo->claves.push_back(hijo->claves[i + grado]);
        }

        if (!hijo->hoja) {
            for (int i = 0; i < grado; i++) {
                nuevoHijo->hijos.push_back(hijo->hijos[i + grado]);
            }
            hijo->hijos.resize(grado);
        }

        int claveMediana = hijo->claves[grado - 1];
        hijo->claves.resize(grado - 1);

        padre->claves.insert(padre->claves.begin() + indice, claveMediana);
        padre->hijos.insert(padre->hijos.begin() + indice + 1, nuevoHijo);
    }

    void insertarNoLleno(NodoB* nodo, int clave) {
        int i = nodo->claves.size() - 1;

        if (nodo->hoja) {
            nodo->claves.push_back(0);
            while (i >= 0 && clave < nodo->claves[i]) {
                nodo->claves[i + 1] = nodo->claves[i];
                i--;
            }
            nodo->claves[i + 1] = clave;
        }
        else {
            while (i >= 0 && clave < nodo->claves[i]) {
                i--;
            }
            i++;

            if (nodo->hijos[i]->claves.size() == (2 * grado - 1)) {
                dividirHijo(nodo, i);
                if (clave > nodo->claves[i]) {
                    i++;
                }
            }
            insertarNoLleno(nodo->hijos[i], clave);
        }
    }

    bool buscar(NodoB* nodo, int clave) {
        if (!nodo) return false;

        int i = 0;
        while (i < nodo->claves.size() && clave > nodo->claves[i]) {
            i++;
        }

        if (i < nodo->claves.size() && clave == nodo->claves[i]) {
            return true;
        }

        if (nodo->hoja) {
            return false;
        }

        return buscar(nodo->hijos[i], clave);
    }

    void mostrarInOrden(NodoB* nodo) {
        if (!nodo) return;

        for (int i = 0; i < nodo->claves.size(); i++) {
            if (!nodo->hoja) {
                mostrarInOrden(nodo->hijos[i]);
            }
            cout << nodo->claves[i] << " ";
        }

        if (!nodo->hoja) {
            mostrarInOrden(nodo->hijos.back());
        }
    }

    void destruirArbol(NodoB* nodo) {
        if (nodo) {
            if (!nodo->hoja) {
                for (auto hijo : nodo->hijos) {
                    destruirArbol(hijo);
                }
            }
            delete nodo;
        }
    }

public:
    ArbolB(int g) : raiz(nullptr), grado(g) {}
    ~ArbolB() { destruirArbol(raiz); }

    void insertar(int clave) {
        if (!raiz) {
            raiz = new NodoB(true);
            raiz->claves.push_back(clave);
            return;
        }

        if (raiz->claves.size() == (2 * grado - 1)) {
            NodoB* nuevaRaiz = new NodoB(false);
            nuevaRaiz->hijos.push_back(raiz);
            dividirHijo(nuevaRaiz, 0);
            raiz = nuevaRaiz;
        }

        insertarNoLleno(raiz, clave);
    }

    bool buscar(int clave) {
        return buscar(raiz, clave);
    }

    void mostrar() {
        cout << "Recorrido InOrden: ";
        mostrarInOrden(raiz);
        cout << endl;
    }

    bool estaVacio() {
        return raiz == nullptr;
    }

    void limpiar() {
        destruirArbol(raiz);
        raiz = nullptr;
    }
};

int main() {
    int grado;
    cout << "Ingrese el grado del arbol B (minimo 2): ";
    cin >> grado;

    if (grado < 2) {
        cout << "Grado no valido. Usando grado 2 por defecto." << endl;
        grado = 2;
    }

    ArbolB arbol(grado);
    int opcion, valor;

    do {
        cout << "|--------------------------------------|" << endl;
        cout << "|        ARBOL B (GRADO grado )        | " << endl;
        cout << "|--------------------------------------|" << endl;
        cout << "| 1. Insertar elemento                 |" << endl;
        cout << "| 2. Buscar elemento                   |" << endl;
        cout << "| 3. Mostrar recorrido InOrden         |" << endl;
        cout << "| 4. Verificar si esta vacio           |" << endl;
        cout << "| 5. Limpiar arbol                     |" << endl;
        cout << "| 0. Salir                             |" << endl;
        cout << "|--------------------------------------|" << endl;
        cout << "| Seleccione una opcion:               | " << endl;
        cout << "|--------------------------------------|" << endl;
        cin >> opcion;
        system("cls");

        switch (opcion) {
        case 1:
            cout << "Ingrese el valor a insertar: ";
            cin >> valor;
            arbol.insertar(valor);
            break;
        case 2:
            cout << "Ingrese el valor a buscar: ";
            cin >> valor;
            cout << (arbol.buscar(valor) ? "Encontrado" : "No encontrado") << endl;
            break;
        case 3:
            arbol.mostrar();
            break;
        case 4:
            cout << (arbol.estaVacio() ? "El arbol esta vacio" : "El arbol no esta vacio") << endl;
            break;
        case 5:
            arbol.limpiar();
            cout << "Arbol limpiado correctamente" << endl;
            break;
        case 0:
            cout << "Saliendo del programa..." << endl;
            break;
        default:
            cout << "Opcion no valida!" << endl;
        }
    } while (opcion != 0);


    return 0;
}