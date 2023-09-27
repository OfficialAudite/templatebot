#pragma once
#include <dpp/dpp.h>

enum CommandCategory {
    GENERAL,
    ADMIN,
    FUN,
};

inline std::map<CommandCategory, std::string> categoryMap = {
    { GENERAL, "General Commands" },
    { ADMIN, "Admin Commands" },
    { FUN, "Fun Commands" },
};

struct CommandInfo {
    std::string name;
    std::string description;
	CommandCategory category = GENERAL;
};