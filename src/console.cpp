#include "mizu/console.h"
#include <iostream>

void ClearConsole() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    printf("\033[H\033[J");
#endif
}

void display_help(dpp::cluster& bot) {
    std::string help_msg =
        "\n"
        "------------------ Console Commands Help ------------------\n"
        "- shards    : Display the number of shards.\n"
        "- guilds    : Display the total number of guilds across all shards.\n"
        "- members   : Display the total number of members across all guilds.\n"
        "- ping      : Display the current ping to the Discord API.\n"
        "- presence  : Change Mizu current presence status.\n"
        "- status    : Display Mizu full stats.\n"
        "- help      : Show this help message.\n"
        "- clr       : Clear the console.\n"
        "- stop      : Stop the bot and exit the program.\n"
        "-----------------------------------------------------------";

    bot.log(dpp::ll_info, help_msg);
}

void console_thread(dpp::cluster& bot) {
    std::string input;

    struct ActivityInfo {
        dpp::activity_type activity;
        std::string name;
    };
    struct PresenceInfo {
        dpp::presence_status status;
        std::string name;
    };

    while (true) {
        std::getline(std::cin, input);
        if (input == "shards") {
            ClearConsole();
            bot.log(dpp::ll_info, "Shards: " + std::to_string(bot.numshards));
        }
        else if (input == "guilds") {
            ClearConsole();
            int total_guilds = 0;
            for (size_t shardId = 0; shardId < bot.numshards; ++shardId) {
                total_guilds += bot.get_shard(shardId)->get_guild_count();
            }
            bot.log(dpp::ll_info, "Total Guilds: " + std::to_string(total_guilds));
        }
        else if (input == "members") {
            ClearConsole();
            int total_members = 0;
            for (size_t shardId = 0; shardId < bot.numshards; ++shardId) {
                total_members += bot.get_shard(shardId)->get_member_count();
            }
            bot.log(dpp::ll_info, "Total Members: " + std::to_string(total_members));
        }
        else if (input == "help") {
            ClearConsole();
            display_help(bot);
        }
        else if (input == "ping") {
            ClearConsole();
            int latency = (int)(bot.rest_ping * 1000);
            bot.log(dpp::ll_info, "Discord API Latency: " + std::to_string(latency) + "ms");
        }
        else if (input == "presence") {
            std::map<int, PresenceInfo> statusMap = {
                {0, {dpp::ps_online, "Online"}},
                {1, {dpp::ps_dnd, "Do Not Disturb"}},
                {2, {dpp::ps_idle, "Idle"}},
                {3, {dpp::ps_invisible, "Invisible"}},
                {4, {dpp::ps_offline, "Offline"}}
            };

            std::map<int, ActivityInfo> activityMap = {
                {0, {dpp::at_game, "Playing .."}},
                {1, {dpp::at_streaming, "Streaming .."}},
                {2, {dpp::at_listening, "Listening to .."}},
                {3, {dpp::at_watching, "Watching .."}},
                {4, {dpp::at_competing, "Competing in .."}}
            };

            // Choose status
            ClearConsole();
            bot.log(dpp::ll_info, "Select a presence status:");
            for (const auto& pair : statusMap) {
                bot.log(dpp::ll_info, std::to_string(pair.first) + ". " + pair.second.name);
            }

            int statusChoice;
            std::cin >> statusChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the buffer

            // Choose activity
            bot.log(dpp::ll_info, "Select an activity type:");
            for (const auto& pair : activityMap) {
                bot.log(dpp::ll_info, std::to_string(pair.first) + ". " + pair.second.name);
            }

            int activityChoice;
            std::cin >> activityChoice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear the buffer

            if (statusMap.find(statusChoice) != statusMap.end() && activityMap.find(activityChoice) != activityMap.end()) {
                bot.log(dpp::ll_info, "Setting status to: " + statusMap[statusChoice].name + " and activity to: " + activityMap[activityChoice].name);

                std::string customPresence;
                bot.log(dpp::ll_info, "Enter custom presence text (leave blank for default):");
                std::getline(std::cin, customPresence);

                if (customPresence.empty()) {
                    customPresence = "over the server";  // Default presence
                }

                dpp::presence p = dpp::presence(statusMap[statusChoice].status, activityMap[activityChoice].activity, customPresence);
                bot.set_presence(p);
                ClearConsole();
                bot.log(dpp::ll_info, "Presence set!");
            }
            else {
                bot.log(dpp::ll_error, "Invalid choice(s). Status and activity not changed.");
            }
        }
        else if (input == "status") {
            int shards = bot.numshards;
            int clusters = bot.maxclusters;
            int latency = (int)(bot.rest_ping * 1000);
            int created = (int)bot.me.get_creation_time();
            int id = bot.me.id;
            std::string botusername = bot.me.username;
            int referencecount = bot.me.refcount;

            int total_members = 0;
            for (size_t shardId = 0; shardId < bot.numshards; ++shardId) {
                total_members += bot.get_shard(shardId)->get_member_count();
            }

            int total_guilds = 0;
            for (size_t shardId = 0; shardId < bot.numshards; ++shardId) {
                total_guilds += bot.get_shard(shardId)->get_guild_count();
            }

            ClearConsole();

            // Print header
            std::cout << "==================================" << std::endl;
            std::cout << "            BOT STATUS            " << std::endl;
            std::cout << "==================================\n" << std::endl;

            // Display bot information
            std::cout << "Bot Username        : " << botusername << std::endl;
            std::cout << "Bot ID              : " << id << std::endl;
            std::cout << "Creation Time       : " << created << std::endl;
            std::cout << "Reference Count     : " << referencecount << std::endl;
			std::cout << "Guild Count         : " << total_guilds << std::endl;
			std::cout << "Members Count       : " << total_members << std::endl;

            // Display other stats
            std::cout << "\n------ Performance Stats ------\n" << std::endl;
            std::cout << "Latency (ms)        : " << latency << std::endl;
            std::cout << "Number of Shards    : " << shards << std::endl;
            std::cout << "Number of Clusters  : " << clusters << std::endl;

            // Print footer
            std::cout << "\n==================================" << std::endl;
        }
        else if (input == "clr") {
            ClearConsole();
        }
        else if (input == "stop") {
            std::exit(0);
            break;
        }
    }
}