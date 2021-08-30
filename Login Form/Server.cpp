#include "Server.h"
#include "lw_http.hpp"
//#include "UserClass.h"

//*GET
//Content type will default to application/x-www-form-urlencoded if no headers are provided
void Server::GET(std::wstring URL, std::string& response)
{
	c_lw_http http_obj;
	if (!(http_obj.open_session()))
	{
		response = "null";
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
		response = "null";
		return;//socket failed to open
	}
	http_obj.get(URL, response,headers);
	http_obj.close_session();
}

//*POST
//Content type will default to application/x-www-form-urlencoded if no headers are provided
//Use fields param to declare data to be sent over througt the Post request
void Server::POST(std::wstring URL, std::vector<post_request_fields> fields, std::string& response)
{
	c_lw_http send;
	c_lw_httpd request;
	if (!(send.open_session()))
	{
		response = "null";//null response if the socket was unable to be opened
		return;
	}
	for (int i = 0; i < fields.size(); i++)
	{
		request.add_field(fields[i].key, fields[i].value.c_str());
	}
	std::wstring wURL = std::wstring(URL.begin(), URL.end());
	send.post(wURL, response, request);
	send.close_session();
}

//*POST w/ headers
//Use header param to declare Authentication fields (i.e. bearer token) and content-type
//Use fields param to declare data to be sent over througt the Post request
void Server::POST(std::wstring URL, std::vector<post_request_fields> fields, std::string& response, std::vector<std::wstring>& headers)
{
	c_lw_http send;
	c_lw_httpd request;
	if (!(send.open_session()))
	{
		response = "null";//null response if the socket was unable to be opened
		return;
	}
	for (int i = 0; i < fields.size(); i++)
	{
		request.add_field(fields[i].key, fields[i].value.c_str());
	}
	std::wstring wURL = std::wstring(URL.begin(), URL.end());
	send.post(wURL, response, request);
	send.close_session();
}

//LOGIN 
//(encryption soon to be added)
void Server::LOGIN(std::string username, std::string password, std::string HWID, std::string &response)
{
	Server::POST(L"http://localhost/post", { {"command","login"},{"username",username.c_str()},{"password",password.c_str()},{"hwid",HWID.c_str()} }, response);
}

//Register
void Server::REGISTER(std::string username, std::string password, std::string HWID,std::string license, std::string& response)
{
	Server::POST(L"http://localhost/post", { {"command","redeem"},{"username",username.c_str()},{"password",password.c_str()},{"hwid",HWID.c_str()},{"license",license} }, response);
}
