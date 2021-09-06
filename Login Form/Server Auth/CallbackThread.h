#pragma once
#include "hwid.h"
#include "UserClass.h"

void ServerCall(int command)
{
	g_response = "";
	fetchingData = true;
	switch (command)
	{
	case LOGIN:
		Auth::LOGIN(globalUser.username, globalUser.password, hwid::get_hardware_id("1"), g_response);
		Sleep(500);
		break;
	case REGISTER:
		Auth::REGISTER(globalUser.username, globalUser.password, hwid::get_hardware_id("1"), globalUser.variable, g_response);
		Sleep(500);
		break;
	case FORGOT_PASSWORD:
		Auth::RESET_PASSWORD(globalUser.username, globalUser.password, globalUser.variable, g_response);
		Sleep(500);
		break;
	case RESET_HWID:
		Auth::RESET_HWID(globalUser.username, globalUser.password, g_response);
		Sleep(500);
		break;
	}
	fetchingData = false;
}