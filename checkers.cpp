#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include "gameLogic.hpp"

using namespace std;

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

int main(){
	setlocale(LC_ALL, "es_ES"); // permite uso de caracteres especiales (acentos, entre otros)
	menu();
	return 0;

}
