#pragma once
#include "Global.h"

std::string defaultConfig = R"({
    "language": "zh_CN",
    "CommandPermissionLevel": 4
})";

std::string defaultLanguage = R"({
    "disconnect.notAllowed": "你不在服务器白名单内！\n\n如需加入服务器，请先加入QQ群获取白名单！\n服务器QQ群：1145141919810",
    "disconnect.clientNotAuth": "客户端未登录！\n\n如需进入服务器，请登录Xbox账户！",
    "command.whitelist.noInfo": "白名单内没有任何内容！",
    "command.whitelist.reload": "已重载白名单！",
    "permission.error.invalidLevel": "无效的命令权限等级！已重置为默认值！",
    "permission.warning.dangerousLevel": "[警告] 将命令权限等级设置为 0 是十分危险的！这意味着所有人都可以使用本插件命令！",
    "command.whitelist.desc": "服务器白名单",
    "command.whitelist.isInWhitelist": "玩家 %1$s 已在白名单中！",
    "command.whitelist.notInWhitelist": "玩家 %1$s 不在白名单中！",
    "command.whitelist.addSuccess": "已成功将玩家 %1$s 加入白名单！",
    "command.whitelist.removeSuccess": "已成功将玩家 %1$s 移除白名单！",
    "command.error.invalidCommandOrigin": "该命令只能由玩家或控制台执行！"
})";
