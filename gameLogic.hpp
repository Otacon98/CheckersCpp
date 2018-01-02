#include <iostream>
#include <cstdio>
#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <termios.h>
#include <string>
#include <list>
#include <string>
#include <math.h>
#include <cassert>

using namespace std;
static struct termios old, new_;

#define KEY_UP 65
#define KEY_DO 66
#define KEY_DE 67
#define KEY_IZ 68
#define ENTER 10
#define ESC 27
#define CTAB 25//Posicion en el eje X del cursor
#define OTAB 29//Posicion en el eje X de las opciones del menu
#define cursor "\033[1;38m</>\033[0m"
#define dos_segundos 2000000

string piezaBlanca="\E[41m░\033[0m";
string piezaNegra="\E[42m░\033[0m";
string blanco="\E[47m░\033[0m";
string negro="█";//
string vacio=" ";
string selector="\E[40m░\033[0m";
int menu();
void resizeTerminal(){
	cout << "\e[8;35;120t";
}
void gotoxy(int x,int y){
	printf("%c[%d;%df",0x1B,y,x);
}
void clearScreen(){
	cout << "\033[2J";
}
string to_string( int x ) {	// convierte int a string para trabajar de forma sencilla la forma en que se guardará el historial de jugadas
  int length = snprintf( NULL, 0, "%d", x );
  assert( length >= 0 );
  char* buf = new char[length + 1];
  snprintf( buf, length + 1, "%d", x );
  string str( buf );
  delete[] buf;
  return str;
}

// Ese codigo de abajo no puede ser mas copy paste... :|
// Que el autor acomode esa verga al castellano.

//getch(); y getche();//Necesario para usar el "dpad"
void initTermios(int echo) {
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new_ = old; /* make new settings same as old settings */
  new_.c_lflag &= ~ICANON; /* disable buffered i/o */
  new_.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new_); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) {
  return getch_(0);
}

//###############################################################
//####################     Clase tablero     ####################
//###############################################################


class Tablero{

	private:
		string tablero[8][8];
		list<string> historialDeJugadas; // lista donde se guardará el log de jugadas
		int turno;
	public:
		//Constructor
		Tablero(){
			for(short i=0;i<8;i++){
				for(short j=0;j<8;j++){
					if( i<= 2 && ((i+j)%2 !=0) )
						tablero[i][j] = piezaNegra;
					else if (i >= 5 && ((i+j)%2!=0))
						tablero[i][j] = piezaBlanca;
					else
						tablero[i][j] = vacio;
				}
			}
			turno = 0;
			historialDeJugadas.push_back("Partida iniciada " + fecha() + " a las: " + momentoActual());
		};

		//Metodos
		void imprimirTablero();
		void imprimirPieza(string, int,int);
		void imprimirPiezas();
		void imprimirCursor(int, int, string);
		void imprimirOtrosDatos();
		void seleccionarPieza(string);
		void imprimirLinea(string);
		void imprimirHistorialJugadas();
		void agregarAlHistorialDeJugadas(string, int, int, int, int);
		bool moverPieza(int,int,int,int);
		int contarPiezas(string);
		string momentoActual();
		string fecha();
		bool botEstaAmenazado(int, int, string, string);
		bool botCome(string, string);
		bool botIntentaEscapar(int, int, string, string);
		void botJugadaAleatoria(string, string);
		bool botOtraPiezaDefiende(int, int, string, string);
		bool botMovimientoEducadoAleatorio(string, string);
		void turnoBot(string, string);
		//getters
		// int getTurno(){ return turno;};
};
void Tablero::imprimirTablero(){
	clearScreen();

	// El siguiente 'superFor' se encarga de dibujar la casillas de el tablero
	// Esos valores locos son solo calculos de coordenadas en el terminal.

	int flag = 0;
	for (int k = 1; k < 33; k+=4) {
		if (flag++ % 2 == 0)
		for (int i = 0; i < 57; i+=7) {
			for (int j = 0; j < 4; j++) {

				gotoxy(i,j+k);

				if (i == 0 ) i++; // Esto es una validacion magica, NO tocar.

				if (i % 2 != 0)
					imprimirLinea(blanco);
				else
					imprimirLinea(negro);

			}
		} // Hendry mamalo, borra este beta cuando lo leas, mmlo otra vez.
		else
		for (int i = 0; i < 57; i+=7) {
			for (int j = 0; j < 4; j++) {

				gotoxy(i,j+k);

				if (i == 0 ) i++; // Esto es una validacion magica, NO tocar.

				if (i % 2 == 0)
					imprimirLinea(blanco);
				else
					imprimirLinea(negro);
			}
		}
	}

	short i = 4;
	// Imprimiendo coordenadas horizontales
	for(int j = 0; j<8; j++){
		gotoxy(i,34);
		cout << j;
		i = i+7;
	}
	short aux = 2;
	// Imprimiendo coordenadas verticales
	for(int i=0; i<8; i++){
		gotoxy(58,aux);
		cout << i;
		aux = aux + 4;
	}
}
void Tablero::imprimirPieza(string pieza, int x, int y) { // Si se preguntan porque primero esta Y y despues X no lo hagan
																													// Tengan Fe de que eso funciona y ya. <3
	gotoxy((y*7) + 2, (x*4) + 2);
	cout << pieza << pieza << pieza << pieza << pieza ;
	gotoxy((y*7) + 2, (x*4) + 3);
	cout << pieza << pieza << pieza << pieza << pieza ;

}
void Tablero::imprimirPiezas(){

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (tablero[i][j] != vacio)
					imprimirPieza(tablero[i][j], i,j);
			}
		}
}
void Tablero::imprimirCursor(int X, int Y,string casilla){
	gotoxy(X,Y);
	imprimirLinea(casilla);
	gotoxy(X,Y+1);cout<<casilla;gotoxy(X+6,Y+1);cout<<casilla;
	gotoxy(X,Y+2);cout<<casilla;gotoxy(X+6,Y+2);cout<<casilla;
	gotoxy(X,Y+3);
	imprimirLinea(casilla);
}
void Tablero::imprimirOtrosDatos() {

		gotoxy(80, 5);
		if (turno % 2 == 0){

			cout << "Turno de las " << piezaBlanca << " Rojas ";

			gotoxy(80, 6);
			cout << "Le quedan " << contarPiezas(piezaBlanca) << " piezas";

		}	else {

			cout << "Turno de las " << piezaNegra << " Verdes ";

			gotoxy(80, 6);
			cout << "Le quedan: " << contarPiezas(piezaNegra) << " piezas";

		}

		gotoxy(80,7);
		cout << "Esperando jugada";

		gotoxy(75,34);
		cout << "[ESC] Salir  [F1] Guardar Partida";
}
void Tablero::imprimirLinea(string color) {

			cout <<color<<color<<color<<color<<color<<color<<color;
}
void Tablero::imprimirHistorialJugadas(){
	// imprime en el mismo espacio las ultimas 5 jugadas
	int i = 0;
	list<string>::reverse_iterator iterador = historialDeJugadas.rbegin(); // para poder imprimir el log en pantalla (del final para el inicio)

	gotoxy(80,20);
	cout << "HISTORIAL  DE  JUGADAS";

	if(historialDeJugadas.size() == 1){
		gotoxy(70,23);
		cout << (*iterador) << "   ";
	}

	if(historialDeJugadas.size() == 2){
		gotoxy(70,23);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,24);
		cout << (*iterador) << "   ";
	}

	if(historialDeJugadas.size() == 3){
		gotoxy(70,23);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,24);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,25);
		cout << (*iterador) << "   ";
	}

	if(historialDeJugadas.size() == 4){
		gotoxy(70,23);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,24);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,25);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,26);
		cout << (*iterador) << "   ";
	}

	if(historialDeJugadas.size() >= 5){
		gotoxy(70,23);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,24);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,25);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,26);
		cout << (*iterador) << "   ";
		iterador++;
		gotoxy(70,27);
		cout << (*iterador) << "   ";
	}

}
void Tablero::agregarAlHistorialDeJugadas(string jugador, int x, int y, int newX, int newY){

	string momento = momentoActual();
	string jugada;
	if(jugador == piezaBlanca){
		jugada = "Rojas - " + momento + ": (" + to_string(y) + "," + to_string(x) + ") -> (" + to_string(newY) + "," + to_string(newX) + ")";
	}else{
		jugada = "Verdes - " + momento + ": (" + to_string(y) + "," + to_string(x) + ") -> (" + to_string(newY) + "," + to_string(newX) + ")";
	}

	historialDeJugadas.push_back(jugada); // agrega de ultimo la ultima jugada dentro de la lista
}
bool Tablero::moverPieza(int x, int y, int newX, int newY) {

	string companero;
	string enemigo;

	if( turno % 2 == 0 ){
		companero = piezaBlanca;
		enemigo = piezaNegra;
	}
	if (turno % 2 != 0 ){
		companero = piezaNegra;
		enemigo = piezaBlanca;
	}

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

	// Validación por si intentan moverse hacia atrás
	if(companero == piezaBlanca && x < newX)
		return false;

	if(companero == piezaNegra && x > newX)
		return false;

	// si donde se selecciono hay un enemigo
	if(companero == piezaBlanca && tablero[newX][newY] == enemigo){

		//
		// CORREGIR: las blancas no comen hacia la derecha y las negras no comen hacia la izquierda
		//

		// si la direccion es hacia la izquierda
		if( y > newY ){

			if(tablero[x-2][y-2] == vacio){
				tablero[newX][newY] = vacio;
				tablero[x][y] = vacio;
				tablero[x-2][y-2] = companero;
				agregarAlHistorialDeJugadas(companero, x, y, x-2, y-2);
				// si comió el turno sigue siendo de ese jugador
				return true;
			}else{
				// la jugada esta bloqueada
				return false;
			}
		}else{
			// la direccion es hacia la derecha
			if(tablero[x+2][y-2] == vacio){
				tablero[newX][newY] = vacio;
				tablero[x][y] = vacio;
				tablero[x+2][y-2] = companero;
				agregarAlHistorialDeJugadas(companero, x, y, x+2, y-2);
				return true;
			}else{
				// la jugada esta bloqueada
				return false;
			}
		}
	}


	if(companero == piezaNegra && tablero[newX][newY] == enemigo){
		// si la direcion es hacia la izquierda
		if( x > newX ){
			if(tablero[x-2][y+2] == vacio){
				tablero[newX][newY] = vacio;
				tablero[x][y] = vacio;
				tablero[x-2][y+2] = companero;
				agregarAlHistorialDeJugadas(companero, x, y, x-2, y+2);
				return true;
			}else{
				// la jugada esta bloqueada
				return false;
			}
		}else{
			// si la jugada es hacia la derecha
			if(tablero[x+2][y+2] == vacio){
				tablero[newX][newY] = vacio;
				tablero[x][y] = vacio;
				tablero[x+2][y+2] = companero;
				agregarAlHistorialDeJugadas(companero, x, y, x+2, y+2);
				return true;
			}else{
				// la jugada esta bloqueada
				return false;
			}
		}
	}

	// si la jugada que realizó no involucra comer, se mueve normalmente hacia la posicion.
	agregarAlHistorialDeJugadas(companero, x, y, newX, newY);
	tablero[newX][newY] = tablero[x][y];
	tablero[x][y] = vacio;
	turno++;
	return true;
}
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
void Tablero::seleccionarPieza(string pieza){

	int KEY, i=0, j=0;
	bool cent;
	do{

		gotoxy(80,3);
		cout << "                 ";
		imprimirCursor( (i*7) + 1 , (j*4) +1 ,selector);
		KEY = getch();

		// agregar si el usuario presiona ESC o F1

		if( (i+j) % 2 == 0)
			imprimirCursor((i*7) + 1, (j*4) +1,blanco);
		if( (i+j) % 2 != 0)
			imprimirCursor((i*7) + 1, (j*4) +1,negro);

		cent = true;
		fflush(stdin);

		switch (KEY){
			case KEY_DO:
				j++;
				if (j>7) j=0;
				break;
			case KEY_UP:
				j--;
				if (j<0) j=7;
				break;
			case KEY_IZ:
				i--;
				if(i<0) i=7;
				break;
			case KEY_DE:
				i++;
				if(i>7) i=0;
				break;
			case ENTER:
				int auxI= i, auxJ = j;
				bool flag;

				do{
					imprimirCursor((i*7) + 1 , (j*4) +1 ,selector);
					fflush(stdin);
					KEY = getch();
					fflush(stdin);
					if( (i+j) % 2 != 0)
						imprimirCursor((i*7) + 1,(j*4) +1,negro);
					if( (i+j) % 2 == 0)
						imprimirCursor((i*7) + 1,(j*4) +1,blanco);

					flag = true;
					fflush(stdin);

					switch (KEY){
						case KEY_DO:
							j++;
							if (j>7) j=0;
							break;
						case KEY_UP:
							j--;
							if (j<0) j=7;
							break;
						case KEY_IZ:
							i--;
							if(i<0) i=7;
							break;
						case KEY_DE:
							i++;
							if(i>7) i=0;
							break;
						case ENTER:
							flag = false;
							break;
					}
				}while(flag);

			if(!moverPieza(auxJ, auxI, j , i)){
				gotoxy(80,15);
				cout << "Error, jugada invalida";
			}else{
				cent = false;
				break;
			}
		}
	}while(cent);
}
string Tablero::momentoActual() {
  time_t now = time(0);
  struct tm  tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "[%X]", &tstruct);
  return buf;
}
string Tablero::fecha(){
  time_t now = time(0);
  struct tm  tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
  return buf;
}

//###############################################################
//################    Funciones de los bots    ##################
//###############################################################

// Si recorrió todo, y no hay piezas amenazadas, intenta ubicarse en los laterales (si está cerca, entiendase, columna 1 o 6)
// si no puede:
// hace un "movimiento educado aleatorio"
// basicamente una jugada aleatoria pero que no pone piezas en peligro
// si no puede hacer mas movimientos educados, come a una pieza enemiga
// si no puede comer a una pieza enemiga, hace una jugada aleatoria.

// Funciones del bot

bool Tablero::botEstaAmenazado(int x, int y, string companero, string enemigo){

	if(companero == piezaBlanca){
		if(tablero[x-1][y-1] == enemigo && tablero[x+1][y+1] == vacio){
			return true;
		}
		if(tablero[x+1][y-1] == enemigo && tablero[x-1][y+1] == vacio){
			return true;
		}
	}

	if(companero == piezaNegra){
		if(tablero[x-1][y+1] == enemigo && tablero[x+1][y-1] == vacio){
			return true;
		}
		if(tablero[x+1][y+1] == enemigo && tablero[x-1][y-1] == vacio){
			return true;
		}
	}
}
bool Tablero::botIntentaEscapar(int x, int y, string companero, string enemigo){ // solo funciona para el bot1 (piezas verdes)

	int coordXEnemigo, coordYEnemigo;

	// obteniendo coordenadas del enemigo que amenaza
	if(companero == piezaBlanca){

		if(tablero[x-1][y-1] == enemigo){
			coordXEnemigo = x-1;
			coordYEnemigo = y-1;
		}
		if(tablero[x+1][y-1] == enemigo){
			coordXEnemigo = x+1;
			coordYEnemigo = y-1;
		}

		if(coordXEnemigo == x-1){
			//'verifica que casilla opuesta (x+1) es segura y se mueve hacia ella, si no es segura, retorna falso (no puede escapar)
			if(tablero[x+2][y-2] == enemigo || tablero[x+1][y-1] == companero){
				return false; // no puede escapar de forma segura
			}

			if(tablero[x][y-2] == enemigo){
				//si hay una pieza compañera que defiende la casilla igualmente se mueve a ella
				if(tablero[x+2][y] == companero){
					tablero[x][y] = vacio;
					tablero[x+1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero,x,y,x+1,y-1);
					return true;
				}else{
					return false; //no puede escapar
				}
			}else{
				// la casilla es segura
				// verificando que está vacia, si no está vacia, un beta ps, que otra pieza intente bloquear.
				if(tablero[x+1][y-1] == vacio){
					// convierte a vacio en donde está parada la pieza
					// y la mueve
					tablero[x][y] = vacio;
					tablero[x+1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero,x,y,x+1,y-1);
					return true;
				}else{
					return false;
				}
			}
		}
				// si esta siendo atacado por la derecha
		if(coordXEnemigo == x+1){
			//'verifica que casilla opuesta (x-1) es segura y se mueve hacia ella, si no es segura, retorna falso (no puede escapar)
			if(tablero[x-2][y-2] == enemigo){
				return false; // no puede escapar de forma segura
			}

			if(tablero[x][y-2] == enemigo){
				//si hay una pieza compañera que defiende la casilla igualmente se mueve a ella
				if(tablero[x-2][y] == companero){
					tablero[x][y] = vacio;
					tablero[x-1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero,x,y,x-1,y-1);
					return true;
				}else{
					return false; //no puede escapar
				}
			}else{
				// la casilla es segura
				// verificando que está vacia, si no está vacia, un beta ps, que otra pieza intente bloquear.
				if(tablero[x-1][y-1] == vacio){
					// convierte a vacio en donde está parada la pieza
					// y la mueve
					tablero[x][y] == vacio;
					tablero[x-1][y-1] == companero;
					agregarAlHistorialDeJugadas(companero,x,y,x-1,y-1);
					return true;
				}else{
					return false;
				}
			}
		}

	}

	if(companero == piezaNegra){

		if(tablero[x-1][y+1] == enemigo){
			coordXEnemigo = x-1;
			coordYEnemigo = y+1;
		}
		if(tablero[x+1][y+1] == enemigo){
			coordXEnemigo = x+1;
			coordYEnemigo = y+1;
		}

		//si coordXEnemigo == x-1, está siendo atacado por la izquierda, caso contrario, es derecha (x+1)
		if(coordXEnemigo == x-1){
			//'verifica que casilla opuesta (x+1) es segura y se mueve hacia ella, si no es segura, retorna falso (no puede escapar)
			if(tablero[x+2][y+2] == enemigo || tablero[x+1][y+1] == companero){
				return false; // no puede escapar de forma segura
			}

			if(tablero[x][y+2] == enemigo){
				//si hay una pieza compañera que defiende la casilla igualmente se mueve a ella
				if(tablero[x+2][y] == companero){
					tablero[x][y] = vacio;
					tablero[x+1][y+1] = companero;
					agregarAlHistorialDeJugadas(companero,x,y,x+1,y+1);
					return true;
				}else{
					return false; //no puede escapar
				}
			}else{
				// la casilla es segura
				// verificando que está vacia, si no está vacia, un beta ps, que otra pieza intente bloquear.
				if(tablero[x+1][y+1] == vacio){
					// convierte a vacio en donde está parada la pieza
					// y la mueve
					tablero[x][y] == vacio;
					tablero[x+1][y+1] == companero;
					agregarAlHistorialDeJugadas(companero,x,y,x+1,y+1);
					return true;
				}else{
					return false;
				}
			}
		}
		// si esta siendo atacado por la derecha
		if(coordXEnemigo == x+1){
			//'verifica que casilla opuesta (x-1) es segura y se mueve hacia ella, si no es segura, retorna falso (no puede escapar)
			if(tablero[x-2][y+2] == enemigo){
				return false; // no puede escapar de forma segura
			}

			if(tablero[x][y+2] == enemigo){
				//si hay una pieza compañera que defiende la casilla igualmente se mueve a ella
				if(tablero[x-2][y] == companero){
					tablero[x][y] = vacio;
					tablero[x-1][y+1] = companero;
					agregarAlHistorialDeJugadas(companero,x,y,x-1,y+1);
					return true;
				}else{
					return false; //no puede escapar
				}
			}else{
				// la casilla es segura
				// verificando que está vacia, si no está vacia, un beta ps, que otra pieza intente bloquear.
				if(tablero[x-1][y+1] == vacio){
					// convierte a vacio en donde está parada la pieza
					// y la mueve
					tablero[x][y] == vacio;
					tablero[x-1][y+1] == companero;
					agregarAlHistorialDeJugadas(companero,x,y,x-1,y+1);
					return true;
				}else{
					return false;
				}
			}
		}


	}

}
void Tablero::botJugadaAleatoria(string companero, string enemigo){

	int bandera = 0;
	int x, y, side;

	if(companero == piezaBlanca){
		do{
			x = rand() % 8 + 1;
			y = rand() % 8 + 1;
			if(tablero[x][y] == companero){
				side = rand() % 2;
				if(side == 0){
					if(tablero[x-1][y-1] == vacio){
						tablero[x][y] = vacio;
						tablero[x-1][y-1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x-1, y-1);
						bandera = 1;
					}
				}else{
					if(tablero[x+1][y-1] == vacio){
						tablero[x][y] = vacio;
						tablero[x+1][y-1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x+1, y-1);
						bandera = 1;
					}
				}
			}
		}while(bandera == 0);

	}else if(companero == piezaNegra){
		do{
			x = rand() % 8 + 1;
			y = rand() % 8 + 1;
			if(tablero[x][y] == companero){
				side = rand() % 2;
				if(side == 0){
					if(tablero[x-1][y+1] == vacio){
						tablero[x][y] = vacio;
						tablero[x-1][y+1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x-1, y+1);
						bandera = 1;
					}
				}else{
					if(tablero[x+1][y+1] == vacio){
						tablero[x][y] = vacio;
						tablero[x+1][y+1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x+1, y+1);
						bandera = 1;
					}
				}
			}
		}while(bandera == 0);
	}

}
bool Tablero::botCome(string companero, string enemigo){

	for(short i = 0; i<8; i++){
		for(short j = 0; j<8; j++){
			if(tablero[i+1][j+1] == enemigo){
				tablero[i][j] = vacio;
				tablero[i+1][j+1] = companero;
				agregarAlHistorialDeJugadas(companero, i, j, i+1, j+1);
				return true;
			}else if(tablero[i-1][j+1] == enemigo){
				tablero[i][j] = vacio;
				tablero[i-1][j+1] = companero;
				agregarAlHistorialDeJugadas(companero, i, j, i-1, j+1);
				return true;
			}else if(tablero[i+1][j-1] == enemigo){
				tablero[i][j] = vacio;
				tablero[i+1][j-1] = companero;
				agregarAlHistorialDeJugadas(companero, i, j, i+1, j-1);
				return true;
			}else if(tablero[i-1][j-1] == enemigo){
				tablero[i][j] = vacio;
				tablero[i-1][j-1] = companero;
				agregarAlHistorialDeJugadas(companero, i, j, i-1, j-1);
				return true;
			}
		}
	}

	return false;
}
bool Tablero::botOtraPiezaDefiende(int x, int y, string companero, string enemigo){

	int coordXEnemigo, coordYEnemigo;

	// obteniendo coordenadas del enemigo que amenaza
	if(companero == piezaBlanca){

		if(tablero[x-1][y-1] == enemigo){
			coordXEnemigo = x-1;
			coordYEnemigo = y-1;
		}
		if(tablero[x+1][y-1] == enemigo){
			coordXEnemigo = x+1;
			coordYEnemigo = y-1;
		}

		if(coordXEnemigo == x-1){
			if(tablero[x+1][y+1] == companero){ // la pieza se encuentra defendida
				botMovimientoEducadoAleatorio(companero, enemigo);
				return true;
			}else if(tablero[x+1][y+1] == vacio){ // no está defendido
				if(tablero[x][y+2] == companero){
					tablero[x][y+2] = vacio;
					tablero[x+1][y+1] = companero;
					agregarAlHistorialDeJugadas(companero, x, y+2, x+1, y+1); // la pieza que está 2 posiciones mas abajo de la pieza amenazada la defiende
					return true;
				}
				if(tablero[x+2][y+2] == companero){
					tablero[x+2][y+2] = vacio;
					tablero[x+1][y+1] = companero;
					agregarAlHistorialDeJugadas(companero, x+2, y+2, x+1, y+1); // la pieza que esta 2 posiciones en diagonal a su derecha, la defiende
					return true;
				}
			}
		}else if(coordXEnemigo == x+1){
			if(tablero[x-1][y+1] == companero){
				botMovimientoEducadoAleatorio(companero, enemigo);
				return true;
			}else if(tablero[x-1][y+1] == vacio){
				if(tablero[x][y+2] == companero){
					tablero[x][y+2] = vacio;
					tablero[x-1][y+1] = companero;
					agregarAlHistorialDeJugadas(companero, x, y+2, x-1, y+1);
					return true;
				}
				if(tablero[x-2][y+2] == companero){
					tablero[x-2][y+2] = vacio;
					tablero[x-1][y+1] = companero;
					agregarAlHistorialDeJugadas(companero, x-2, y+2, x-1, y+1);
					return true;
				}
			}
		}
	}
	if(companero == piezaNegra){

		if(tablero[x-1][y+1] == enemigo){
			coordXEnemigo = x-1;
			coordYEnemigo = y+1;
		}
		if(tablero[x+1][y+1] == enemigo){
			coordXEnemigo = x+1;
			coordYEnemigo = y+1;
		}

		if(coordXEnemigo == x-1){
			if(tablero[x+1][y-1] == companero){
				botMovimientoEducadoAleatorio(companero, enemigo);
				return true;
			}else if(tablero[x+1][y-1] == vacio){
				if(tablero[x][y-2] == companero){
					tablero[x][y-2] = vacio;
					tablero[x+1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero, x, y-2, x+1, y-1);
					return true;
				}
				if(tablero[x+2][y-2] == companero){
					tablero[x+2][y-2] = vacio;
					tablero[x+1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero, x+2, y-2, x+1, y-1);
					return true;
				}
			}
		}

		if(coordXEnemigo == x+1){
			if(tablero[x-1][y-1] == companero){
				botMovimientoEducadoAleatorio(companero, enemigo);
				return true;
			}else if(tablero[x-1][y-1] == vacio){
				if(tablero[x][y-2] == companero){
					tablero[x][y-2] = vacio;
					tablero[x-1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero, x, y-2, x-1, y-1);
					return true;
				}
				if(tablero[x-2][y-2] == companero){
					tablero[x-2][y-2] = vacio;
					tablero[x-1][y-1] = companero;
					agregarAlHistorialDeJugadas(companero, x-2, y-2, x-1, y-1);
					return true;
				}
			}
		}
	}

}
bool Tablero::botMovimientoEducadoAleatorio(string companero, string enemigo){

	int bandera = 0;
	int x, y, side;

	if(companero == piezaBlanca){
		do{
			x = rand() % 8;
			y = rand() % 8;
			if(tablero[x][y] == companero){
				side = rand() % 2;
				if(side == 0){
					if(tablero[x+1][y-1] == vacio && x != 7 && y != 0){
						tablero[x][y] = vacio;
						tablero[x+1][y-1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x+1, y-1);
						bandera = 1;
					}
				}else{
					if(tablero[x-1][y-1] == vacio && y != 0 && x != 0){
						tablero[x][y] = vacio;
						tablero[x-1][y-1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x-1, y-1);
						bandera = 1;
					}
				}
			}
		}while(bandera == 0);

	}else if(companero == piezaNegra){
		do{
			x = rand() % 8;
			y = rand() % 8;
			if(tablero[x][y] == companero){
				side = rand() % 2;
				if(side == 0){
					if(tablero[x+1][y+1] == vacio && y != 7){
						tablero[x][y] = vacio;
						tablero[x+1][y+1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x+1, y+1);
						bandera = 1;
					}
				}else{
					if(tablero[x-1][y+1] == vacio && y != 0){
						tablero[x][y] = vacio;
						tablero[x-1][y+1] = companero;
						agregarAlHistorialDeJugadas(companero, x, y, x-1, y+1);
						bandera = 1;
					}
				}
			}
		}while(bandera == 0);
	}
}
void Tablero::turnoBot(string companero, string enemigo){

	for(short i = 0; i<8; i++){
		for(short j = 0; j<8; j++){
			if(tablero[i][j] == companero){
				if(botEstaAmenazado(i,j,companero,enemigo)){ // true == está amenazado; false lo contrario
					if(botIntentaEscapar(i,j,companero, enemigo)){ // true si escapó, false si no puede esacapar
						return; // https://stackoverflow.com/questions/346613/how-do-you-exit-from-a-void-function-in-c
					}else if (botOtraPiezaDefiende(i, j, companero, enemigo)){ // true si nadie está defendiendo a la pieza
						return;
					}
				}
			}
		}
	}


	if(botMovimientoEducadoAleatorio(companero, enemigo)){
		return;
	}
	else if(botCome(companero, enemigo)){
		return;
	}else{
		botJugadaAleatoria(companero, enemigo);
		return;
	}

}

//###############################################################
//###################    Opciones del menu    ###################
//###############################################################
int menuXY(int a, int A){
	int KEY;
	int y=a;
	A=A+a;
	do{
		gotoxy(CTAB,y);
		cout<<cursor;
		KEY=getch();
		switch (KEY)
		{
			case KEY_UP:
				gotoxy(CTAB,y);cout<<"   ";
				y--;
				if (y==(a-1))
					y=(A-1);
			break;
			case KEY_DO:
				gotoxy(CTAB,y);cout<<"   ";
				y++;
				if (y==A)
					y=a;
			break;
			case ENTER:
				return (y-a+1);
			break;
		}
	}while (KEY!=ENTER);
	return (y-a+1);
}
void PressEnterToContinue(){
  int c;
  fflush( stdout );
  do c = getchar(); while ((c != '\n') && (c != EOF));
}
void humanoVShumano(){

	//char coordH;
	//int turno = 1;
	Tablero tablero = Tablero();

	do{
		clearScreen();
		tablero.imprimirTablero();
		tablero.imprimirPiezas();
		tablero.imprimirOtrosDatos();
		tablero.imprimirHistorialJugadas();
		tablero.seleccionarPieza(piezaBlanca);
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
void humanoVSbot(){

	Tablero tablero = Tablero();
	do{
		clearScreen();
		tablero.imprimirTablero();
		tablero.imprimirPiezas();
		tablero.imprimirOtrosDatos();
		tablero.imprimirHistorialJugadas();
		tablero.seleccionarPieza(piezaBlanca);
		tablero.turnoBot(piezaNegra, piezaBlanca);
	}while(true);

}
void botVSbot(){

	Tablero tablero = Tablero();
	do{
		clearScreen();
		tablero.imprimirTablero();
		tablero.imprimirPiezas();
		tablero.imprimirOtrosDatos();
		tablero.imprimirHistorialJugadas();
		PressEnterToContinue();
		tablero.turnoBot(piezaBlanca, piezaNegra);
		clearScreen();
		tablero.imprimirTablero();
		tablero.imprimirPiezas();
		tablero.imprimirOtrosDatos();
		tablero.imprimirHistorialJugadas();
		PressEnterToContinue();
		tablero.turnoBot(piezaNegra, piezaBlanca);
	}while(true);

}
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
int menu(){
		short input;
		resizeTerminal();
		clearScreen();
		gotoxy(33,6);
		cout << "\033[1;20mB I E N V E N I D O\033[0m\n";
		gotoxy(33,7);
		cout << "\033[1;20mA  D A M A S  C H I N A S\033[0m\n";
		gotoxy(OTAB,10);
		cout << "\033[1;38m[1].\033[0m Humano contra Humano\n";
		gotoxy(OTAB,11);
		cout << "\033[1;38m[2].\033[0m Humano contra Bot\n";
		gotoxy(OTAB,12);
		cout << "\033[1;38m[3].\033[0m Bot contra Bot\n";
		gotoxy(OTAB,13);
		cout << "\033[1;38m[4].\033[0m Instrucciones\n";
		gotoxy(OTAB,14);
		cout << "\033[1;38m[5].\033[0m Información\n";
		gotoxy(OTAB,15);
		cout << "\033[1;38m[6].\033[0m Salir\n";
		input=menuXY(10,6);
		switch(input){
			case 1:
				humanoVShumano();
				break;
			case 2:
				humanoVSbot();
				break;
			case 3:
				botVSbot();
			case 4:
				instrucciones();
				break;
			case 5:
				clearScreen();
				info();
				break;
			case 6:
				salir();
				break;

	}
	return (1);
}
