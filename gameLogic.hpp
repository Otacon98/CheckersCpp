#include <iostream>
#include <cstdio>
#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <math.h>
using namespace std;

string blanco="░";
string negro="█";
string piezaBlanca = "O";
string piezaNegra = "X";
string vacio = " ";

void resizeTerminal(){
	cout << "\e[8;27;85t";
}
void gotoxy(int x,int y){
	printf("%c[%d;%df",0x1B,y,x);
}
void clearScreen(){
	cout << "\033[2J";
}

// Clase tablero
class Tablero{

	private:
		string tablero[8][8]; // aloja a las piezas.
		int turno; // Define el turno y de quien es el turno.

	public:
		//Constructor
		Tablero(){
			for(short i=0;i<8;i++){
				for(short j=0;j<8;j++){
					if( i<= 2 && ((i+j)%2 !=0) )
						tablero[i][j] = piezaBlanca;
					else if (i>=5&&((i+j)%2!=0))
						tablero[i][j] = piezaNegra;
					else
						tablero[i][j] = vacio;
				}
			}
			turno = 0;
		};
		//Methods
		void imprimirCasillas();
		void imprimirTablero();
		void imprimirPiezas();
		int contarPiezas(string);
		bool moverPieza(int,int,int,int);
		//Getters
		int getTurno();
		//Setters
};

int Tablero::getTurno(){
	return turno;
}

void Tablero::imprimirCasillas(){
	// Esto es infernal de leer, analicen uno y modulenlo, como explique en la funcion "imprimirTablero"

	// El siguiente 'superFor' se encarga de dibujar la casillas de el tablero
	int flag = 0;
	for (int k = 1; k < 25; k+=3) {
		if (flag++ % 2 == 0)
		for (int i = 0; i < 41; i+=5) {
			for (int j = 0; j < 3; j++) {
				gotoxy(i,j+k);
				if (i == 0 ) i++;
				if (i % 2 != 0)
				cout << blanco << blanco << blanco << blanco << blanco;
				else
				cout << negro << negro << negro << negro << negro;
			}
		} // Hendry mamalo, borra este beta cuando lo leas, mmlo otra vez.
		else
		for (int i = 0; i < 41; i+=5) {
			for (int j = 0; j < 3; j++) {
				gotoxy(i,j+k);
				if (i == 0 ) i++;
				if (i % 2 == 0)
				cout << blanco << blanco << blanco << blanco << blanco;
				else
				cout << negro << negro << negro << negro << negro;

			}
		}

	}

	short i = 3;
	// Imprimiendo coordenadas horizontales
	for(int a = 65; a<73; a++){ // A == 65 - ASCII code
		gotoxy(i,26);
		cout << (char)a;
		i = i+5;
	}

	short aux = 2;
	// Imprimiendo coordenadas verticales
	for(int i = 0;i<8;i++){
		gotoxy(43,aux);
		cout << i;
		aux = aux +3;
	}


}
void Tablero::imprimirPiezas(){

	// Esta funcion no valida (le falta un acento en valida), Solo imprime las piezas


			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 8; l++) {

					if (tablero[k][l] != vacio) {

							gotoxy((l*5) + 3, (k*3) + 2);
							cout << tablero[k][l];
					}
				}
			}

}
void Tablero::imprimirTablero(){
	// Angel - 10/12/17 - 6:50pm

	// mi idea es tener la funcion "imprimirTablero" que llame a "imprimirCasilla" y luego "imprimirPieza"
	// quedando algo bastante agradable tipo:
	//
	//		imprimirCasillas();
	//		imprimirPiezas(tablero);
	//
	//	Y quedaría una abstracción bastante chevere de lo que es la matriz que se muestra con respecto al tablero real donde estarán las piezas
	//

	// Fernando 23/12/17
	// Cito: imprimirPiezas(tablero); Pasarle tablero a un metodo de la clase tablero no tiene mucho sentido
	// debido a que tablero ya es parte del objeto :v, Borra todo estos comentarios innecesario de este metodo despues de leer esto.

	imprimirCasillas();
	imprimirPiezas();
};
int Tablero::contarPiezas(string pieza) {
	int count = 0;
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			if (tablero[i][k] == pieza) {
				count++;
			}
		}
	}
	return count;
}

bool Tablero::moverPieza(int x, int y, int newX, int newY){

	// Validaciones locas.

	if (turno % 2 == 0 && tablero[x][y] != piezaBlanca)
		return false;
	
	if (turno % 2 != 0 && tablero[x][y] != piezaNegra)
		return false;

	if( newX % 2 == 0 && newY % 2 == 0 )
		return false;

	if( newX % 2 != 0 && newY % 2 != 0 )
		return false;

	if (tablero[newX][newY] == tablero[x][y])
		return false;

	if ( (abs(x - newX)) != 1 )
		return false;

	if ( (abs(y - newY)) != 1 )
		return false;


	tablero[newX][newY] = tablero[x][y];
	tablero[x][y] = " ";

	turno++;

	return true;



}


void humanoVShumano(){

	int coordX;
	int coordY;
	int newCoordX;
	int newCoordY;

	Tablero tablero;
	do {
		clearScreen();
		tablero.imprimirTablero();

		gotoxy(55, 5);
		if (tablero.getTurno() % 2 == 0){

			cout << "Turno de las blancas " << piezaBlanca;

			gotoxy(55, 6);
			cout << "Sus fichas son: " << tablero.contarPiezas(piezaBlanca);

		}	else {

			cout << "Turno de las negras " << piezaNegra;

			gotoxy(55, 6);
			cout << "Sus fichas son: " << tablero.contarPiezas(piezaNegra);

		}

		gotoxy(55,7);
		cout << "Esperando jugada";

		do{

		gotoxy(47,10);
		cout << "Introduzca primera coordenada (0-7)        "; // Los espacios en blanco son una validacion pirata. (Bien pirata :v)

		gotoxy(65,11);
		cin >> coordX;

		gotoxy(47,10);
		cout << "Introduzca segunda coordenada (A-H)        ";

		gotoxy(65,11);
		cin >> coordY;

		gotoxy(47,10);
		cout << "Introduzca primera coordenada a mover (0-7)";

		gotoxy(65,11);
		cin >> newCoordX;

		gotoxy(47,10);
		cout << "Introduzca segunda coordenada a mover (A-H)";

		gotoxy(65,11);
		cin >> newCoordY;

		if (!tablero.moverPieza(coordX,coordY,newCoordX,newCoordY)){
			gotoxy(50,9);
			cout << "Error, jugada no permitida";
			continue; // para el que no se acuerde, continue termina la vuelta del ciclo y comienza otra iteracion.
		}else
			break; // El break rompe y se sale completamente.

	}while(true);




	}while(true);
}

void instrucciones(){
	short aux = 0;

	do{
		clearScreen();
		gotoxy(30,6);
		cout << "\033[1;38m¿Cómo se juega?\033[0m";
		gotoxy(25,8);
		cout << "Desarrollado como proyecto para";
		gotoxy(25,9);
		cout << "la asignatura Estructura de Datos";
		gotoxy(25,11);
		cout << "\033[1;4mIntegrantes:\033[0m\n";
		gotoxy(35,13);
		cout << "Salazar Fernando";
		gotoxy(35,14);
		cout << "Flores Hendry";
		gotoxy(35,15);
		cout << "Rodríguez Angel";
		gotoxy(25,17);
		cout << "Presione \033[1;20m'Enter'\033[0m para volver al menú";
		gotoxy(62,17);
		getchar();
		aux = getchar();
	}while(aux != 10);
	clearScreen();
}

void humanoVSbot(){}

void botVSbot(){}

void info(){
	short aux = 0;
	do{
		clearScreen();
		gotoxy(30,6);
		cout << "\033[1;38mD A M A S   C H I N A S\033[0m";
		gotoxy(25,8);
		cout << "Desarrollado como proyecto para";
		gotoxy(25,9);
		cout << "la asignatura Estructura de Datos";
		gotoxy(25,11);
		cout << "\033[1;4mIntegrantes:\033[0m\n";
		gotoxy(35,13);
		cout << "Salazar Fernando";
		gotoxy(35,14);
		cout << "Flores Hendry";
		gotoxy(35,15);
		cout << "Rodriguez Angel";
		gotoxy(25,17);
		cout << "Presione \033[1;20m'Enter'\033[0m para volver al menu";
		gotoxy(63,17);
		getchar();
		aux = getchar();
	}while(aux != 10);
	clearScreen();
}

void salir(){

	cout << "\nSaliendo del juego" << endl;
	usleep(1000000);
	gotoxy(20,19);
	cout << ". " << endl;
	usleep(1000000);
	gotoxy(22,19);
	cout << ". " << endl;
	usleep(1000000);
	gotoxy(24,19);
	cout << ". " << endl;
	usleep(2000000);
}
