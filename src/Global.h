#pragma once
#include <include_all.h>
#include <sstream>

#define PLUGIN_NAME "GMWhitelist"

extern ll::Logger logger;

extern std::string tr(std::string const& key, std::vector<std::string> const& data = {});

extern void initDataFile();
extern void showWhitelist(CommandOutput& output);
extern void RegisterCommands();
extern void listenEvent();

extern bool addPlayer(std::string const& name);
extern bool removePlayer(std::string const& name);