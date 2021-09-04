#pragma once
#include <Windows.h>
#include <intrin.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sddl.h>
class hwid
{
public:
	static std::string get_hardware_id(const std::string id);
};
