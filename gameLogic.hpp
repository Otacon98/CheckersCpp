#include <iostream>

using namespace std;

void gotoxy(int x,int y){
	printf("%c[%d;%df",0x1B,y,x);
}

void humanoVShumano(){}

void instrucciones(){}

void humanoVSbot(){}

void botVSbot(){}

void clear(){ cout << "\033[2J\033[1;1H"; }

void salir(){
	cout << "Saliendo del juego . . .";
}

