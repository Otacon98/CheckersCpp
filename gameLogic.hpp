#include <iostream>
#include <cstdio>
#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <termios.h>
#include <math.h>

using namespace std;
static struct termios old, new_;

//"Aqui llego HendryXXXXXXXXX1 a tirar mierda en el codigo": // fer: Maldita sea.
#define KEY_UP 65
#define KEY_DO 66
#define KEY_DE 67
#define KEY_IZ 68
#define ENTER 10
#define ESC 27
#define CTAB 25//Posicion en el eje X del cursor
#define OTAB 29//Posicion en el eje X de las opciones del menu
#define cursor "\033[1;38m</>"

string piezaBlanca="\E[41m░";
string piezaNegra="\E[42m░";
string blanco="\E[47m░";
string negro="█";//
string vacio=" ";
string selector="\E[40m░";

void resizeTerminal(){
	cout << "\e[8;27;85t";
}
void gotoxy(int x,int y){
	printf("%c[%d;%df",0x1B,y,x);
}
void clearScreen(){
	cout << "\033[2J";
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
		};

		//Metodos
		void imprimirTablero();
		void imprimirPieza(string, int,int);
		void imprimirPiezas();
		void imprimirCursor(int, int, string);
		void imprimirOtrosDatos();
		void seleccionarPieza(string);
		void imprimirLinea(string);
		void moverPieza();
		bool moverPieza(int,int,int,int);
		int contarPiezas(string);

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
	for(int a = 65; a<73; a++){ // A == 65 - ASCII code
		gotoxy(i,34);
		cout << (char)a;
		i = i+7;
	}
	short aux = 2;
	// Imprimiendo coordenadas verticales
	for(int i=8;i>=1;i--){
		gotoxy(58,aux);
		cout << i;
		aux = aux +4;
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

			cout << "Turno de las blancas " << piezaBlanca << " Rojas ";

			gotoxy(80, 6);
			cout << "Sus fichas son: " << contarPiezas(piezaBlanca);

		}	else {

			cout << "Turno de las negras " << piezaNegra << " Mostaza ";

			gotoxy(80, 6);
			cout << "Sus fichas son: " << contarPiezas(piezaNegra);

		}

		gotoxy(80,7);
		cout << "Esperando jugada";

}

void Tablero::imprimirLinea(string color) {

			cout <<color<<color<<color<<color<<color<<color<<color;
}
bool Tablero::moverPieza(int x,int y,int newX, int newY) {

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

			do{
				imprimirCursor((i*7) + 1 , (j*4) +1 ,selector);
				KEY=getch();
				fflush(stdin);
				if ((i+j)%2!=0)
					imprimirCursor((i*7) + 1,(j*4) +1,negro);
				if((i+j)%2==0)
					imprimirCursor((i*7) + 1,(j*4) +1,blanco);
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
							if(!moverPieza(auxJ, auxI, j , i)){
								gotoxy(80,3);
								cout << "Error, jugada invalida";
							}else
								cent = false;
					break;
				}
			}while(cent);
			break;
		}
	}while(cent);
}



//###############################################################
//###################     Otras Funciones     ###################
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
