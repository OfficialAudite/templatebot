#pragma once
#include "command_info.h"
#include <dpp/dpp.h>

// Command Info
extern const CommandInfo TemplateCommandInfo;

// Command Handler
void HandleTemplateCommand(const dpp::slashcommand_t& event, dpp::cluster& bot);
