#include "Entry.h"
#include "Global.h"

ll::Logger logger(PLUGIN_NAME);

namespace GMWhitelist {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() {
    initPlugin();
    return true;
}

bool Entry::enable() {
    RegisterCommands();
    listenEvent();
    logger.info("GMWhitelist Loaded!");
    logger.info("Author: Tsubasa6848");
    logger.info("Repository: https://github.com/GroupMountain/GMWhitelist");
    return true;
}

bool Entry::disable() { return true; }

} // namespace GMWhitelist

LL_REGISTER_PLUGIN(GMWhitelist::Entry, GMWhitelist::Entry::getInstance());
