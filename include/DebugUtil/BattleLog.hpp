#pragma once

#include "Util/Time.hpp"
#include "Util/Logger.hpp"

#define CONCAT__(a, b) a##b
#define printBattleLogImpl__(f, ...) LOG_DEBUG(f, Util::Time::GetElapsedTimeMs(), __VA_ARGS__)
//#define printBattleLog(f, ...) printBattleLogImpl__(CONCAT__("[TIME:{}] ", f), __VA_ARGS__)
#define printBattleLog(f, ...) LOG_DEBUG(f, Util::Time::GetElapsedTimeMs(), __VA_ARGS__)
#define ENABLE_BATTLE_LOG
