#pragma once
#include "core.h"
#include "spdlog/spdlog.h"

class JNAPI LogSystem
{
public:
	~LogSystem();

	static void init();
	static spdlog::logger* getLogger();

private:
	LogSystem();

	static spdlog::logger* Logger;
};
#define JNLOGTRACE(...) LogSystem::getLogger()->trace(__VA_ARGS__);
#define JNLOGINFO(...) LogSystem::getLogger()->info(__VA_ARGS__);
#define JNLOGDEBUG(...) LogSystem::getLogger()->debug(__VA_ARGS__);
#define JNLOGWARN(...) LogSystem::getLogger()->warn(__VA_ARGS__);
#define JNLOGERROR(...) LogSystem::getLogger()->error(__VA_ARGS__);