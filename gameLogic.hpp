#include <iostream>
#include <cstdio>
#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <termios.h>

using namespace std;
static struct termios old, new_;

//"Aqui llego HendryXXXXXXXXX1 a tirar mierda en el codigo":
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

//getch(); y getche();//Necesario para usar el "dpad"
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new_ = old; /* make new settings same as old settings */
  new_.c_lflag &= ~ICANON; /* disable buffered i/o */
  new_.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new_); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
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
		}; 	
		void imprimirTablero();
		void imprimirPiezas();
		void imprimirCursor(int, int, string);
		void seleccionarPieza(string);
		void moverPieza();
		int imprimirCasilla(int,int,int,int);
};
void Tablero::imprimirTablero(){
	clearScreen();
	int i,j,x=0,y=1;
	for (i=1;i<=8;i++){
		for (j=1,x=0;j<=8||x<36;j++){
			x=imprimirCasilla(i,j,x,y)+7;
		}
		y=y+4;
		x=0;
	}
	i = 4;
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
int Tablero::imprimirCasilla(int i,int j,int x, int y){
	if ((i+j)%2==0){
		for(int z=0;z<4;z++){
			gotoxy(x,z+y);
			cout<<blanco<<blanco<<blanco<<blanco<<blanco<<blanco<<blanco;
		}
	}
	else{
		for(int z=0;z<4;z++){
			gotoxy(x,z+y);
			cout <<negro<<negro<<negro<<negro<<negro<<negro<<negro;
		}
	}
	if (x==0)
		return(1);
	else
		return(x);
}

void Tablero::imprimirPiezas(){
	int y=2, x=2;
	string aux;
	for(int i=0;i<=8;i++){
		for (int j=0;j<8;j++){
			if(this->tablero[i][j]==piezaBlanca){
				gotoxy(x,y);
				cout<<piezaBlanca<<piezaBlanca<<piezaBlanca<<piezaBlanca<<piezaBlanca;
				gotoxy(x,y+1);
				cout<<piezaBlanca<<piezaBlanca<<piezaBlanca<<piezaBlanca<<piezaBlanca;
			}else if(this->tablero[i][j]==piezaNegra){
				gotoxy(x,y);
				cout<<piezaNegra<<piezaNegra<<piezaNegra<<piezaNegra<<piezaNegra;	
				gotoxy(x,y+1);
				cout<<piezaNegra<<piezaNegra<<piezaNegra<<piezaNegra<<piezaNegra;
			}
			x=x+7;
		}
		x=2;
		y=y+4;
	}
}
void Tablero::seleccionarPieza(string pieza){
	int KEY,x=1, y=29, i=0, j=7;
	bool cent=true;
	do{
		imprimirCursor(x,y,selector);
		KEY=getch();
		fflush(stdin);
		if ((i+j)%2!=0)
			imprimirCursor(x,y,negro);
		if((i+j)%2==0)
			imprimirCursor(x,y,blanco);
		fflush(stdin);
		switch (KEY){
			case KEY_DO:
				y=y+4;
				j=j+1;
				if (j>7){
					y=1;
					j=0;
				}			
			break;
			case KEY_UP:
				y=y-4;
				j=j-1;
				if (j<0){
					y=29;
					j=7;
				}
			break;
			case KEY_IZ:
				x=x-7;
				i=i-1;
				if(i<0){
					x=50;
					i=7;
				}
			break;
			case KEY_DE:
				x=x+7;
				i=i+1;
				if(i>7){
					x=1;
					i=0;
				}
			break;
			case ENTER:
				if (tablero[j][i]==pieza)
					cent=false;
			break;
		}
	}while(cent);
	/*Llamado de la funcion de movimiento...
	 * Busca los movimientos disponibles, si no existen sebe retornar a esta...
	 * todo se esta enredando.. jeje
	 */
}

void Tablero::imprimirCursor(int X, int Y,string casilla){
	gotoxy(X,Y);
	cout<<casilla<<casilla<<casilla<<casilla<<casilla<<casilla<<casilla;
	gotoxy(X,Y+1);cout<<casilla;gotoxy(X+6,Y+1);cout<<casilla;		
	gotoxy(X,Y+2);cout<<casilla;gotoxy(X+6,Y+2);cout<<casilla;
	gotoxy(X,Y+3);
	cout<<casilla<<casilla<<casilla<<casilla<<casilla<<casilla<<casilla;
}


//###############################################################
//###################     Otras Funciones     ###################
//###############################################################


void humanoVShumano(){
	
	//char coordH;
	//int turno = 1;
	Tablero tablero=Tablero();
	clearScreen();
	tablero.imprimirTablero();
	tablero.imprimirPiezas();
	tablero.seleccionarPieza(piezaBlanca);
	
	
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

