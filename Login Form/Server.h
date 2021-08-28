#pragma once
#include "lw_http.hpp"

struct post_request {
	char* field1;
	char* content;
};
class Server {
public: 
	void GET(std::wstring URL, std::string& response);
	void GET(std::wstring URL, std::string& response,std::vector<std::wstring> &headers);
	void POST(std::wstring URL, std::vector<post_request> body, std::string& response);
	void POST(std::wstring URL, std::vector<post_request> body, std::string& response, std::vector<std::wstring>& headers);
};