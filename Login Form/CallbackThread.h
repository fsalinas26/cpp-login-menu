#pragma once
void CallbackThread()
{
	for (;;)
	{
		if (beginLogin)
		{
			Server::LOGIN(globalUser.username, globalUser.password, "3UI1LI891P1", g_response);
			Sleep(1000);
			beginLogin = false;
		}
		if (beginRegister)
		{
			Server::REGISTER(globalUser.username, globalUser.password, "3UI1LI891P1",globalUser.variable, g_response);
			Sleep(1000);
			beginRegister = false;
		}
		Sleep(1);
	}
}