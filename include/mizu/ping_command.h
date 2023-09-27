#pragma once
#include <dpp/dpp.h>
#include "command_info.h"

const CommandInfo& getPingCommandInfo();
void HandlePingCommand(const dpp::slashcommand_t& event, dpp::cluster& bot);