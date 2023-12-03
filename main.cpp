#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <climits>
#include <cstdlib>

using namespace std;

const int filas = 6;
const int columnas = 7;
const int LONGITUD_GANADORA = 4;

enum Jugador {
    VACIO,
    JUGADOR1,
    JUGADOR2
};

struct Movimiento {
    int fila, columna;
};

class Conecta4 {
private:
    Jugador tablero[filas][columnas];
    int dificultad;
    int victoriasJugador, victoriasIA;
    
public:
    Conecta4(int _dificultad) : dificultad(_dificultad), victoriasJugador(0), victoriasIA(0) {
        
        // Inicializar el tablero con valores VACIO
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                tablero[i][j] = VACIO;
            }
        }
    }

    void imprimirTablero() {
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                string ficha = (tablero[i][j] == VACIO) ? ". " : (tablero[i][j] == JUGADOR1) ? "X " : "O ";
                cout << " " << ficha;
            }
            cout << "\n";
        }

        cout << "---------------------\n";
        cout << " 1  2  3  4  5  6  7 \n";
    }

    bool esGanador(Jugador jugador) {
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j <= columnas - LONGITUD_GANADORA; ++j) {
                bool ganado = true;
                for (int k = 0; k < LONGITUD_GANADORA; ++k) {
                    if (tablero[i][j + k] != jugador) {
                        ganado = false;
                        break;
                    }
                }
                if (ganado) return true;
            }
        }

        for (int i = 0; i <= filas - LONGITUD_GANADORA; ++i) {
            for (int j = 0; j < columnas; ++j) {
                bool ganado = true;
                for (int k = 0; k < LONGITUD_GANADORA; ++k) {
                    if (tablero[i + k][j] != jugador) {
                        ganado = false;
                        break;
                    }
                }
                if (ganado) return true;
            }
        }

        for (int i = 0; i <= filas - LONGITUD_GANADORA; ++i) {
            for (int j = 0; j <= columnas - LONGITUD_GANADORA; ++j) {
                bool ganado = true;
                for (int k = 0; k < LONGITUD_GANADORA; ++k) {
                    if (tablero[i + k][j + k] != jugador) {
                        ganado = false;
                        break;
                    }
                }
                if (ganado) return true;
            }
        }

        for (int i = 0; i <= filas - LONGITUD_GANADORA; ++i) {
            for (int j = LONGITUD_GANADORA - 1; j < columnas; ++j) {
                bool ganado = true;
                for (int k = 0; k < LONGITUD_GANADORA; ++k) {
                    if (tablero[i + k][j - k] != jugador) {
                        ganado = false;
                        break;
                    }
                }
                if (ganado) return true;
            }
        }

        return false;
    }

    bool estaTableroLleno() {
        for (int i = 0; i < filas; ++i) {
            for (int j = 0; j < columnas; ++j) {
                if (tablero[i][j] == VACIO)
                    return false;
            }
        }
        return true;
    }

    int evaluar() {
        if (esGanador(JUGADOR1))
            return 10;
        else if (esGanador(JUGADOR2))
            return -10;
        else
            return 0;
    }

    int minimax(int profundidad, bool esJugadorMaximizador, int alfa, int beta) {
        int puntaje = evaluar();
        if (puntaje != 0) return puntaje;
        if (estaTableroLleno()) return 0;
        if (profundidad == 0) return 0;

        if (esJugadorMaximizador) {
            int maxEval = INT_MIN;
            for (int c = 0; c < columnas; ++c) {
                int f = -1;
                for (int i = filas - 1; i >= 0; --i) {
                    if (tablero[i][c] == VACIO) {
                        f = i;
                        break;
                    }
                }
                if (f != -1) {
                    tablero[f][c] = JUGADOR1;
                    int eval = minimax(profundidad - 1, false, alfa, beta);
                    tablero[f][c] = VACIO;
                    maxEval = max(maxEval, eval);
                    alfa = max(alfa, eval);
                    if (beta <= alfa) break;
                }
            }
            return maxEval;
        } else {
            int minEval = INT_MAX;
            for (int c = 0; c < columnas; ++c) {
                int f = -1;
                for (int i = filas - 1; i >= 0; --i) {
                    if (tablero[i][c] == VACIO) {
                        f = i;
                        break;
                    }
                }
                if (f != -1) {
                    tablero[f][c] = JUGADOR2;
                    int eval = minimax(profundidad - 1, true, alfa, beta);
                    tablero[f][c] = VACIO;
                    minEval = min(minEval, eval);
                    beta = min(beta, eval);
                    if (beta <= alfa) break;
                }
            }
            return minEval;
        }
    }

    Movimiento mejorMovimiento() {
        int profundidad;
        if (dificultad == 1)
            profundidad = 4;  // Fácil
        else if (dificultad == 2)
            profundidad = 6;  // Medio
        else
            profundidad = 10;  // Difícil (Aumenta la profundidad)

        int mejorPuntaje = INT_MIN;
        Movimiento mejorMovimiento;
        mejorMovimiento.fila = -1;
        mejorMovimiento.columna = -1;

        for (int c = 0; c < columnas; ++c) {
            int f = -1;
            for (int i = filas - 1; i >= 0; --i) {
                if (tablero[i][c] == VACIO) {
                    f = i;
                    break;
                }
            }
            if (f != -1) {
                tablero[f][c] = JUGADOR1;
                int puntajeMovimiento = minimax(profundidad - 1, false, INT_MIN, INT_MAX);
                tablero[f][c] = VACIO;

                // Introduce aleatoriedad en la elección de movimientos
                if (puntajeMovimiento > mejorPuntaje || (puntajeMovimiento == mejorPuntaje && rand() % 2 == 0)) {
                    mejorMovimiento.fila = f;
                    mejorMovimiento.columna = c;
                    mejorPuntaje = puntajeMovimiento;
                }
            }
        }

        return mejorMovimiento;
    }

    void jugar() {
    int eleccion;

    cout << "¿Quieres empezar primero? (1 para sí, 0 para no): ";
    cin >> eleccion;

    while (eleccion != 1 && eleccion != 0) {
        cout << "Opcion denegada. Intentalo denuevo.\n";
        cin >> eleccion;
    }

    bool turnoJugador = (eleccion == 1);

    while (true) {
        if (turnoJugador) {
            imprimirTablero();
            int c;

            cout << "Tu turno (ingresa el número de columna de 1 a 7, 0 para salir): ";
            cin >> c;

            if (c == 0) {
                guardarPartida();
                cout << "Partida guardada. ¡Hasta luego!\n";
                break;
            }

            c--;  // Ajustar a índices de matriz (de 0 a 6)

            if (c >= 0 && c < columnas && tablero[0][c] == VACIO) {
                for (int f = filas - 1; f >= 0; --f) {
                    if (tablero[f][c] == VACIO) {
                        tablero[f][c] = JUGADOR2;
                        break;
                    }
                }

                if (esGanador(JUGADOR2)) {
                    imprimirTablero();
                    guardarPartidas();
                    cout << "¡Felicidades! ¡Has ganado!\n";
                    victoriasJugador++;
                    break;
                }

                if (estaTableroLleno()) {
                    imprimirTablero();
                    guardarPartidas();
                    cout << "¡Es un empate!\n";
                    break;
                }
            } else {
                cout << "Columna inválida. Por favor, elige otra columna.\n";
            }
        } else {
            Movimiento mejorMovimientoIA = mejorMovimiento();
            tablero[mejorMovimientoIA.fila][mejorMovimientoIA.columna] = JUGADOR1;

            if (esGanador(JUGADOR1)) {
                imprimirTablero();
                guardarPartidas();
                cout << "¡La IA ha ganado!\n";
                victoriasIA++;
                break;
            }

            if (estaTableroLleno()) {
                imprimirTablero();
                guardarPartidas();
                cout << "¡Es un empate!\n";
                break;
            }
        }

        turnoJugador = !turnoJugador;
    }
}
    
    void guardarPartidas() {
    ofstream archivo("Partidas.csv", ios::out | ios::app);

    if (archivo.is_open()) {
        if (esGanador(JUGADOR1) || esGanador(JUGADOR2)) {
            // Guardar el tablero
            for (int i = 0; i < filas; ++i) {
                for (int j = 0; j < columnas; ++j) {
                    archivo << static_cast<int>(tablero[i][j]);
                    if (j < columnas - 1) archivo << ',';
                }
                archivo << '\n';
            }

            archivo << "Ganador: " << (esGanador(JUGADOR1) ? "IA" : "Jugador") << '\n';
        } else {
            archivo << "Partida sin ganador\n";
        }

        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo para guardar la partida.\n";
    }
}
    void guardarPartida() {
        ofstream archivo("partida_guardada.csv");

        if (archivo.is_open()) {
            // Guardar el tablero
            for (int i = 0; i < filas; ++i) {
                for (int j = 0; j < columnas; ++j) {
                    archivo << static_cast<int>(tablero[i][j]);
                    if (j < columnas - 1) archivo << ',';
                }
                archivo << '\n';
            }

            // Guardar dificultad
            archivo << dificultad;

            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo para guardar la partida.\n";
        }
    }
    
    bool cargarPartida() {
        ifstream archivo("partida_guardada.csv");

        if (archivo.is_open()) {
            // Cargar el tablero
            for (int i = 0; i < filas; ++i) {
                string fila;
                if (getline(archivo, fila)) {
                    istringstream ss(fila);
                    for (int j = 0; j < columnas; ++j) {
                        char c;
                        if (ss >> c) {
                            tablero[i][j] = static_cast<Jugador>(c - '0');
                            if (ss.peek() == ',') ss.ignore();
                        }
                    }
                }
            }

            // Cargar dificultad
            string dificultadStr;
            if (getline(archivo, dificultadStr)) {
                istringstream ss(dificultadStr);
                if (ss >> dificultad) {
                    cout << "Partida cargada exitosamente.\n";
                    return true;
                } else {
                    cout << "Error al cargar la dificultad.\n";
                    return false;
                }
            }

            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo para cargar la partida.\n";
            return false;
        }
        return false;
    }
    void guardarPuntuacion() {
        ofstream archivo("puntuacion.csv");

        if (archivo.is_open()) {
            archivo << victoriasJugador << "," << victoriasIA << "\n";
            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo para guardar la puntuación.\n";
        }
    }

    void cargarPuntuacion() {
        ifstream archivo("puntuacion.csv");

        if (archivo.is_open()) {
            string linea;
            if (getline(archivo, linea)) {
                istringstream ss(linea);
                string victoriasJugadorStr, victoriasIAStr;

                if (getline(ss, victoriasJugadorStr, ',') && getline(ss, victoriasIAStr)) {
                    victoriasJugador = stoi(victoriasJugadorStr);
                    victoriasIA = stoi(victoriasIAStr);
                }
            }

            archivo.close();
        } else {
            cout << "No se pudo abrir el archivo para cargar la puntuación.\n";
        }
    }
    void mostrarPuntuacion() {
        cout << "Puntuación:\n";
        cout << "Jugador: " << victoriasJugador << " victorias\n";
        cout << "IA: " << victoriasIA << " victorias\n";
    }
};

void mostrarMenu() {
    cout << "Menú:\n";
    cout << "1. Nueva partida\n";
    cout << "2. Continuar partida\n";
    cout << "3. Ver puntuación\n";
    cout << "4. Salir\n";
}

int main() {
    int opcion;
    int dificultad;
    int victoriasJugador;
    int victoriasIA;
    Conecta4 juego(1);  
    juego.cargarPuntuacion();
    cout << "Bienvenido al juego Connect 4\n\n";

    do {
        mostrarMenu();
        cout << "Selecciona una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "Selecciona la dificultad:\n";
                cout << "1. Fácil\n";
                cout << "2. Medio\n";
                cout << "3. Difícil\n";
                cin >> dificultad;

                while (dificultad != 1 && dificultad != 2 && dificultad != 3) {
                    cout << "Opcion denegada. Intentalo denuevo.\n";
                    cin >> dificultad;
                }

                // Actualizar la dificultad del juego antes de jugar
                juego = Conecta4(dificultad);
                juego.jugar();
                break;
            }
            case 2:
                if(juego.cargarPartida()){
                    juego.jugar();
                }
                break;
            case 3:
                juego.mostrarPuntuacion();
                break;
            case 4:
                cout << "Gracias por jugar\n";
                juego.guardarPuntuacion();
                break;
            default:
                cout << "Opción no válida. Inténtalo de nuevo.\n";
        }
    } while (opcion != 4);

    return 0;
}
