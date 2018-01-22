#include <iostream>
#include <cstdio>
#include <time.h>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include <string>
#include <list>
#include <math.h>
#include <fstream>

using namespace std;
static struct termios old, new_;

#define KEY_UP 65
#define KEY_DO 66
#define KEY_DE 67
#define KEY_IZ 68
#define KEY_F2 19
#define ENTER 10
#define ESC 27
#define DEL 127
#define TAB 9 //???
#define OTAB 43 //Posicion en el eje X de las opciones del menu
#define cursor "\033[1;38m</>\033[0m"
#define dos_segundos 2000000


string piezaBlanca="\E[42m░\033[0m";
string piezaNegra="\E[41m░\033[0m";
string damaBlanca="\E[31m░\033[0m";
string damaNegra="\E[32m░\033[0m";
string negro="\E[47m░\033[0m";
string blanco="\E[40m░\033[0m";
string selector="\E[46m░\033[0m";
string vacio=" ";


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
void PressEnterToContinue(){
  int c;
  fflush( stdout );
  do c = getchar(); while ((c != '\n') && (c != EOF));
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

string toString( int x ) {	// convierte int a string para trabajar de forma sencilla la forma en que se guardará el historial de jugadas
  int length = snprintf( NULL, 0, "%d", x );
  char* buf = new char[length + 1];
  snprintf( buf, length + 1, "%d", x );
  string str( buf );
  delete[] buf;
  return str;
}
void initTermios(int echo) {
  tcgetattr(0, &old);
  new_ = old;
  new_.c_lflag &= ~ICANON;
  new_.c_lflag &= echo ? ECHO : ~ECHO;
  tcsetattr(0, TCSANOW, &new_);
}
void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}
char getch_(int echo) {
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
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
		Tablero(list<string> historialDeJugadasGuardado, string tableroGuardado[][8]){

			string STRING_AUX;
			char CHAR_AUX;
			list<string>::iterator iterador = historialDeJugadasGuardado.begin();
			list<string>::iterator iterador2 = historialDeJugadas.begin();

			while( iterador != historialDeJugadasGuardado.end() ){
				// metiendo elemento por elemento el historial guardado en el historial local
				historialDeJugadas.push_back( (*iterador) );
				iterador++;
			}

			STRING_AUX = historialDeJugadasGuardado.back();
			CHAR_AUX = (char) STRING_AUX[0];

			if( CHAR_AUX == *"R"){
				turno = 1;
			}else{
				turno = 0;
			}

			// generando el tablero en base del tablero guardado
			for(short i=0;i<8;i++){
				for(short j=0;j<8;j++){
					tablero[i][j] = tableroGuardado[i][j];
				}
			}

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
		void guardarPartida();
		bool moverPieza(int,int,int,int);
		bool comerPieza(int,int,int,int);
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
		bool dama(int);
		bool dama(int,int,int,int);

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
		for (int i = 1; i < 57; i+=7)
			for (int j = 0; j < 4; j++) {

				gotoxy(i,j+k);

				if (i % 2 != 0)
					imprimirLinea(blanco);
				else
					imprimirLinea(negro);
			}
		else
		for (int i = 1; i < 57; i+=7)
			for (int j = 0; j < 4; j++) {

				gotoxy(i,j+k);

				if (i % 2 == 0)
					imprimirLinea(blanco);
				else
					imprimirLinea(negro);
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

			cout << "Turno de las " << piezaBlanca << "\033[1;38m VERDES \033[0m";

			gotoxy(80, 6);
			cout << "Le quedan " << contarPiezas(piezaBlanca) << " piezas";

		}	else {

			cout << "Turno de las " << piezaNegra << "\033[1;38m ROJAS \033[0m";

			gotoxy(80, 6);
			cout << "Le quedan: " << contarPiezas(piezaNegra) << " piezas";

		}

		gotoxy(80,7);
		cout << "Esperando jugada";

		gotoxy(75,34);
		cout << "\033[1;38m[DEL] Salir  [TAB] Guardar Partida\033[0m";
}
void Tablero::imprimirLinea(string color) {

			cout <<color<<color<<color<<color<<color<<color<<color;
}
void Tablero::imprimirHistorialJugadas(){
	// imprime en el mismo espacio las ultimas 5 jugadas
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
	if(turno % 2 == 0){
		jugada = "Verdes - " + momento + ": (" + toString(y) + "," + toString(x) + ") -> (" + toString(newY) + "," + toString(newX) + ")";
	}else{
		jugada = "Rojas - " + momento + ": (" + toString(y) + "," + toString(x) + ") -> (" + toString(newY) + "," + toString(newX) + ")";
	}

	historialDeJugadas.push_back(jugada); // agrega de ultimo la ultima jugada dentro de la lista
}
bool Tablero::moverPieza(int x, int y, int newX, int newY) {

	string pieza;

	if( turno % 2 == 0 ){
		pieza = piezaBlanca;
	}
	if (turno % 2 != 0 ){
		pieza = piezaNegra;
	}
	if (tablero[newX][newY] != vacio){
		return false;
	}

	if (tablero[newX][newY] != vacio)
		return false;

	if ((newX + newY) % 2 == 0)
		return false;

	if (tablero[newX][newY] == tablero[x][y])
		return false;

	if (newX == x && newY == y)
		return false;

	if(pieza == piezaBlanca && x < newX)
		return false;

	if(pieza == piezaNegra && x > newX)
		return false;

	if ( (abs(x - newX)) != 1 )
		return false;

	if ( (abs(y - newY)) != 1 )
		return false;

	if ( tablero[x][y] == damaBlanca || tablero[x][y] == damaNegra){
		if (dama(x, y, newX, newY)){
			agregarAlHistorialDeJugadas(piezaBlanca, x, y, newX, newY);
			tablero[newX][newY] = tablero[x][y];
			tablero[x][y] = vacio;
			turno++;
			return true;
		}
	}


	if ( (abs(x - newX)) == 2 && (abs(y - newY)) == 2)//Que pasa si es una dama?
		return(comerPieza(x, y, newX, newY));//Llamado a la funcion comer pieza donda la magia ocurre.


	/*shitposting V2.0: Valide todo en la funcion "comer pieza"... para capturar la pieza te tienes
	 que situar en la posicion donde va a caer la pieza...*/

	// si la jugada que realizó no involucra comer, se mueve normalmente hacia la posicion.

	agregarAlHistorialDeJugadas(pieza, x, y, newX, newY);

	if (dama(newX) && pieza == piezaBlanca){
		tablero[newX][newY] = damaBlanca;
	}else if (dama(newX) && pieza == piezaNegra){
		tablero[newX][newY] = damaNegra;
	}else
		tablero[newX][newY] = tablero[x][y];

	tablero[x][y] = vacio;
	turno++;
	return true;
}
bool Tablero::comerPieza(int x, int y, int newX, int newY){

	//Aqui empieza la mememagia...
	string pieza;//La pieza que se tiene que comer
	int auxX, auxY;//Posicion axuliar, dnde esta el enemigo...
	bool flag=true;//Si ya se guardo la posicion, etonces no sigue buscando, porsi habian mas de dos posible jugadas para comer...

	if (turno % 2 != 0){
		pieza = piezaBlanca;

		if ((y > newY) && (tablero[x+1][y-1] == pieza) && flag){//La matriz esta invertida...
			auxX=x+1;
			auxY=y-1;
			flag = false;
		}
		if ((y < newY) && (tablero[x+1][y+1] == pieza) && flag){
			auxX=x+1;
			auxY=y+1;
			flag = false;
		}
	}else{
		pieza = piezaNegra;
		if ((y > newY) && (tablero[x-1][y-1] == pieza) && flag){
			auxX=x-1;
			auxY=y-1;
			flag = false;
		}
		if ((y < newY) && (tablero[x-1][y+1] == pieza) && flag){
			auxX=x-1;
			auxY=y+1;
			flag = false;
		}
	}

	if (flag) return false;//Si no se baja la bandera, no es posible comer a pieza...

	agregarAlHistorialDeJugadas(piezaBlanca, x, y, newX, newY);
	if (dama(newX) && pieza == piezaBlanca){
		tablero[newX][newY] = damaBlanca;
	}else if (dama(newX) && pieza == piezaNegra){
		tablero[newX][newY] = damaNegra;
	}else
		tablero[newX][newY] = tablero[x][y];
	tablero[x][y] = vacio;
	tablero[auxX][auxY] = vacio;//Eliminando al enemigo...


	//Comer doble.... //Aqui es que se pone feo...
	//Hendryxx1: Comer triple es una jugada vailda?, porque si no lo es es necesario una banera aqui...
	if (turno%2 == 0){
		if ( (tablero[newX-1][newY-1] == pieza ) && ( tablero[newX-2][newY-2] == vacio ))//Verifica que alla via libre.
			comerPieza(newX, newY, newX-2, newY-2);
		else if ( (tablero[newX-1][newY+1] == pieza) && (tablero[newX-2][newY+2] == vacio))
			comerPieza(newX, newY, newX-2, newY+2);
		else
			turno++;
	}else if (turno%2 != 0){
		if ( (tablero[newX+1][newY+1] == pieza ) && ( tablero[newX+2][newY+2] == vacio) )//Verifica que alla via libre.
			comerPieza( newX, newY, newX+2, newY+2 );
		else if ( (tablero[newX+1][newY-1] == pieza ) && ( tablero[newX+2][newY-2] == vacio ))
			comerPieza( newX, newY, newX+2, newY-2 );
		else
			turno++;//Solo incrementa el turno cando no tiene que mas comer...
	}

	return true;
}
bool Tablero::dama(int x){

	if (turno%2 == 0){
		if (x==0)
			return true;
	}else{
		if (x==7)
			return true;
	}
	return false;
}
bool Tablero::dama(int x, int y, int newX, int newY){//Esta funcion solo busca si hay objetos enmedio del camino...

	string pieza, pieza_;

	if (turno%2!=0){
		pieza = piezaBlanca;
		pieza_= damaBlanca;
	}else{
		pieza = piezaNegra;
		pieza_= damaNegra;
	}

	if ((x == newX) && (y == newY))
		return true;

	if ((newY < y) && (newX > x)){
		if (tablero[x+1][y-1] == vacio){//La matriz esta invertida...
			return(dama(x+1, y-1, newX, newY));
		}else if (((tablero[x+1][y-1] == pieza) || (tablero[x+1][y-1] == pieza_)) && (x+2 == newX) && (y-2 == newY) ){
			tablero[x+1][y-1] = vacio;
			return true;
		}
	}
	if ((newY > y) && (newX > x)){
		if (tablero[x+1][y+1] == vacio){//La matriz esta invertida...
			return(dama(x+1, y+1, newX, newY));
		}else if (((tablero[x+1][y+1] == pieza) || (tablero[x+1][y+1] == pieza_)) && (x+2 == newX) && (y+2 == newY) ){
			tablero[x+1][y+1] = vacio;
			return true;
		}
	}
	if ((newY < y) && (newX < x)){
		if (tablero[x-1][y-1] == vacio){//La matriz esta invertida...
			return(dama(x-1, y-1, newX, newY));
		}else if (((tablero[x-1][y-1] == pieza) || (tablero[x-1][y-1] == pieza_)) && (x-2 == newX) && (y-2 == newY) ){
			tablero[x-1][y-1] = vacio;
			return true;
		}
	}
	if ((newY > y) && (newX < x)){
		if (tablero[x-1][y+1] == vacio){//La matriz esta invertida...
			return(dama(x-1, y+1, newX, newY));
		}else if (((tablero[x-1][y+1] == pieza) || (tablero[x-1][y+1] == pieza_)) && (x-2 == newX) && (y+2 == newY) ){
			tablero[x-1][y+1] = vacio;
			return true;
		}
	}
	return false;
}
int Tablero::contarPiezas(string pieza) {
	int count = 0;
	string aux = "";
	if(turno % 2 == 0){
		aux = damaNegra;
	}else{
		aux = damaBlanca;
	}
	for (int i = 0; i < 8; i++) {
		for (int k = 0; k < 8; k++) {
			if (tablero[i][k] == pieza || tablero[i][k] == aux) {
				count++;
			}
		}
	}
	return count;
}
void Tablero::seleccionarPieza(string pieza){

	int KEY, i, j;
	bool cent;
	if(turno%2!=0)
		i=j=0;
	else
		i=j=7;
	do{
		imprimirCursor( (i*7) + 1 , (j*4) +1 ,selector);
		KEY = getch();
		fflush(stdin);

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
			case DEL:
				gotoxy(80,15);cout<<"\033[1;38m¿DESEA SALIR DE LA PARTIDA?\033[0m";
				gotoxy(80,16);cout<<"\033[1;38mPRESIONE 'DEL' PARA SALIR\033[0m";
				if(DEL==getch()) menu();
				gotoxy(80,15);cout<<"                            ";
				gotoxy(80,16);cout<<"                            ";
				break;
			case TAB:
				guardarPartida();
				break;
			case ENTER:
				int auxI= i, auxJ = j;
				bool flag;
				if((turno%2==0&&tablero[j][i]==piezaBlanca)||(turno%2!=0&&tablero[j][i]==piezaNegra))//Valida que la pieza seleccionada sea la del turno correspondiente.
					flag = true;
				if((turno%2==0&&tablero[j][i]==damaBlanca)||(turno%2!=0&&tablero[j][i]==damaNegra))//Valida que la pieza seleccionada sea la del turno correspondiente.
					flag = true;
				while(flag){
					imprimirCursor((i*7) + 1 , (j*4) +1 ,selector);
					fflush(stdin);
					KEY = getch();
					fflush(stdin);
					if( (i+j) % 2 != 0)
						imprimirCursor((i*7) + 1,(j*4) +1,negro);
					if( (i+j) % 2 == 0)
						imprimirCursor((i*7) + 1,(j*4) +1,blanco);
					if(i==auxI && j==auxJ)
						imprimirCursor((i*7) + 1,(j*4) +1,selector);

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
							break;
					}
				};

				imprimirCursor((auxI*7) + 1,(auxJ*4) +1,blanco);//Borra el cursor temporal para evitar problemas posteriores.
				if(!moverPieza(auxJ, auxI, j , i)){
					gotoxy(80,15);
					cout << "\033[1;38mJUGADA INVALIDA\033[0m";
				}else{
					cent = false;
					gotoxy(80,3);
					cout << "                           ";
				}
				break;//break del switch case...
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
void Tablero::guardarPartida(){

	string NOMBRE;
	string FULLROUTE;
	string FULLROUTE_TAB;
	list<string>::iterator iterador = historialDeJugadas.begin();	// creando el iterador para la lista del historial

	gotoxy(80,15);cout<<"\033[1;38mINGRESE EL NOMBRE DE LA PARTIDA:\033[0m";
	gotoxy(80,16);cin >> NOMBRE;
	gotoxy(80,15);cout<<"                            ";
	gotoxy(80,16);cout<<"                            ";

	FULLROUTE	= "partidas-guardadas/" + NOMBRE + ".txt";
	FULLROUTE_TAB = "partidas-guardadas/" + NOMBRE + "Tablero.obj";
	//char FULLROUTE_TABLERO;
	//strcpy(FULLROUTE_TABLERO,FULLROUTE_TAB.c_str());

	// guardando el historial
	ofstream archivo(FULLROUTE.c_str()); // creando el archivo donde se almacenará la partida
	while( iterador != historialDeJugadas.end() ){
		// insertando en el archivo, el historial, linea por linea
		archivo << (*iterador) << endl;
		iterador++;
	}
	archivo.close();


	// guardando el tablero
	FILE *fichero = fopen(FULLROUTE_TAB.c_str(), "wb");
	fwrite(tablero, sizeof(tablero),1,fichero);
	fclose(fichero);

	gotoxy(80,15);cout << "                                         ";
	gotoxy(80,15);cout << "\033[1;38mPARTIDA GUARDADA EXITOSAMENTE\033[0m";
	gotoxy(80,16);cout << "Presione 'Enter' para continuar..";
	PressEnterToContinue();
	PressEnterToContinue();
	gotoxy(80,15);cout<<"                                 ";
	gotoxy(80,16);cout<<"                                 ";
	return;
}

//###############################################################
//################    Funciones de los bots    ##################
//###############################################################

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
	return(true);//QUE LADILLAAAA
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

int menuXY(int CTAB,int a, int A){
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
void ganador(string pieza){
	gotoxy(50,20);
	if(pieza == "\E[42m░\033[0m"){
		cout << "GANARON LAS VERDES";
		PressEnterToContinue();
	}
	else{
		cout << "GANARON LAS ROJAS";
		PressEnterToContinue();
	}
}
void humanoVShumano(){

	//char coordH;
	//int turno = 1;
	Tablero tablero = Tablero();

	do{
		clearScreen();
		if(tablero.contarPiezas(piezaBlanca) == 0){
			ganador(piezaNegra);
			break;
		}
		if(tablero.contarPiezas(piezaNegra) == 0){
			ganador(piezaBlanca);
			break;
		}
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
void cargarPartida(){

	string NOMBRE;
	string FULLROUTE;
	string FULLROUTE_TABLERO;
	string linea;
	list<string> historialGuardado;
	string tableroGuardado[8][8];
	ifstream archivo;
	ifstream archivo2;


	clearScreen();
	gotoxy(45,10);

	cout << "Ingrese el nombre de la partida a cargar: " << endl;
	gotoxy(45,11);
	cin >> NOMBRE;
	FULLROUTE	= "partidas-guardadas/" + NOMBRE + ".txt";
	FULLROUTE_TABLERO = "partidas-guardadas/" + NOMBRE + "Tablero.txt";

	// abriendo el archivo
	archivo.open(FULLROUTE.c_str()); // leyendo el archivo suministrado
	if(archivo){
		gotoxy(45,15);
		cout << "Archivo cargado exitosamente" << endl;
		while( getline(archivo, linea) ){
			// insertando en el archivo, el historial, linea por linea
			historialGuardado.push_back(linea);
		}
		archivo.close();



		gotoxy(45,16);
		cout << "Presione 'Enter' para continuar con su partida";
		PressEnterToContinue();
		PressEnterToContinue();

		Tablero tablero = Tablero(historialGuardado, tableroGuardado);

		do{
			clearScreen();
			tablero.imprimirTablero();
			tablero.imprimirPiezas();
			tablero.imprimirOtrosDatos();
			tablero.imprimirHistorialJugadas();
			tablero.seleccionarPieza(piezaBlanca);
		}while(true);

	}else{
		gotoxy(45,15);
		cout << "No existe esa partida guardada" << endl;
		gotoxy(45,16);
		cout << "Presione 'Enter' para volver";
		PressEnterToContinue();
		PressEnterToContinue();
	}
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
void setup(){
	char esc_start[] = { 0x1b, ']', '0', ';', 0 };
  char esc_end[] = { 0x07, 0 };
  cout << esc_start << "CheckersCpp" << esc_end;

}
int menu(){
		setup();
		bool aux = true;
		short input;
		resizeTerminal();
		while(aux){
			clearScreen();
			gotoxy(45,10);
			cout << "\033[1;20mB I E N V E N I D O\033[0m\n";
			gotoxy(48,11);
			cout << "\033[1;20mA  D A M A S\033[0m\n";
			gotoxy(OTAB,15);
			cout << "\033[1;38m[1].\033[0m Humano contra Humano\n";
			gotoxy(OTAB,16);
			cout << "\033[1;38m[2].\033[0m Humano contra Bot\n";
			gotoxy(OTAB,17);
			cout << "\033[1;38m[3].\033[0m Bot contra Bot\n";
			gotoxy(OTAB,18);
			cout << "\033[1;38m[4].\033[0m Cargar Partida\n";
			gotoxy(OTAB,19);
			cout << "\033[1;38md[5].\033[0m Instrucciones\n";
			gotoxy(OTAB,20);
			cout << "\033[1;38m[6].\033[0m Información\n";
			gotoxy(OTAB,21);
			cout << "\033[1;38m[7].\033[0m Salir\n";
			input = menuXY(38,15,7);
			switch(input){
				case 1:
					humanoVShumano();
					break;
				case 2:
					humanoVSbot();
					break;
				case 3:
					botVSbot();
					break;
				case 4:
					cargarPartida();
					break;
				case 5:
					instrucciones();
					break;
				case 6:
					clearScreen();
					info();
					break;
				case 7:
					salir();
					aux = false;
					break;
				}
			}
	return (1);
}
