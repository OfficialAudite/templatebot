#include "mizu/command_handlers.h"

void RegisterSlashCommands(dpp::cluster& bot) {
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        auto commandName = event.command.get_command_name();

        if (CommandHandlers.find(commandName) != CommandHandlers.end()) {
            CommandHandlers.at(commandName)(event, bot);
        }
        else {
			bot.log(dpp::ll_error, "Unknown command: " + commandName);
        }
    });
}

void RegisterOnReadyHandler(dpp::cluster& bot) {
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {

            std::vector<dpp::slashcommand> commands;

            for (const auto& cmdInfo : AllCommands) {
                
				std::string commandBeingRegistered = "Command Registered: " + cmdInfo.name;
                bot.log(dpp::ll_info, commandBeingRegistered);
                
                commands.emplace_back(cmdInfo.name, cmdInfo.description, bot.me.id);
            }

            bot.global_bulk_command_create(commands);

			dpp::presence p = dpp::presence(dpp::presence_status::ps_dnd, dpp::activity_type::at_watching, "over the server");
           
			std::string registeredCommands = "Registered commands: " + std::to_string(commands.size());

			bot.log(dpp::ll_info, registeredCommands);
			bot.set_presence(dpp::presence(p));
        }
    });
}