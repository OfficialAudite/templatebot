#include <dpp/dpp.h>
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <chrono>

class Database {
public:
    struct GuildData {
        int64_t GuildID;
        std::string GuildName;
        bool isValid = false;
    };

    struct UserData {
        int64_t UserID;
        int64_t GuildID;
        std::string Username;
        int XP;
        int Money;
        int Level;
        bool isValid = false;
    };

    static Database* getInstance();
    ~Database();

    void initializeTables();
    bool insertGuild(int64_t guildID, const std::string& guildName);
    bool updateGuild(int64_t guildID, const std::string& guildName);
    bool insertUser(int64_t userID, const std::string& username, int xp, int money, int level, int64_t guildID);
    bool updateUser(int64_t userID, const std::string& username, int xp, int money, int level, int64_t guildID);
    GuildData getGuildData(int64_t guildID);
    UserData getUserData(int64_t userID, int64_t guildID);
    void initUser(const dpp::message_create_t& event, dpp::cluster& bot);
    double returnPing();

private:
    static Database* instance;
    MYSQL *con; // Instead of mysqlpp::Connection* con;

    Database(const std::string& host, const std::string& user, const std::string& password, const std::string& database);
};
