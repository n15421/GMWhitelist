#include "Entry.h"
#include "Global.h"

struct WhitelistParam {
    enum class Action { add, remove } action;
    std::string name;
};

void RegWhitelistCmd() {
    auto& cmd = ll::command::CommandRegistrar::getInstance().getOrCreateCommand(
        "whitelist",
        tr("command.whitelist.desc"),
        (CommandPermissionLevel)GMWhitelist::Entry::getInstance()->getConfig().CommandPermissionLevel
    );
    ll::service::getCommandRegistry()->registerAlias("whitelist", "allowlist");
    cmd.overload<WhitelistParam>()
        .required("action")
        .required("name")
        .execute<[](CommandOrigin const& origin, CommandOutput& output, WhitelistParam const& param) {
            auto type = origin.getOriginType();
            if (type == CommandOriginType::DedicatedServer || type == CommandOriginType::Player) {
                auto player = param.name;
                if (magic_enum::enum_name(param.action) == "add") {
                    auto res = addPlayer(player);
                    if (res) {
                        return output.success(tr("command.whitelist.addSuccess", {player}));
                    }
                    return output.error(tr("command.whitelist.isInWhitelist", {player}));
                } else {
                    auto res = removePlayer(player);
                    if (res) {
                        return output.success(tr("command.whitelist.removeSuccess", {player}));
                    }
                    return output.error(tr("command.whitelist.notInWhitelist", {player}));
                }
            }
            return output.error(tr("command.error.invalidCommandOrigin"));
        }>();
    cmd.overload<WhitelistParam>()
        .text("reload")
        .execute<[](CommandOrigin const& origin, CommandOutput& output, WhitelistParam const& param) {
            auto type = origin.getOriginType();
            if (type == CommandOriginType::DedicatedServer || type == CommandOriginType::Player) {
                initDataFile();
                return output.success(tr("command.whitelist.reload"));
            }
            return output.error(tr("command.error.invalidCommandOrigin"));
        }>();
    cmd.overload<WhitelistParam>()
        .text("list")
        .execute<[](CommandOrigin const& origin, CommandOutput& output, WhitelistParam const& param) {
            auto type = origin.getOriginType();
            if (type == CommandOriginType::DedicatedServer || type == CommandOriginType::Player) {
                return showWhitelist(output);
            }
            return output.error(tr("command.error.invalidCommandOrigin"));
        }>();
}

void RegisterCommands() { RegWhitelistCmd(); }