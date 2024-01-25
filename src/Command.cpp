#include "Global.h"

void RegWhitelistCmd(CommandRegistry& registry) {
    auto command = DynamicCommand::createCommand(
        registry,
        "whitelist",
        tr("command.whitelist.desc"),
        (CommandPermissionLevel)commandPermissionLevel
    );
    command->setAlias("allowlist");
    command->setEnum("Action", {"add", "remove"});
    command->setEnum("Reload", {"reload"});
    command->setEnum("List", {"list"});
    command->mandatory(
        "action",
        DynamicCommand::ParameterType::Enum,
        "Action",
        CommandParameterOption::EnumAutocompleteExpansion
    );
    command->mandatory(
        "reload",
        DynamicCommand::ParameterType::Enum,
        "Reload",
        CommandParameterOption::EnumAutocompleteExpansion
    );
    command->mandatory(
        "list",
        DynamicCommand::ParameterType::Enum,
        "List",
        CommandParameterOption::EnumAutocompleteExpansion
    );
    command->mandatory("player", DynamicCommand::ParameterType::String);
    command->addOverload({"action", "player"});
    command->addOverload({"list"});
    command->addOverload({"reload"});
    command->setCallback([](DynamicCommand const&                                    command,
                            CommandOrigin const&                                     origin,
                            CommandOutput&                                           output,
                            std::unordered_map<std::string, DynamicCommand::Result>& result) {
        auto type = origin.getOriginType();
        if (type == CommandOriginType::DedicatedServer || type == CommandOriginType::Player) {
            if (result["action"].isSet) {
                auto action = result["action"].get<std::string>();
                auto player = result["player"].get<std::string>();
                if (action == "add") {
                    auto res = addPlayer(player);
                    if (res) {
                        return output.success(tr("command.whitelist.addSuccess", {player}));
                    }
                    return output.error(tr("command.whitelist.isInWhitelist", {player}));
                }
                auto res = removePlayer(player);
                if (res) {
                    return output.success(tr("command.whitelist.removeSuccess", {player}));
                }
                return output.error(tr("command.whitelist.notInWhitelist", {player}));
            }
            if (result["list"].isSet) {
                return showWhitelist(output);
            }
            if (result["reload"].isSet) {
                initDataFile();
                return output.success(tr("command.whitelist.reload"));
            }
        }
        return output.error(tr("command.error.invalidCommandOrigin"));
    });
    DynamicCommand::setup(registry, std::move(command));
}

void RegisterCommands() {
    auto registry = ll::service::getCommandRegistry();
    RegWhitelistCmd(registry);
}