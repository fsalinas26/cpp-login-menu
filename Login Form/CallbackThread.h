#pragma once

void ServerFetch(int command)
{
	fetchingData = true;
	switch (command)
	{
	case LOGIN:
		Server::LOGIN(globalUser.username, globalUser.password, "3UI1LI891P1", g_response);
		Sleep(500);
		break;
	case REGISTER:
		Server::REGISTER(globalUser.username, globalUser.password, "3UI1LI891P1", globalUser.variable, g_response);
		Sleep(500);
		break;
	default:
		break;
	}
	fetchingData = false;
}