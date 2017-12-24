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

string piezaBlanca="\E[41m░\033[0m";
string piezaNegra="\E[42m░\033[0m";
string blanco="\E[47m░\033[0m";
string negro="█";//
string vacio=" ";
string selector="\E[40m░\033[0m";

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
//########################     MENU     #########################
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
					else if (i>=5&&((i+j)%2!=0))
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
		void moverPieza();
		bool moverPieza(int,int,int,int);
		int contarPiezas(string);
		string momentoActual();
		string fecha();
		bool botEstaAmenazado(int, int, string);
		bool botIntentaComer(string);
		bool botIntentaEscapar(int, int, string, string);
		void botJugadaAleatoria(string);
		bool botOtraPiezaDefiende(int, int);
		bool botMovimientoEducadoAleatorio();
		void turnoBot1();
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
		jugada = "Rojas - " + momento + ": (" + to_string(x) + "," + to_string(y) + ") -> (" + to_string(newX) + "," + to_string(newY) + ")";
	}else{
		jugada = "Verdes - " + momento + ": (" + to_string(x) + "," + to_string(y) + ") -> (" + to_string(newX) + "," + to_string(newY) + ")";
	}

	historialDeJugadas.push_back(jugada); // agrega de ultimo la ultima jugada dentro de la lista
}
bool Tablero::moverPieza(int x, int y, int newX, int newY) {

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

	agregarAlHistorialDeJugadas(piezaNegra, x, y, newX, newY);
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
	int KEY,i=0, j=0;
	bool cent;
	do{

		gotoxy(80,3);
		cout << "                 ";
		imprimirCursor( (i*7) + 1 , (j*4) +1 ,selector);
		KEY = getch();

		if((i+j)%2==0)
			imprimirCursor((i*7) + 1, (j*4) +1,blanco);
		if ((i+j)%2!=0)
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
					KEY=getch();
					fflush(stdin);
					if ((i+j)%2!=0)
						imprimirCursor((i*7) + 1,(j*4) +1,negro);
					if((i+j)%2==0)
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
				gotoxy(80,3);
				cout << "Error, jugada invalida";
			}else
					cent = false;
					break;
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
//###################    Funciones del bot    ###################
//###############################################################

// Funciones del bot1
bool Tablero::botEstaAmenazado(int x, int y, string enemigo){

	if(tablero[x-1][y-1] == enemigo){
		return true;
	}
	if(tablero[x+1][y-1] == enemigo){
		return true;
	}
	if(tablero[x-1][y+1] == enemigo){
		return true;
	}
	if(tablero[x+1][y+1] == enemigo){
		return true;
	}

	return false;
}
bool Tablero::botIntentaEscapar(int x, int y, string enemigo, string companero){ // solo funciona para el bot1 (piezas verdes)

	int coordXEnemigo, coordYEnemigo;
	// obteniendo coordenadas del enemigo que amenaza
	if(tablero[x-1][y-1] == enemigo){
		coordXEnemigo = x-1;
		coordYEnemigo = y-1;
	}
	if(tablero[x+1][y-1] == enemigo){
		coordXEnemigo = x+1;
		coordYEnemigo = y-1;
	}
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
		if(tablero[x+2][y+2] == enemigo){
			return false; // no puede escapar de forma segura
		}

		if(tablero[x][y+2] == enemigo){
			//si hay una pieza compañera que defiende la casilla igualmente se mueve a ella
			if(tablero[x+2][y] == companero){
				tablero[x][y] == vacio;
				tablero[x+1][y+1] == companero;
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

		if(tablero[x][y+2] == enemigo){
			//si hay una pieza compañera que defiende la casilla igualmente se mueve a ella
			if(tablero[x-2][y] == companero){
				tablero[x][y] == vacio;
				tablero[x-1][y-1] == companero;
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
				return true;
			}else{
				return false;
			}
		}
	}

}
void Tablero::turnoBot1(){ // bot1 es el de las piezas verdes.
	short bandera = 0; // 0 = no ha jugado
	for(short i = 0; i<8; i++){
		for(short j = 0; j<8; j++){
			if(bandera == 1){
				// turno contricante
			}
			if(tablero[i][j] == piezaBlanca){	// piezaBlanca == piezaVerde en el juego (las de arriba, que pertenecen al bot1)
				// se analiza el entorno de la pieza en la que se esta parado
				if(botEstaAmenazado(i,j,piezaNegra)){ // true == está amenazado; false lo contrario
					if(botIntentaEscapar(i,j,piezaNegra, piezaBlanca)){ // true si escapó, false si no puede esacapar
						bandera = 1;
					}else if (botOtraPiezaDefiende(i, j)){ // true si otra pieza puede defender; false lo contrario
						bandera = 1;
					}
				}
			}
		}
	}

	if(bandera == 1){
		// turno contricante
	}

	// Si recorrió todo, y no hay piezas amenazadas, intenta ubicarse en los laterales (si está cerca, entiendase, columna 1 o 6)
	// si no puede:
	// hace un "movimiento educado aleatorio"
	// basicamente una jugada aleatoria pero que no pone piezas en peligro
	// si no puede hacer mas movimientos educados, come a una pieza enemiga
	// si no puede comer a una pieza enemiga, hace una jugada aleatoria.

	if(botMovimientoEducadoAleatorio()){
		bandera = 1;
	}
	else if(botIntentaComer(piezaNegra)){
		bandera = 1;
	}else{
		botJugadaAleatoria(piezaNegra);
		bandera = 1;
	}

}
void Tablero::botJugadaAleatoria(string enemigo){

}
bool Tablero::botIntentaComer(string enemigo){

}
bool Tablero::botOtraPiezaDefiende(int x, int y){

}
bool Tablero::botMovimientoEducadoAleatorio(){

}
// Funciones del bot2 (solo se usan en el modo BotVsBot)

//###############################################################
//###################    Opciones del menu    ###################
//###############################################################

void humanoVShumano(){

	//char coordH;
	//int turno = 1;
	Tablero tablero=Tablero();
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
