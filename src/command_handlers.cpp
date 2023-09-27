#include "mizu/command_handlers.h"
#include "mizu/ping_command.h"
#include "mizu/help_command.h"
#include "mizu/cat_command.h"

const std::map<std::string, CommandHandler> CommandHandlers = {
    {"ping", HandlePingCommand},
    {"help", HandleHelpCommand},
	{"cat", HandleCatCommand}
};

const std::vector<CommandInfo> AllCommands = {
    getPingCommandInfo(),
    getHelpCommandInfo(),
	getCatCommandInfo()
};