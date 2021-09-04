#pragma once
#include <vector>
#include <stdlib.h>
#include <string>

struct post_request_fields {
	char key[128];
	std::string value;
};


enum Call
{
	LOGIN,
	REGISTER,
	FORGOT,
	RESET
};


class Server {
public:
	static void GET(std::wstring URL, std::string& response);
	static void GET(std::wstring URL, std::string& response,std::vector<std::wstring> &headers);
	static void POST(std::wstring URL, std::vector<post_request_fields> request, std::string &response);
	static void POST(std::wstring URL, std::vector<post_request_fields> request, std::string &response, std::vector<std::wstring> &headers);
	static void LOGIN(std::string username, std::string password, std::string HWID, std::string& response);
	static void REGISTER(std::string username, std::string password, std::string HWID, std::string license, std::string& response);
	static void FORGOT(std::string username, std::string newPassword, std::string license, std::string& response);
	static void RESET(std::string username, std::string password, std::string& response);
};
