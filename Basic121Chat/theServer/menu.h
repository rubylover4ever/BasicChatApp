#pragma once
#include"server.h"
#include <windows.h>


class Menu : public Server{
public:
	void setSystemColor();
	void loadingScreen();
	void mainScreen();
	void menuSwitch();

private:
	const char* s_color = "COLOR fd";

};
