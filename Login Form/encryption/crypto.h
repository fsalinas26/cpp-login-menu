#pragma once
#include <winsvc.h>
#pragma warning (disable: 4191 4099)
#pragma comment (lib, "cryptlib.lib")
#include "./cryptopp/aes.h"
#include "./cryptopp/modes.h"
#include "./cryptopp/base64.h"
#include "./cryptopp/hex.h"
#include <iosfwd>
#include "cryptopp/stdcpp.h"
#include <algorithm>
#include <winioctl.h>
#include <winternl.h>
#include <iostream>
#include <string>
#include <random>
#include "json.hpp"
using namespace nlohmann;
class c_crypto
{
public:
	std::string iv;
	std::string key;
	std::string token;
	static std::string encrypt(const std::string str_in, const std::string key, const std::string iv);
	static std::string decrypt(const std::string str_in, const std::string key, const std::string iv);
	static std::map<std::string, std::string> decryptJson(const std::string key, const std::string iv, json& obj);
	static std::string b64_to_binary(const std::string str_in);
	static std::string MD5_HASH(const std::string in);
	static std::string generate_random_token();
private:
};
