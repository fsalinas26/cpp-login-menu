#include <stdlib.h>
#include <string>

enum Call
{
	LOGIN,
	REGISTER,
	FORGOT_PASSWORD,
	RESET_HWID
};

class Auth {
public:
	
	static void LOGIN(std::string username, std::string password, std::string HWID, std::string& response);
	static void REGISTER(std::string username, std::string password, std::string HWID, std::string license, std::string& response);
	static void RESET_PASSWORD(std::string username, std::string newPassword, std::string license, std::string& response);
	static void RESET_HWID(std::string username, std::string password, std::string& response);
};