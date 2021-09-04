#include "hwid.h"
#pragma warning (disable: 4474)
#pragma warning (disable: 4172)

std::string hwid::get_hardware_id(const std::string id)
{
	HANDLE h_token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &h_token))
	{
		GetLastError();
		return nullptr;
	}
	DWORD dw_buffer_size = 0;
	if (!GetTokenInformation(h_token, TokenUser, nullptr, 0, &dw_buffer_size) && (GetLastError() !=
		ERROR_INSUFFICIENT_BUFFER))
	{
		GetLastError();
		CloseHandle(h_token);
		h_token = nullptr;
		return nullptr;
	}
	std::vector<BYTE> buffer;
	buffer.resize(dw_buffer_size);
	const auto p_token_user = reinterpret_cast<PTOKEN_USER>(&buffer[0]);
	if (!GetTokenInformation(h_token, TokenUser, p_token_user, dw_buffer_size, &dw_buffer_size))
	{
		GetLastError();

		CloseHandle(h_token);
		h_token = nullptr;

		return nullptr;
	}
	if (!IsValidSid(p_token_user->User.Sid))
	{
		CloseHandle(h_token);
		h_token = nullptr;

		return nullptr;
	}
	CloseHandle(h_token);
	h_token = nullptr;
	LPTSTR psz_sid = nullptr;
	if (!ConvertSidToStringSid(p_token_user->User.Sid, &psz_sid))
	{
		return nullptr;
	}
	std::string psz_sid_str(psz_sid);
	psz_sid_str += id;
	return std::string(psz_sid_str);
}
