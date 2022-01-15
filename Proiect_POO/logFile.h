#pragma once

#include <fstream>

class Logs {
public:
	static std::fstream logF;

	Logs() = default;
	~Logs() = default;
};