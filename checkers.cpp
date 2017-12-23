#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include "gameLogic.hpp"

using namespace std;

int main() {

	setlocale(LC_ALL, "es_ES");
	resizeTerminal();
	short input = 0;

	while(input != 6){
		clearScreen();
		gotoxy(33,6);
		cout << "\033[1;20mB I E N V E N I D O\033[0m\n";
		gotoxy(30,7);
		cout << "\033[1;20mA  D A M A S  C H I N A S\033[0m\n";
		gotoxy(30,10);
		cout << "\033[1;38m[1].\033[0m Humano contra Humano\n";
		gotoxy(30,11);
		cout << "\033[1;38m[2].\033[0m Humano contra Bot\n";
		gotoxy(30,12);
		cout << "\033[1;38m[3].\033[0m Bot contra Bot\n";
		gotoxy(30,13);
		cout << "\033[1;38m[4].\033[0m Instrucciones\n";
		gotoxy(30,14);
		cout << "\033[1;38m[5].\033[0m Información\n";
		gotoxy(30,15);
		cout << "\033[1;38m[6].\033[0m Salir\n";
		gotoxy(30,17);
		cout << "\033[1;4m¿Qué desea hacer?:\033[0m\n";
		gotoxy(51,17);
		cin >> input;

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
			default:
				// Aqui se debe validar el buffer del input o alguna magia negra para que no explote
				// cuando le tiras en el menu algo tipo "123456734980"
				break;
		}
	}

	return 0;
}
