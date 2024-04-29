#include "Global.h"

// nlohmann::json mWhiteList;

std::unordered_map<mce::UUID, std::string> mWhiteListMap;
std::unordered_set<std::string>            mNameCache;

void saveWhitelistFile() {
    std::string    path = "./whitelist.json";
    nlohmann::json data;
    for (auto& [uuid, name] : mWhiteListMap) {
        auto info    = nlohmann::json::object();
        info["uuid"] = uuid.asString();
        info["name"] = name;
        data.push_back(info);
    }
    for (auto& name : mNameCache) {
        auto info    = nlohmann::json::object();
        info["name"] = name;
        data.push_back(info);
    }
    GMLIB::Files::JsonFile::writeFile(path, data);
}

void initDataFile() {
    auto data = GMLIB::Files::JsonFile::initJson("./whitelist.json", nlohmann::json::array());
    for (auto& info : data) {
        if (info.contains("name")) {
            std::string uuid                           = info["uuid"];
            mWhiteListMap[mce::UUID::fromString(uuid)] = info["name"];
        } else {
            mNameCache.insert(info["name"]);
        }
    }
}

bool isInWhitelist(mce::UUID const& uuid, std::string const& name) {
    if (mWhiteListMap.contains(uuid)) {
        if (mWhiteListMap[uuid] != name) {
            mWhiteListMap[uuid] = name;
            saveWhitelistFile();
        }
        return true;
    }
    if (mNameCache.contains(name)) {
        mWhiteListMap[uuid] = name;
        mNameCache.erase(name);
        saveWhitelistFile();
        return true;
    }
    return false;
}

bool isNameInWhitelist(std::string const& name) {
    if (mNameCache.contains(name)) {
        return true;
    }
    for (auto& [uuid, playername] : mWhiteListMap) {
        if (playername == name) {
            return true;
        }
    }
    return false;
}

bool addPlayer(std::string const& name) {
    if (isNameInWhitelist(name)) {
        return false;
    }
    mNameCache.insert(name);
    saveWhitelistFile();
    return true;
}

bool removePlayer(std::string const& name) {
    auto pl = ll::service::getLevel()->getPlayer(name);
    if (pl) {
        auto msg = tr("disconnect.notAllowed");
        pl->disconnect(msg);
    }
    for (auto& [uuid, playername] : mWhiteListMap) {
        if (playername == name) {
            mWhiteListMap.erase(uuid);
            saveWhitelistFile();
            return true;
        }
    }
    for (auto& playername : mNameCache) {
        if (playername == name) {
            mWhiteListMap.erase(name);
            saveWhitelistFile();
            return true;
        }
    }
    return false;
}

void showWhitelist(CommandOutput& output) {
    if (mWhiteListMap.empty()) {
        return output.error(tr("command.whitelist.noInfo"));
    }
    output.success(tr("command.whitelist.showInfo"));
    for (auto& [uuid, name] : mWhiteListMap) {
        output.success(tr("command.whitelist.whitelistInfo", {name, uuid.asString()}));
    }
    for (auto& name : mNameCache) {
        output.success(tr("command.whitelist.whitelistInfo", {name, ""}));
    }
}

void listenEvent() {
    auto& eventBus = ll::event::EventBus::getInstance();
    eventBus.emplaceListener<GMLIB::Event::PacketEvent::ClientLoginAfterEvent>(
        [](GMLIB::Event::PacketEvent::ClientLoginAfterEvent& event) {
            auto uuid     = event.getUuid();
            auto realName = event.getRealName();
            if (!isInWhitelist(uuid, realName)) {
                auto msg = tr("disconnect.notAllowed");
                event.disConnectClient(msg);
            }
        },
        ll::event::EventPriority::High
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