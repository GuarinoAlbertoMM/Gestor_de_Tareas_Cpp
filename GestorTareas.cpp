#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <string>

using namespace std;

// Estructura para representar una tarea
struct Tarea {
    string titulo;
    string descripcion;
    int prioridad; // 1 = alta, 2 = media, 3 = baja
    string fechaVencimiento;
};

// Clase para gestionar la lista de tareas pendientes
class ListaTareas {
    vector<Tarea> tareas;

public:
    //Funcion para agregar una tarea
    void agregarTarea(const Tarea& tarea) {
        tareas.push_back(tarea);
        ordenarTareas();
    }

    //Funcion para eliminar una tarea
    void eliminarTarea(int indice) {
        if (indice >= 0 && indice < tareas.size()) {
            tareas.erase(tareas.begin() + indice);
        }
    }

    //Funcion para modificar una tarea
    void modificarTarea(int indice, const Tarea& nuevaTarea) {
        if (indice >= 0 && indice < tareas.size()) {
            tareas[indice] = nuevaTarea;
            ordenarTareas();
        }
    }

    //Funcion para mostrar las tareas
    void mostrarTareas() {
        cout << "\nLista de Tareas:\n";
        for (size_t i = 0; i < tareas.size(); ++i) {
            cout << i + 1 << ". " << tareas[i].titulo 
                 << " - Prioridad: " << tareas[i].prioridad 
                 << " - Fecha de vencimiento: " << tareas[i].fechaVencimiento << endl;
        }
        if (tareas.empty()) {
            cout << "No hay tareas pendientes.\n";
        }
    }

private:
    //Ordena las tareas por prioridad
    void ordenarTareas() {
        sort(tareas.begin(), tareas.end(), [](const Tarea& a, const Tarea& b) {
            if (a.prioridad == b.prioridad) {
                return a.fechaVencimiento < b.fechaVencimiento;
            }
            return a.prioridad < b.prioridad;
        });
    }
};

// Clase para gestionar el historial de acciones con pilas
class HistorialAcciones {
    stack<string> historial;
    stack<string> rehacer;

public:
    void registrarAccion(const string& accion) {
        historial.push(accion);
        // Limpiar la pila de rehacer cuando se añade una nueva acción
        while (!rehacer.empty()) rehacer.pop();
    }

    void deshacerAccion() {
        if (!historial.empty()) {
            string ultimaAccion = historial.top();
            historial.pop();
            rehacer.push(ultimaAccion);
            cout << "Deshacer: " << ultimaAccion << endl;
        } else {
            cout << "No hay acciones para deshacer.\n";
        }
    }

    void rehacerAccion() {
        if (!rehacer.empty()) {
            string accion = rehacer.top();
            rehacer.pop();
            historial.push(accion);
            cout << "Rehacer: " << accion << endl;
        } else {
            cout << "No hay acciones para rehacer.\n";
        }
    }
};

// Clase para gestionar la cola de tareas urgentes
class ColaTareasUrgentes {
    queue<Tarea> colaUrgentes;

public:
    void agregarTareaUrgente(const Tarea& tarea) {
        colaUrgentes.push(tarea);
    }

    void procesarTareaUrgente() {
        if (!colaUrgentes.empty()) {
            Tarea tarea = colaUrgentes.front();
            colaUrgentes.pop();
            cout << "Procesando tarea urgente: " << tarea.titulo << endl;
        } else {
            cout << "No hay tareas urgentes.\n";
        }
    }
};

// Nodo para la organización jerárquica de tareas
class NodoCategoria {
public:
    string nombreCategoria;
    vector<NodoCategoria*> subcategorias;
    vector<Tarea> tareas;

    NodoCategoria(string nombre) : nombreCategoria(nombre) {}

    void agregarSubcategoria(NodoCategoria* subcat) {
        subcategorias.push_back(subcat);
    }

    void agregarTarea(const Tarea& tarea) {
        tareas.push_back(tarea);
    }

    void mostrarTareas() {
        cout << nombreCategoria << ":\n";
        for (const auto& tarea : tareas) {
            cout << "- " << tarea.titulo << " (Prioridad: " << tarea.prioridad << ")" << endl;
        }
        for (auto& subcat : subcategorias) {
            subcat->mostrarTareas();
        }
    }
};

int main() {
    ListaTareas listaTareas;
    HistorialAcciones historialAcciones;
    ColaTareasUrgentes colaUrgentes;

    NodoCategoria trabajo("Trabajo");
    NodoCategoria personal("Personal");
    NodoCategoria estudios("Estudios");

    int opcion = 0;

    do {
        cout << "\n--- Aplicación de Gestión de Tareas ---\n";
        cout << "1. Agregar tarea\n";
        cout << "2. Eliminar tarea\n";
        cout << "3. Modificar tarea\n";
        cout << "4. Mostrar lista de tareas\n";
        cout << "5. Deshacer acción\n";
        cout << "6. Rehacer acción\n";
        cout << "7. Agregar tarea urgente\n";
        cout << "8. Procesar tarea urgente\n";
        cout << "9. Mostrar tareas organizadas en categorías\n";
        cout << "10. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        if (opcion == 1) {
            Tarea tarea;
            cout << "Título: ";
            cin.ignore();
            getline(cin, tarea.titulo);
            cout << "Descripción: ";
            getline(cin, tarea.descripcion);
            cout << "Prioridad (1 = alta, 2 = media, 3 = baja): ";
            cin >> tarea.prioridad;
            cout << "Fecha de vencimiento (YYYY-MM-DD): ";
            cin >> tarea.fechaVencimiento;

            listaTareas.agregarTarea(tarea);
            historialAcciones.registrarAccion("Agregar tarea: " + tarea.titulo);
        }
        else if (opcion == 2) {
            listaTareas.mostrarTareas();
            int indice;
            cout << "Ingrese el número de la tarea a eliminar: ";
            cin >> indice;
            listaTareas.eliminarTarea(indice - 1);
            historialAcciones.registrarAccion("Eliminar tarea " + to_string(indice));
        }
        else if (opcion == 3) {
            listaTareas.mostrarTareas();
            int indice;
            cout << "Ingrese el número de la tarea a modificar: ";
            cin >> indice;
            
            Tarea nuevaTarea;
            cout << "Nuevo título: ";
            cin.ignore();
            getline(cin, nuevaTarea.titulo);
            cout << "Nueva descripción: ";
            getline(cin, nuevaTarea.descripcion);
            cout << "Nueva prioridad (1 = alta, 2 = media, 3 = baja): ";
            cin >> nuevaTarea.prioridad;
            cout << "Nueva fecha de vencimiento (YYYY-MM-DD): ";
            cin >> nuevaTarea.fechaVencimiento;

            listaTareas.modificarTarea(indice - 1, nuevaTarea);
            historialAcciones.registrarAccion("Modificar tarea " + to_string(indice));
        }
        else if (opcion == 4) {
            listaTareas.mostrarTareas();
        }
        else if (opcion == 5) {
            historialAcciones.deshacerAccion();
        }
        else if (opcion == 6) {
            historialAcciones.rehacerAccion();
        }
        else if (opcion == 7) {
            Tarea tareaUrgente;
            cout << "Título: ";
            cin.ignore();
            getline(cin, tareaUrgente.titulo);
            cout << "Descripción: ";
            getline(cin, tareaUrgente.descripcion);
            cout << "Prioridad (1 = alta, 2 = media, 3 = baja): ";
            cin >> tareaUrgente.prioridad;
            cout << "Fecha de vencimiento (YYYY-MM-DD): ";
            cin >> tareaUrgente.fechaVencimiento;

            colaUrgentes.agregarTareaUrgente(tareaUrgente);
        }
        else if (opcion == 8) {
            colaUrgentes.procesarTareaUrgente();
        }
        else if (opcion == 9) {
            trabajo.mostrarTareas();
            personal.mostrarTareas();
            estudios.mostrarTareas();
        }

    } while (opcion != 10);

    return 0;
}
