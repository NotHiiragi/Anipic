#pragma once

#include "config-utils/shared/config-utils.hpp"

// Declare the mod config as "ModConfiguration" and declare all its values and functions.
DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(defaulty, std::string, "animals", "cat");

    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(defaulty);
    )
)