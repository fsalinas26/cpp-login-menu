#pragma once
#include <vector>
#include <stdlib.h>
#include <string>
#include "lw_http.hpp"

struct post_request_fields {
	char key[128];
	std::string value;
};


class Server {
public:
	static void GET(std::wstring URL, std::string& response);
	static void GET(std::wstring URL, std::string& response,std::vector<std::wstring> &headers);
	static void POST(std::wstring URL, std::vector<post_request_fields> request, std::string &response);
	static void POST(std::wstring URL, std::vector<post_request_fields> request, std::string &response, std::vector<std::wstring> &headers);
};
