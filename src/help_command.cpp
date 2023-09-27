#include "mizu/help_command.h"
#include "mizu/command_handlers.h"

const CommandInfo& getHelpCommandInfo() {
    static CommandInfo cmdInfo = { "help", "Mizu helps with all commands" };
    return cmdInfo;
}

void HandleHelpCommand(const dpp::slashcommand_t& event, dpp::cluster& bot) {
    dpp::embed e;
    e.set_title("Commands")
        .set_color(0xc756c7)
        .set_thumbnail(bot.me.get_avatar_url());

    // Map to hold commands categorized by their category
    std::map<CommandCategory, std::string> categorizedCommands;

    for (const auto& cmdInfo : AllCommands) {
        std::string& categoryCommands = categorizedCommands[cmdInfo.category];

        if (!categoryCommands.empty()) {
            categoryCommands += "\n";  // Newline separate commands within same category
        }

        // Append the command to its category
        categoryCommands += "`/" + cmdInfo.name + "` - " + cmdInfo.description;
    }

    // Now, add each category as an embed field
    for (const auto& [category, commands] : categorizedCommands) {
        e.add_field(categoryMap[category], commands, true);  // assuming categoryMap is accessible
    }

    dpp::message msg(event.command.channel_id, e);
    event.reply(msg);
}