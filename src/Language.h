#pragma once
#include "Global.h"

std::string defaultLanguage_en_US = R"(
    disconnect.notAllowed=You are not whitelisted in this server!
    disconnect.clientNotAuth=The client is not logged in! \n\nTo access the server, log in to your Xbox account!
    command.whitelist.noInfo=There is nothing in the whitelist!
    command.whitelist.reload=Whitelist has been reloaded!
    permission.error.invalidLevel=Invalid command permission level. Reset to default value!
    permission.warning.dangerousLevel=[WARNING] Setting the command permission level to 0 is dangerous! This means that everyone can use this plugin command!
    command.whitelist.desc=Whitelist
    command.whitelist.isInWhitelist=Player %1$s is already whitelisted!
    command.whitelist.notInWhitelist=Player %1$s is not whitelisted!
    command.whitelist.addSuccess=Player %1$s is successfully added into whitelist!
    command.whitelist.removeSuccess=Player %1$s is successfully removed from whitelist!
    command.error.invalidCommandOrigin=This command can only be executed by the player or the console!
    command.whitelist.showInfo=Whitelist Info:
    command.whitelist.whitelistInfo=Player ID: %1$s , UUID: %2$s
)";

std::string defaultLanguage_zh_CN = R"(
    disconnect.notAllowed=你不在服务器白名单内！\n\n如需加入服务器，请先加入QQ群获取白名单！\n服务器QQ群：1145141919810
    disconnect.clientNotAuth=客户端未登录！\n\n如需进入服务器，请登录Xbox账户！
    command.whitelist.noInfo=白名单内没有任何内容！
    command.whitelist.reload=已重载白名单！
    permission.error.invalidLevel=无效的命令权限等级！已重置为默认值！
    permission.warning.dangerousLevel=[警告] 将命令权限等级设置为 0 是十分危险的！这意味着所有人都可以使用本插件命令！
    command.whitelist.desc=服务器白名单
    command.whitelist.isInWhitelist=玩家 %1$s 已在白名单中！
    command.whitelist.notInWhitelist=玩家 %1$s 不在白名单中！
    command.whitelist.addSuccess=已成功将玩家 %1$s 加入白名单！
    command.whitelist.removeSuccess=已成功将玩家 %1$s 移除白名单！
    command.error.invalidCommandOrigin=该命令只能由玩家或控制台执行！
    command.whitelist.showInfo=服务器白名单信息如下：
    command.whitelist.whitelistInfo=玩家ID: %1$s , UUID: %2$s
)";
