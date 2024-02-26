#pragma once

#include <ll/api/plugin/NativePlugin.h>

namespace GMWhitelist {

[[nodiscard]] auto getSelfPluginInstance() -> ll::plugin::NativePlugin&;

} // namespace GMWhitelist
