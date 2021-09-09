#pragma once
#include <string>
ImFont* Smallfont;
ImFont* LargeFont;
std::map<std::string, std::string> g_response;

bool fetchingData = false;

enum Page {
	Login,
	Menu
}currentPage;


 
