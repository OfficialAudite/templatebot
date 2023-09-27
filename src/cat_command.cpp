#include "mizu/cat_command.h"
#include <iostream>
#include <cpr/cpr.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

const CommandInfo& getCatCommandInfo() {
    static CommandInfo cmdInfo = { "cat", "Sends awesome cat picture", FUN };
    return cmdInfo;
}

void HandleCatCommand(const dpp::slashcommand_t& event, dpp::cluster& bot) {
    auto response = cpr::Get(cpr::Url{"https://api.thecatapi.com/v1/images/search"});

    rapidjson::Document document;
    document.Parse(response.text.c_str());

    if (document.IsArray() && document.Size() > 0 && document[0].HasMember("url")) {
        std::string catImageUrl = document[0]["url"].GetString();

        dpp::embed e = dpp::embed()
            .set_title("Here's your cat!")
            .set_image(catImageUrl)
            .set_color(0xc756c7);

        dpp::message msg(event.command.channel_id, e);
        event.reply(msg);
    }
    else {
		bot.log(dpp::ll_error, "Failed to parse cat image URL from API response");
        dpp::message msg(event.command.channel_id, "Sorry, couldn't fetch a cat image right now. :(");
        event.reply(msg);
    }
}