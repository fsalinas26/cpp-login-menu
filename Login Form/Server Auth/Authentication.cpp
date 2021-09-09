#include "Authentication.h"
#include "HttpRequest.h"
#include "crypto.h"
#include <json.hpp>

c_crypto g_crypto;
using namespace std;
using namespace nlohmann;

/**
* Initialize random session IV with server to encrypt data
* @returns base64url encoded IV
*/
static string GenerateIV()
{
	#ifdef DEBUG
	cout << "Starting Authentication Process..." << '\n';
	cout << "Initializing random session IV with server..." << '\n';
	#endif // DEBUG
	string generated_iv;
	vector<wstring> headers = { PUBLIC_TOKEN };
	Server::GET(L"http://localhost/initialize", generated_iv, headers);
	return generated_iv;
}

static void decryptResponse(const string res, map<string, string> &g_response)
{
	if (res.size() > 0)
	{
		auto obj = json::parse(res);
		if (obj["res"] == "Invalid Session")
			g_response["res"] = obj["res"];
		else {
			g_response = c_crypto::decryptJson(g_crypto.key, g_crypto.iv, obj);
		}
	}
	else
		g_response["res"] = "NULL";
}
/**
* Initializes session IV. Sends post request to server to login
* response returns the json body
*/
void Auth::LOGIN(string username, string password, string HWID, map<string, string> &response)
{
	try
	{
		c_lw_http send;
		c_lw_httpd request;
		g_crypto.iv = GenerateIV();
		if (g_crypto.iv.size() < 1)
		{
			response["res"] = "null";
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
		request.add_field("iv", (g_crypto.iv).c_str()); 

		string tempRes;
		vector<wstring> headers = { L"Authorization: PUBLIC_TOKEN" };
		send.post(L"http://localhost/post", tempRes, request, headers);
		decryptResponse(tempRes, response);
		send.close_session();
	}
	catch (int error)
	{
		cout << "Error occured number " << error << endl;
	}
}
//Reset HWID
//Using AES-256-CFB Encryption
void Auth::RESET_HWID(string username, string password, map<string, string> &response)
{
	try
	{
		c_lw_http send;
		c_lw_httpd request;

		g_crypto.iv = GenerateIV();

		if (g_crypto.iv.size() < 1)
		{
			response["res"] = "null";
			return;
		}
		if (!(send.open_session()))
		{
			return; //return if socket was unable to be opened
		}
		request.add_field("command", c_crypto::encrypt("resethwid", g_crypto.key, g_crypto.iv).c_str());
		request.add_field("username", c_crypto::encrypt(username, g_crypto.key, g_crypto.iv).c_str());
		request.add_field("password", c_crypto::SHA256_HASH(password).c_str());
		request.add_field("iv", (g_crypto.iv).c_str());

		string tempRes;
		vector<wstring> headers = { L"Authorization: PUBLIC_TOKEN" };
		send.post(L"http://localhost/post", tempRes, request, headers);
		decryptResponse(tempRes, response);
		send.close_session();
	}
	catch (int error)
	{
		cout << "Error occured number " << error << endl;
	};
}


//Register
//Using AES-256-CFB Encryption
void Auth::REGISTER(string username, string password, string HWID, string license, map<string, string> &response)
{
	try
	{
		c_lw_http send;
		c_lw_httpd request;

		g_crypto.iv = GenerateIV();

		if (g_crypto.iv.size() < 1)
		{
			response["res"] = "null";
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
		request.add_field("iv", (g_crypto.iv).c_str());

		string tempRes;
		vector<wstring> headers = { L"Authorization: PUBLIC_TOKEN" };
		send.post(L"http://localhost/post", tempRes, request, headers);
		decryptResponse(tempRes, response);
		send.close_session();
	}
	catch (int error)
	{
		cout << "Error occured number " << error << endl;
	};
}

//Reset Password
void Auth::RESET_PASSWORD(string username, string newPassword, string license, map<string, string> &response)
{
	try
	{
		c_lw_http send;
		c_lw_httpd request;

		g_crypto.iv = GenerateIV();

		if (g_crypto.iv.size() < 1)
		{
			response["res"] = "null";
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
		request.add_field("iv", (g_crypto.iv).c_str());

		string tempRes;
		vector<wstring> headers = { L"Authorization: PUBLIC_TOKEN" };
		send.post(L"http://localhost/post", tempRes, request, headers);
		decryptResponse(tempRes, response);
		send.close_session();
	}
	catch (int error)
	{
		cout << "Error occured number " << error << endl;
	};
}