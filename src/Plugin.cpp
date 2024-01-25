#include "Plugin.h"
#include "Global.h"

ll::Logger logger(PLUGIN_NAME);

namespace plugin {

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) {
    // Code for loading the plugin goes here.
    initConfigFile();
}

bool Plugin::enable() {
    // Code for enabling the plugin goes here.
    RegisterCommands();
    logger.info("GMWhitelist Loaded!");
    logger.info("Author: Tsubasa6848");
    logger.info("Repository: https://github.com/GroupMountain/GMWhitelist");
    return true;
}

bool Plugin::disable() {
    // Code for disabling the plugin goes here.
    unloadPlugin();
    logger.info("GMWhitelist Unloaded!");
    return true;
}

} // namespace plugin