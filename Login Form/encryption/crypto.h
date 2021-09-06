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

class c_crypto
{
public:
	std::string iv;
	std::string key;
	static std::string encrypt(const std::string str_in, const std::string key, const std::string iv);
	static std::string decrypt(const std::string str_in, const std::string key, const std::string iv);
	static std::string base64url_safe(std::string str_in);
	static std::string SHA256_HASH(const std::string password);
private:
};
