#pragma once
#include <vector>
#include <stdlib.h>
#include <string>

struct post_request_fields {
	char* key;
	char* value;
};
class Server {
public: 
	static void GET(std::wstring URL, std::string& response);
	static void GET(std::wstring URL, std::string& response,std::vector<std::wstring> &headers);
	static void POST(std::wstring URL, std::vector<post_request_fields> request, std::string &response);
	static void POST(std::wstring URL, std::vector<post_request_fields> request, std::string &response, std::vector<std::wstring> &headers);
};