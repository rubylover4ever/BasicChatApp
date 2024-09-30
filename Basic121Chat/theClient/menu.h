#pragma once
#include"client.h"
#include <windows.h>


class Menu : public Client{
public:
	void setSystemColor();
	void loadingScreen();
	void mainScreen();
	void menuSwitch();

private:
	const char* s_color = "COLOR fd";

};
