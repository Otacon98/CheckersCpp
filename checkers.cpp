#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include <ctime>
#include "gameLogic.hpp"

using namespace std;

int main(){
	setlocale(LC_ALL, "es_ES"); // permite uso de caracteres especiales (acentos, entre otros)
	menu();
	return 0;

}
