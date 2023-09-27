#include "mizu/command_template.h"

const CommandInfo TemplateCommandInfo = {
    "template",    // command name
    "Description of the template command"  // command description
    // Add more attributes as needed
};

void HandleTemplateCommand(const dpp::slashcommand_t& event, dpp::cluster& bot) {
    // Command implementation goes here
}
