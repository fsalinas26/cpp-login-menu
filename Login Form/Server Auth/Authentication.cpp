#include "Authentication.h"
#include "HttpRequest.h"
#include "crypto.h"
#include "json.hpp"

c_crypto g_crypto;
using namespace nlohmann;
//Generate IV from Server
static std::string GenerateIV()
{
	std::string generated_iv;
	Server::GET(L"http://localhost/initialize", generated_iv);
	return generated_iv;
}
//Login
//Using AES-256-CFB Encryption
void Auth::LOGIN(std::string username, std::string password, std::string HWID, std::string& response)
{
	try
	{
		c_lw_http send;
		c_lw_httpd request;
		response = "null";

		g_crypto.iv = GenerateIV();

		if (g_crypto.iv.size() < 1) 
		{
			return;
		}
		if (!(send.open_session()))
		{
			return; //return if socket was unable to be opened
		}
		request.add_field("command", c_crypto::encrypt("login", g_crypto.key, g_crypto.iv).c_str());
		request.add_field("username", c_crypto::encrypt(username, g_crypto.key, g_crypto.iv).c_str());
		request.add_field("password", c_crypto::SHA256_HASH(password).c_str());
		request.add_field("hwid", c_crypto::SHA256_HASH(HWID).c_str());
		request.add_field("iv", c_crypto::base64url_safe(g_crypto.iv).c_str());

		std::string tempRes;
		send.post(L"http://localhost/post", tempRes, request);
		if (tempRes.size() > 0)
		{
			auto obj = json::parse(tempRes);
			response = c_crypto::decrypt(obj["res"], g_crypto.key, g_crypto.iv);
		}
		else
			response = "NULL RESPONSE";
		send.close_session();
	}
	catch (int error)
	{
		std::cout << "Error occured number " << error << std::endl;
	}
}
//Reset HWID
//Using AES-256-CFB Encryption
void Auth::RESET_HWID(std::string username, std::string password, std::string& response)
{
	c_lw_http send;
	c_lw_httpd request;
	response = "null";

	g_crypto.iv = GenerateIV();

	if (g_crypto.iv.size() < 1)
	{
		return;
	}
	if (!(send.open_session()))
	{
		return; //return if socket was unable to be opened
	}
	request.add_field("command", c_crypto::encrypt("resethwid", g_crypto.key, g_crypto.iv).c_str());
	request.add_field("username", c_crypto::encrypt(username, g_crypto.key, g_crypto.iv).c_str());
	request.add_field("password", c_crypto::SHA256_HASH(password).c_str());
	request.add_field("iv", c_crypto::base64url_safe(g_crypto.iv).c_str());

	std::string tempRes;
	send.post(L"http://localhost/post", tempRes, request);
	if (tempRes.size() > 0)
	{
		auto obj = json::parse(tempRes);
		response = c_crypto::decrypt(obj["res"], g_crypto.key, g_crypto.iv);
	}
	else
		response = "NULL RESPONSE";
	send.close_session();
}


//Register
//Using AES-256-CFB Encryption
void Auth::REGISTER(std::string username, std::string password, std::string HWID, std::string license, std::string& response)
{
	c_lw_http send;
	c_lw_httpd request;
	response = "null";

	g_crypto.iv = GenerateIV();

	if (g_crypto.iv.size() < 1)
	{
		return;
	}
	if (!(send.open_session()))
	{
		return; //return if socket was unable to be opened
	}
	request.add_field("command", c_crypto::encrypt("redeem", g_crypto.key, g_crypto.iv).c_str());
	request.add_field("username", c_crypto::encrypt(username, g_crypto.key, g_crypto.iv).c_str());
	request.add_field("password", c_crypto::SHA256_HASH(password).c_str());
	request.add_field("license", c_crypto::encrypt(license, g_crypto.key, g_crypto.iv).c_str());
	request.add_field("hwid", c_crypto::SHA256_HASH(HWID).c_str());
	request.add_field("iv", c_crypto::base64url_safe(g_crypto.iv).c_str());

	std::string tempRes;
	send.post(L"http://localhost/post", tempRes, request);
	if (tempRes.size() > 0)
	{
		auto obj = json::parse(tempRes);
		response = c_crypto::decrypt(obj["res"], g_crypto.key, g_crypto.iv);
	}
	else
		response = "NULL RESPONSE";
	send.close_session();
}

//Reset Password
void Auth::RESET_PASSWORD(std::string username, std::string newPassword, std::string license, std::string& response)
{
	c_lw_http send;
	c_lw_httpd request;
	response = "null";

	g_crypto.iv = GenerateIV();

	if (g_crypto.iv.size() < 1)
	{
		return;
	}
	if (!(send.open_session()))
	{
		return; //return if socket was unable to be opened
	}
	request.add_field("command", c_crypto::encrypt("resetpw", g_crypto.key, g_crypto.iv).c_str());
	request.add_field("username", c_crypto::encrypt(username, g_crypto.key, g_crypto.iv).c_str());
	request.add_field("newPassword", c_crypto::SHA256_HASH(newPassword).c_str());
	request.add_field("license", c_crypto::encrypt(license, g_crypto.key, g_crypto.iv).c_str());
	request.add_field("iv", c_crypto::base64url_safe(g_crypto.iv).c_str());

	std::string tempRes;
	send.post(L"http://localhost/post", tempRes, request);
	if (tempRes.size() > 0)
	{
		auto obj = json::parse(tempRes);
		response = c_crypto::decrypt(obj["res"], g_crypto.key, g_crypto.iv);
	}
	else
		response = "NULL RESPONSE";
	send.close_session();
}