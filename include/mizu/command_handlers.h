#pragma once
#include "command_info.h"
#include <dpp/dpp.h>
#include <map>
#include <functional>
#include <vector>

// Define the type for command handlers
using CommandHandler = std::function<void(const dpp::slashcommand_t&, dpp::cluster&)>;
extern const std::map<std::string, CommandHandler>CommandHandlers;

// Declare the vector that will contain all commands
extern const std::vector<CommandInfo> AllCommands;