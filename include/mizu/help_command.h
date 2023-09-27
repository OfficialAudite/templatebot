#pragma once
#include <dpp/dpp.h>
#include "command_info.h"

const CommandInfo& getHelpCommandInfo();
void HandleHelpCommand(const dpp::slashcommand_t& event, dpp::cluster& bot);