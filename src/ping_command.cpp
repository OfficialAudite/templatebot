#include <sstream>
#include <iomanip>
#include "mizu/ping_command.h"
#include "mizu/Database.h"

const CommandInfo& getPingCommandInfo() {
    static CommandInfo cmdInfo = { "ping", "Ping pong!" };
    return cmdInfo;
}

void HandlePingCommand(const dpp::slashcommand_t& event, dpp::cluster& bot) {
    int latency = (int)(bot.rest_ping * 1000);
	std::string description = (latency <= 500) ? ":ping_pong: These figures are within expected ranges!" : ":warning: These figures are outside of expected ranges!";
    description += "\n\n:green_circle: Up :red_circle: Down :yellow_circle: Maintenance\n";
	const char* emojis[3] = { ":green_circle:", ":red_circle:", ":yellow_circle:"};
	std::string currentShard = "`Mizu " + std::to_string(event.from->shard_id) + "`";

    Database* db = Database::getInstance();
    double dbPing = db->returnPing();
    dbPing = std::round(dbPing * 1000.0) / 1000.0;

    std::stringstream ss;
    ss << std::fixed << std::setprecision(3) << dbPing;

    dpp::embed e = dpp::embed()
        .set_title("Ping!")
        .set_color(0xc756c7)
        .set_description(description)
        .add_field("Shard Information", "`Shard` `Guilds/Members`", false)
        .add_field("Shard used for this guild", currentShard, false)
        .add_field("Discord API", std::to_string(latency) + "ms", false)
        .add_field("Database", ss.str() + "ms", false);
    
    int shardsPerCluster = 8;
    int activeClusters = bot.maxclusters;

    for (int i = 0; i < activeClusters; i++) {
        std::string clusterInfo = "";
        for (int x = 0; x < shardsPerCluster; x++) {
            int shardId = (i * shardsPerCluster) + x;
            if (shardId < bot.numshards) {
                auto shard = bot.get_shard(shardId);
                bool isShardConnected;
                int members;
                int guildCount;
                if (shard) {
                    isShardConnected = shard->is_connected();
					members = shard->get_member_count();
					guildCount = shard->get_guild_count();
                }
                else {
                    isShardConnected = false;
                    members = 0;
                    guildCount = 0;
                }
				std::string shardData = "`" + std::to_string(guildCount) + "/" + std::to_string(members) + "` ";
                std::string shardStatus = isShardConnected ? emojis[0] : emojis[1];
                clusterInfo += "`Mizu " + std::to_string(shardId) + "` " + shardData + shardStatus + "\n";
            }
        }
        // Add the cluster string as a field to the embed
        e.add_field("Cluster " + std::to_string(i), clusterInfo, true);
    }

    dpp::message msg(event.command.channel_id, e);
    event.reply(msg);
}