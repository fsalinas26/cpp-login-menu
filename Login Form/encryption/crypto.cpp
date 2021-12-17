#include <Windows.h>
#include "crypto.h"
#include "xor.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "cryptopp/md5.h"
#include "sha.h"
using namespace CryptoPP;
std::string c_crypto::b64_to_binary(const std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64Decoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}
std::string c_crypto::generate_random_token()
{
	SecByteBlock key(AES::DEFAULT_KEYLENGTH), iv(AES::BLOCKSIZE);
	std::string token;
	OS_GenerateRandomBlock(true, key, key.size());
	StringSource b64_ss(key.data(), key.size(), true,
		new Base64URLEncoder(
			new StringSink(token)
		)
	);
	return token;
}
static std::string b64URL_to_binary(const std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64URLDecoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}

std::string c_crypto::MD5_HASH(const std::string str_in)
{
	std::string str_out;
	MD5 hash;
	hash.Update((const byte*)str_in.data(), str_in.size());
	str_out.resize(hash.DigestSize());
	hash.Final((byte*)&str_out[0]);
	return str_out;
}

std::string c_crypto::encrypt(const std::string str_in, const std::string key, const std::string iv)
{
	std::string cipher, binary_iv;

	binary_iv = b64URL_to_binary(iv);

	CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption encryption;
	encryption.SetKeyWithIV((byte*)key.c_str(), key.length(), (byte*)binary_iv.c_str());

	StringSource s(str_in, true,
		new StreamTransformationFilter(encryption,
			new Base64URLEncoder(//encode our cipher in base64url
				new StringSink(cipher),
				false 
			)
		)
	);
	return cipher;
}


std::string c_crypto::decrypt(const std::string cipher, const std::string key, const std::string iv)
{
	std::string str_out, binary_iv,binary_cipher;

	binary_iv = b64URL_to_binary(iv);
	binary_cipher = b64URL_to_binary(cipher);
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
	decryption.SetKeyWithIV((byte*)key.c_str(), key.length(), (byte*)binary_iv.c_str());
	
	StringSource s(binary_cipher, true,
		new StreamTransformationFilter(decryption,
			new StringSink(str_out)
		)
	); 
	return str_out;
}
static void debug_json_output(json& obj)
{
	std::cout << "JSON PARSED OBJECT \n" << std::string(30, '*') << '\n';
	for (auto& [key, value] : obj.items())
	{
		if (value.is_string())
		{
			std::cout << key << value.get<std::string>() << '\n';
		}
	}
}

std::map<std::string, std::string> c_crypto::decryptJson(const std::string aes_key, const std::string iv, json &obj)
{

	std::string binary_iv;
	std::map<std::string, std::string> map_out;
	binary_iv = b64URL_to_binary(iv);
	for (auto& [key, value] : obj.items())
	{
		if (value.is_string() && key!="token")
		{
			std::string newValue;
			CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
			decryption.SetKeyWithIV((byte*)aes_key.c_str(), aes_key.length(), (byte*)binary_iv.c_str());
			StringSource s(b64URL_to_binary(value.get<std::string>()), true,
				new StreamTransformationFilter(decryption,
					new StringSink(newValue)
				)
			);
			map_out[key] = newValue;
		}
	}
	return map_out;
	
}
