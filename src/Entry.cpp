#include "Entry.h"
#include "Global.h"
#include "Language.h"
ll::Logger logger(PLUGIN_NAME);
int        commandPermissionLevel = 4;

namespace GMWhitelist {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() { return true; }

bool Entry::enable() {
    mConfig.emplace();
    if (!ll::config::loadConfig(*mConfig, getSelf().getConfigDir() / u8"config.json")) {
        ll::config::saveConfig(*mConfig, getSelf().getConfigDir() / u8"config.json");
    }
    mI18n.emplace(getSelf().getLangDir(), mConfig->language);
    mI18n->updateOrCreateLanguage("en_US", defaultLanguage_en_US);
    mI18n->updateOrCreateLanguage("zh_CN", defaultLanguage_zh_CN);
    mI18n->loadAllLanguages();
    mI18n->chooseLanguage(mConfig->language);
    commandPermissionLevel = mConfig->CommandPermissionLevel;
    if (commandPermissionLevel < 0 || commandPermissionLevel > 4) {
        mConfig->CommandPermissionLevel = 4;
        logger.error(tr("permission.error.invalidLevel"));
    }
    if (commandPermissionLevel == 0) {
        logger.warn(tr("permission.warning.dangerousLevel"));
    }
    initDataFile();
    RegisterCommands();
    listenEvent();
    logger.info("GMWhitelist Loaded!");
    logger.info("Author: Tsubasa6848");
    logger.info("Repository: https://github.com/GroupMountain/GMWhitelist");
    return true;
}

bool Entry::disable() {
    mConfig.reset();
    return true;
}


Config& Entry::getConfig() { return mConfig.value(); }

LangI18n& Entry::getI18n() { return *mI18n; }


} // namespace GMWhitelist

LL_REGISTER_PLUGIN(GMWhitelist::Entry, GMWhitelist::Entry::getInstance());

std::string tr(std::string const& key, std::vector<std::string> const& data) {
    return GMWhitelist::Entry::getInstance()->getI18n().get(key, data);
}