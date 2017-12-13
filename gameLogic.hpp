#include <iostream>
#include <cstdio>
#include <time.h>
#include <unistd.h>
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
void clear(){ 
	cout << "\033[2J\033[1;1H"; 
}

// Clase tablero
class Tablero{
	
	private:
		string tablero[8][8];
		
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
		}; 	
		void imprimirCasillas();
		void imprimirTablero();
			
			
};

void Tablero::imprimirCasillas(){
	// Esto es infernal de leer, analicen uno y modulenlo, como explique en la funcion "imprimirTablero"
	
	//Fila 1
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+1);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+1);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+1);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+1);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+1);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+1);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+1);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+1);
		cout << negro << negro << negro << negro << negro;
	}
	
	// Fila 2
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+4);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+4);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+4);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+4);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+4);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+4);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+4);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+4);
		cout << blanco << blanco << blanco << blanco << blanco;
	}

	// Fila 3
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+7);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+7);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+7);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+7);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+7);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+7);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+7);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+7);
		cout << negro << negro << negro << negro << negro;
	}
	
	// Fila 4
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+10);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+10);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+10);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+10);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+10);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+10);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+10);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+10);
		cout << blanco << blanco << blanco << blanco << blanco;
	}

	// Fila 5
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+13);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+13);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+13);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+13);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+13);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+13);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+13);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+13);
		cout << negro << negro << negro << negro << negro;
	}
	
	// Fila 6
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+16);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+16);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+16);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+16);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+16);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+16);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+16);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+16);
		cout << blanco << blanco << blanco << blanco << blanco;
	}

	// Fila 7
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+19);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+19);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+19);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+19);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+19);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+19);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+19);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+19);
		cout << negro << negro << negro << negro << negro;
	}
	
	// Fila 8 - mamalo hendry	
	for(int i = 0; i<3; i++ ){
		gotoxy(0,i+22);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(6,i+22);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(11,i+22);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(16,i+22);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(21,i+22);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(26,i+22);
		cout << blanco << blanco << blanco << blanco << blanco;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(31,i+22);
		cout << negro << negro << negro << negro << negro;
	}
	for(int i = 0; i<3; i++ ){
		gotoxy(36,i+22);
		cout << blanco << blanco << blanco << blanco << blanco;
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
void Tablero::imprimirTablero(){
	// Angel - 10/12/17 - 6:50pm
	// Imprimiendo casillas -- Alguien que module esto para que sea mas amigable (con fors. yo lo estaba haciendo pero hay que 
	// sentarse un rato para calcular las coordenadas del gotoxy para que se imprima bien
	// mi idea es tener la funcion "imprimirTablero" que llame a "imprimirCasilla" y luego "imprimirPieza"
	// quedando algo bastante agradable tipo:
	//
	//		imprimirCasillas();
	//		imprimirPiezas(tablero);
	//
	//	Y quedaría una abstracción bastante chevere de lo que es la matriz que se muestra con respecto al tablero real donde estarán las piezas
	//
	
	imprimirCasillas();
	//imprimirPiezas(tablero);
};	


void humanoVShumano(){
	
	char coordH;
	int turno = 1;
	Tablero tablero;
	clear();
	tablero.imprimirTablero();
	
	gotoxy(55, 5);
	cout << "Turno del jugador " << turno;
	
	gotoxy(55, 6);
	cout << "Sus fichas son: O";
	
	gotoxy(55,7);
	cout << "Esperando jugada";
	
	gotoxy(47,10);
	cout << "Introduzca primera coordenada (A-H)";
	
	gotoxy(65,11);
	cin >> coordH;
}

void instrucciones(){}

void humanoVSbot(){}

void botVSbot(){}

void info(){}

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

