#include "Global.h"

nlohmann::json mWhiteList;

void saveWhitelistFile() {
    std::string path = "./whitelist.json";
    GMLIB::Files::JsonFile::writeFile(path, mWhiteList);
}

void initDataFile() {
    auto emptyFile = nlohmann::json::array();
    mWhiteList     = GMLIB::Files::JsonFile::initJson("./whitelist.json", emptyFile);
}

bool isInWhitelist(std::string& info) {
    for (auto& key : mWhiteList) {
        if (key.contains("uuid")) {
            if (key["uuid"] == info) {
                return true;
            }
        } else {
            if (key["name"] == info) {
                auto data = GMLIB::Server::UserCache::tryFindCahceInfoFromName(info);
                if (data.has_value()) {
                    key["uuid"] = data.value().at("uuid");
                    saveWhitelistFile();
                }
                return true;
            }
        }
    }
    return false;
}

bool addPlayer(std::string& name) {
    if (isInWhitelist(name)) {
        return false;
    }
    auto info = GMLIB::Server::UserCache::tryFindCahceInfoFromName(name);
    auto key  = nlohmann::json::object();
    if (info.has_value()) {
        auto cacheUuid = info.value()["uuid"].get<std::string>();
        if (isInWhitelist(cacheUuid)) {
            return false;
        }
        key["uuid"] = cacheUuid;
    }
    key["name"] = name;
    mWhiteList.push_back(key);
    saveWhitelistFile();
    return true;
}

bool removePlayer(std::string& name) {
    auto pl = ll::service::getLevel()->getPlayer(name);
    if (pl) {
        auto msg = tr("disconnect.notAllowed");
        pl->disconnect(msg);
    }
    for (auto it = mWhiteList.begin(); it != mWhiteList.end(); ++it) {
        if (it.value().at("name") == name) {
            mWhiteList.erase(it);
            --it;
            saveWhitelistFile();
            return true;
        }
    }
    return false;
}

void showWhitelist(CommandOutput& output) {
    if (mWhiteList.empty()) {
        return output.success(tr("command.whitelist.noInfo"));
    }
    auto info = mWhiteList.dump(4);
    return output.success(info);
}

void listenEvent() {
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.emplaceListener<GMLIB::Event::PlayerEvent::PlayerLoginAfterEvent>(
        [](GMLIB::Event::PlayerEvent::PlayerLoginAfterEvent& event) {
            auto uuid     = event.getUuid().asString();
            auto realName = event.getRealName();
            if (!isInWhitelist(uuid) && !isInWhitelist(realName)) {
                auto msg = tr("disconnect.notAllowed");
                event.disConnectClient(msg);
            }
        },
        ll::event::EventPriority::Lowest
    );
}

LL_AUTO_STATIC_HOOK(
    FuckVanillaCmd,  // Vinalla Command Registry
    ll::memory::HookPriority::Normal, 
    "?setup@AllowListCommand@@SAXAEAVCommandRegistry@@AEAVAllowListFile@@@Z", 
    void, 
    class CommandRegistry& registry, 
    class AllowListFile&
) {
    return;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    DisableVanillaAllowlist, // server.properties
    ll::memory::HookPriority::Normal,
    PropertiesSettings,
    "?useAllowList@PropertiesSettings@@QEBA_NXZ",
    bool
) {
    return false;
}