#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include "gameLogic.hpp"

using namespace std;

int main() {
	
	short input = 18;
	while(input){
		cout << "B I E N V E N I D O\n\n";
		cout << "[1]. Humano contra Humano\n";
		cout << "[2]. Humano contra Bot\n";
		cout << "[3]. Bot contra Bot\n";
		cout << "[4]. Instrucciones\n";
		cout << "[5]. Salir del juego\n";
		cout << "¿Qué desea hacer?: ";
		
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
				cout << "Saliendo del juego . . . ";
				usleep(1000000);
				
				input = 0;
				break;
			default:
				break;
		}
	}
	
	return 0;
}
