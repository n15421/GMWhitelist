#include "Global.h"
#include <GMLIB/Server/UserCache.h>

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

void handleWhitelist(NetworkIdentifier const& source, std::string& uuid, std::string& realName) {
    if (isInWhitelist(uuid) || isInWhitelist(realName)) {
        return;
    }
    auto msg = tr("disconnect.notAllowed");
    ll::service::getServerNetworkHandler()
        ->disconnectClient(source, Connection::DisconnectFailReason::Kicked, msg, false);
}

void showWhitelist(CommandOutput& output) {
    if (mWhiteList.empty()) {
        return output.success(tr("command.whitelist.noInfo"));
    }
    for (auto& key : mWhiteList) {
        auto info = mWhiteList.dump(4);
        output.success(info);
    }
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerLoginHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVLoginPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class LoginPacket const&       packet
) {
    origin(source, packet);
    auto cert       = packet.mConnectionRequest->getCertificate();
    auto uuid       = ExtendedCertificate::getIdentity(*cert);
    auto clientXuid = ExtendedCertificate::getXuid(*cert, true);
    auto realName   = ExtendedCertificate::getIdentityName(*cert);
    if (clientXuid.empty()) {
        std::string msg = tr("disconnect.clientNotAuth");
        ll::service::getServerNetworkHandler()
            ->disconnectClient(source, Connection::DisconnectFailReason::Kicked, msg, false);
    }
    auto strUuid = uuid.asString();
    handleWhitelist(source, strUuid, realName);
}

LL_AUTO_STATIC_HOOK(FuckVanillaCmd, ll::memory::HookPriority::Normal, "?setup@AllowListCommand@@SAXAEAVCommandRegistry@@AEAVAllowListFile@@@Z", void, class CommandRegistry& registry, class AllowListFile&) {
    return;
}

void unloadPlugin() {
    ll::memory::HookRegistrar<PlayerLoginHook>().unhook();
    auto registry = ll::service::getCommandRegistry();
    registry->unregisterCommand("whitelist");
    delete Config;
}