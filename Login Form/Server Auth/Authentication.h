#include <stdlib.h>
#include <string>
#include <map>
#define PUBLIC_TOKEN L"Authorization: PUBLIC_TOKEN" //Set this token in config.js on Node Server
enum Call
{
	LOGIN,
	REGISTER,
	FORGOT_PASSWORD,
	RESET_HWID
};

class Auth {
public:
	
	static void LOGIN(std::string username, std::string password, std::string HWID, std::map<std::string, std::string> &response);
	static void REGISTER(std::string username, std::string password, std::string HWID, std::string license, std::map<std::string, std::string> &response);
	static void RESET_PASSWORD(std::string username, std::string newPassword, std::string license, std::map<std::string, std::string> &response);
	static void RESET_HWID(std::string username, std::string password, std::map<std::string, std::string> &response);
};