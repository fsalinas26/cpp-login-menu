#include <Windows.h>
#include "crypto.h"
#include "xor.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "modes.h"
#include "files.h"
#include "osrng.h"
#include "hex.h"
#include "sha.h"

using namespace CryptoPP;


static std::string b64_to_binary(const std::string str_in)
{
	std::string str_out;
	StringSource b64_ss(str_in, true,
		new Base64Decoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}
std::string c_crypto::SHA256_HASH(const std::string str_in)
{
	std::string str_out;
	SHA256 hash;
	hash.Update((const byte*)str_in.data(), str_in.size());
	str_out.resize(hash.DigestSize());
	hash.Final((byte*)&str_out[0]);
	return str_out;
}
std::string c_crypto::base64url_safe(std::string str_in)
{
	str_in = b64_to_binary(str_in);
	std::string str_out;
	StringSource b64_urls(str_in, true,
		new Base64URLEncoder(
			new StringSink(str_out)
		)
	);
	return str_out;
}

std::string c_crypto::encrypt(const std::string str_in, const std::string key, const std::string iv)
{
	std::string cipher, binary_key, binary_iv;

	binary_key = b64_to_binary(key);
	binary_iv = b64_to_binary(iv);

	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption;
	encryption.SetKeyWithIV((byte*)binary_key.c_str(), binary_key.length(), (byte*)binary_iv.c_str());

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
	std::string str_out, binary_key, binary_iv,binary_cipher;

	binary_key = b64_to_binary(key);
	binary_iv = b64_to_binary(iv);
	binary_cipher = b64_to_binary(cipher);
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption;
	decryption.SetKeyWithIV((byte*)binary_key.c_str(), binary_key.length(), (byte*)binary_iv.c_str());
	
	StringSource s(binary_cipher, true,
		new StreamTransformationFilter(decryption,
			new StringSink(str_out)
		)
	); 
	return str_out;
}
