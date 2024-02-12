#pragma once
#include <include_all.h>
#include <sstream>

#define PLUGIN_NAME "GMWhitelist"

extern ll::Logger logger;

extern nlohmann::json mWhiteList;

extern int commandPermissionLevel;

extern std::string tr(std::string key, std::vector<std::string> data = {});

extern void initDataFile();
extern void showWhitelist(CommandOutput& output);
extern void initPlugin();
extern void RegisterCommands();
extern void listenEvent();

extern bool addPlayer(std::string& name);
extern bool removePlayer(std::string& name);