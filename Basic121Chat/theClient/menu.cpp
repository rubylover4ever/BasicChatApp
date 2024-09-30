#include "menu.h"
int k = 0;

void Menu::setSystemColor(){
	system(s_color); 
}

void Menu::loadingScreen() {
	int bar1 = 177, bar2 = 219;
	std::cout << "\n\n\n\n\t\t\t\t\t\t  LOADING...";
	std::cout << "\n\n\n\n\t\t\t\t\t";
	for (int i = 0; i < 30; i++) {
		std::cout << (char)bar1;
	}
	std::cout << "\r";
	std::cout << "\t\t\t\t\t";
	for (int i = 0; i < 30; i++) {
		std::cout << (char)bar2;
		Sleep(15);
	}
	std::cout << "\n\n\t\t\t\t\t" << (char)1;
	system("Pause");
	std::cout << std::endl;
	system("cls");
}

void Menu::mainScreen() {
	std::cout << "\n\n\t\t\t\t\t\t  MAIN MENU";
	std::cout << "\n\n\n\n\t\t\t LOG IN[1]";
	std::cout << "\n\n\n\n\t\t\t  CHAT [2]";
	std::cout << "\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t  EXIT [0]";
}

void Menu::menuSwitch()
{
	//bool exit = false;
	system("cls");
	mainScreen();
	//	Make the function recursive so when you enter a magic key word or number you get sent
	// back to the menuSwitch(main screen) and you can choose again what you want to do
	int choose;
	std::cin >> choose;
	switch (choose) {
	case 0:
		system("cls");//exit the app
		std::cout << "\n\n\n\n\n\t\t\t\t\t\t Goodbye. =)\n\n\n\n\n\t\t\t\t\t\t";
		break;
	case 1:
		system("cls");
		std::cout << "\n\n\n\n\n\t\t\t\t Username: ";
		getName();
		menuSwitch();
		break;
	case 2:
		int retValue;
		system("cls");
		if (k == 0) {
			++k;
			retValue = start();
		}
		else {
			retValue = chat();
		}
		
		if(retValue == 2){
			system("cls");
			std::cout << "\n\n\n\n\n\t\t\tNo users are online at the moment, you will be redirected back to the main screen.\n\n\t\t\t\t\t";
			system("Pause");
			menuSwitch();
			break;
		}
		else if (retValue == 1) {
			menuSwitch();
			break;
		}
	case 3:
		system("cls");
		std::cout << "action 3";//see online friends 
		break;
	default:
		system("cls");
		std::cout << "\n\n\n\t\t\t\t\tInvalid action, please try again.";
		Sleep(1000);
		menuSwitch();
	}
}

