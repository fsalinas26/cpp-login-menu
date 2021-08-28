#include "Server.h"
#include "lw_http.hpp"

//*GET
//Content type will default to application/x-www-form-urlencoded if no headers are provided
void Server::GET(std::wstring URL, std::string& response)
{
	c_lw_http http_obj;
	if (!(http_obj.open_session()))
	{
		return;//socket failed to open
	}
	http_obj.get(URL, response);
	http_obj.close_session();
}

//*GET w/ headers
//Use header param to declare Authentication fields (i.e. bearer token) and content-type
void Server::GET(std::wstring URL, std::string& response, std::vector<std::wstring>& headers)
{
	c_lw_http http_obj;
	if (!(http_obj.open_session()))
	{
		return;//socket failed to open
	}
	http_obj.get(URL, response,headers);
	http_obj.close_session();
}

void Server::POST(std::wstring URL, std::vector<post_request> body, std::string& response)
{
}

void Server::POST(std::wstring URL, std::vector<post_request> body, std::string& response, std::vector<std::wstring>& headers)
{
}
