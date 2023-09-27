#pragma once
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <dpp/dpp.h>
#include "command_info.h"

const CommandInfo& getCatCommandInfo();
void HandleCatCommand(const dpp::slashcommand_t& event, dpp::cluster& bot);