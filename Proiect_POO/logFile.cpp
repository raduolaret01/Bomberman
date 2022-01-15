#include "logFile.h"

std::fstream Logs::logF = std::fstream("logs/log.txt", std::fstream::out | std::fstream::trunc);