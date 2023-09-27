#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>

#include "mizu/console.h"
#include "mizu/database.h"
#include "mizu/slash_commands.h"      // If these files contain relevant logic.
#include "mizu/command_handlers.h"    // If these files contain relevant logic.

#include <dpp/dpp.h>
#include <dpp/utility.h>
#include <dpp/dispatcher.h>

#include <sstream>
#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;

class MyBot {
private:
    dpp::cluster bot;

public:
    MyBot(const std::string& token)
        : bot(token, dpp::i_default_intents | dpp::i_message_content) {

        bot.on_log(dpp::utility::cout_logger());

        // Registering slash command handlers
        RegisterSlashCommands(bot);

        // Register the on_ready handler
        RegisterOnReadyHandler(bot);

        bot.on_message_create([&](const dpp::message_create_t& event) {
            Database::getInstance()->initUser(event, bot); // Use getInstance() to access the Database
        });
    }

    void start() {
        bot.start(dpp::st_wait);
    }

    dpp::cluster& getBot() {
        return bot;
    }
};

int main() {
    json configdocument;
    std::ifstream configfile("../config.json");
    configfile >> configdocument;

    MyBot bot(configdocument["token"]);

    Database::getInstance()->initializeTables(); // Use getInstance() to access the Database

    std::thread consoleInputThread(console_thread, std::ref(bot.getBot()));
    consoleInputThread.detach();

    bot.start();

    std::cout << "done" << std::endl;

    return 0;
}
