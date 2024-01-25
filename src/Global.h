#pragma once
#include <GMLIB/Files/JsonConfig.h>
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Files/JsonLanguage.h>
#include <ll/api/Logger.h>
#include <ll/api/command/CommandRegistrar.h>
#include <ll/api/command/DynamicCommand.h>
#include <ll/api/memory/Hook.h>
#include <ll/api/service/Bedrock.h>
#include <mc/certificates/Certificate.h>
#include <mc/certificates/ExtendedCertificate.h>
#include <mc/network/ServerNetworkHandler.h>
#include <mc/network/packet/LoginPacket.h>
#include <mc/server/common/commands/AllowListCommand.h>
#include <mc/world/actor/player/Player.h>
#include <mc/world/level/Level.h>
#include <sstream>

#define PLUGIN_NAME "GMWhitelist"

extern ll::Logger logger;
extern int        commandPermissionLevel;

extern GMLIB::Files::JsonConfig* Config;

extern nlohmann::json mWhiteList;

extern std::string tr(std::string key, std::vector<std::string> data = {});

extern void initDataFile();
extern void showWhitelist(CommandOutput& output);
extern void initConfigFile();
extern void RegisterCommands();
extern void unloadPlugin();

extern bool addPlayer(std::string& name);
extern bool removePlayer(std::string& name);
