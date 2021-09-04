#pragma once
#include "hwid.h"
void ServerFetch(int command)
{
	g_response = "";
	fetchingData = true;
	switch (command)
	{
	case LOGIN:
		Server::LOGIN(globalUser.username, globalUser.password, hwid::get_hardware_id("1"), g_response);
		Sleep(500);
		break;
	case REGISTER:
		Server::REGISTER(globalUser.username, globalUser.password, hwid::get_hardware_id("1"), globalUser.variable, g_response);
		Sleep(500);
		break;
	case FORGOT:
		Server::FORGOT(globalUser.username, globalUser.password, globalUser.variable, g_response);
		Sleep(500);
		break;
	case RESET:
		Server::RESET(globalUser.username, globalUser.password, g_response);
		Sleep(500);
		break;
	}
	fetchingData = false;
}