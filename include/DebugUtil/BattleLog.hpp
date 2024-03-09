#pragma once

#include "Util/Time.hpp"

#define CONCAT__(a, b) a##b
#define printBattleLogImpl__(f, ...) printf(f, (uint64_t)Util::Time::GetElapsedTimeMs(), __VA_ARGS__)
#define printBattleLog(f, ...) printBattleLogImpl__(CONCAT__("[%llu] ", f), __VA_ARGS__)
