#include "mizu/database.h"
#include <mysql/mysql.h>
#include <dpp/nlohmann/json.hpp>

Database* Database::instance = nullptr;

using json = nlohmann::json;

Database* Database::getInstance() {
    json configdocument;
    std::ifstream configfile("../config.json");
    configfile >> configdocument;

    if (instance == nullptr) {
        instance = new Database(configdocument["db_host"], configdocument["db_username"], configdocument["db_password"], configdocument["db_database"]);
    }
    return instance;
}

Database::Database(const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
    con = mysql_init(NULL);
    if (mysql_real_connect(con, host.c_str(), user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0)) {
        std::cout << "Database connection established" << std::endl;
    } else {
        std::cout << "Failed to connect to database: " << mysql_error(con) << std::endl;
    }
}

Database::~Database() {
    mysql_close(con);
}

void Database::initializeTables() {
    const char* query1 = "CREATE TABLE IF NOT EXISTS Guilds (GuildID BIGINT PRIMARY KEY, GuildName VARCHAR(255) NOT NULL)";
    if (mysql_query(con, query1)) {
        std::cerr << "Error creating table Guilds: " << mysql_error(con) << std::endl;
    }

    const char* query2 = "CREATE TABLE IF NOT EXISTS Users (UserID BIGINT, Username VARCHAR(255) NOT NULL, XP INT DEFAULT 0, Money INT DEFAULT 0, Level INT DEFAULT 1, GuildID BIGINT, PRIMARY KEY(UserID, GuildID), FOREIGN KEY (GuildID) REFERENCES Guilds(GuildID))";
    if (mysql_query(con, query2)) {
        std::cerr << "Error creating table Users: " << mysql_error(con) << std::endl;
    }
}

bool Database::insertGuild(int64_t guildID, const std::string& guildName) {
    std::string query = "INSERT INTO Guilds (GuildID, GuildName) VALUES (" + std::to_string(guildID) + ", '" + guildName + "')";
    if (mysql_query(con, query.c_str())) {
        std::cerr << "INSERT Guild Error: " << mysql_error(con) << std::endl;
        return false;
    }
    return true;
}

bool Database::updateGuild(int64_t guildID, const std::string& guildName) {
    std::string query = "UPDATE Guilds SET GuildName='" + guildName + "' WHERE GuildID=" + std::to_string(guildID);
    if (mysql_query(con, query.c_str())) {
        std::cerr << "UPDATE Guild Error: " << mysql_error(con) << std::endl;
        return false;
    }
    return true;
}

bool Database::insertUser(int64_t userID, const std::string& username, int xp, int money, int level, int64_t guildID) {
    std::string query = "INSERT INTO Users (UserID, Username, XP, Money, Level, GuildID) VALUES (" + std::to_string(userID) + ", '" + username + "', " + std::to_string(xp) + ", " + std::to_string(money) + ", " + std::to_string(level) + ", " + std::to_string(guildID) + ")";
    if (mysql_query(con, query.c_str())) {
        std::cerr << "INSERT User Error: " << mysql_error(con) << std::endl;
        return false;
    }
    return true;
}

bool Database::updateUser(int64_t userID, const std::string& username, int xp, int money, int level, int64_t guildID) {
    std::string query = "UPDATE Users SET Username='" + username + "', XP=" + std::to_string(xp) + ", Money=" + std::to_string(money) + ", Level=" + std::to_string(level) + " WHERE UserID=" + std::to_string(userID) + " AND GuildID=" + std::to_string(guildID);
    if (mysql_query(con, query.c_str())) {
        std::cerr << "UPDATE User Error: " << mysql_error(con) << std::endl;
        return false;
    }
    return true;
}

Database::GuildData Database::getGuildData(int64_t guildID) {
    std::string query = "SELECT * FROM Guilds WHERE GuildID = " + std::to_string(guildID);
    if (mysql_query(con, query.c_str())) {
        std::cerr << "SELECT Guild Error: " << mysql_error(con) << std::endl;
        return GuildData{};
    }
    
    MYSQL_RES* result = mysql_store_result(con);
    MYSQL_ROW row;

    GuildData data;
    row = mysql_fetch_row(result);
    if (row) {
        data.GuildID = std::stoll(row[0]);
        data.GuildName = row[1];
        data.isValid = true;
    }
    
    mysql_free_result(result); // Always free the result after using!
    
    return data;
}

Database::UserData Database::getUserData(int64_t userID, int64_t guildID) {
    std::string query = "SELECT * FROM Users WHERE UserID = " + std::to_string(userID) + " AND GuildID = " + std::to_string(guildID);
    if (mysql_query(con, query.c_str())) {
        std::cerr << "SELECT User Error: " << mysql_error(con) << std::endl;
        return UserData{};
    }
    
    MYSQL_RES* result = mysql_store_result(con);
    MYSQL_ROW row;

    UserData data;
    row = mysql_fetch_row(result);
    if (row) {
        data.UserID = std::stoll(row[0]);
        data.Username = row[1];
        data.XP = std::stoi(row[2]);
        data.Money = std::stoi(row[3]);
        data.Level = std::stoi(row[4]);
        data.GuildID = std::stoll(row[5]);
        data.isValid = true;
    }
    
    mysql_free_result(result); // Always free the result after using!
    
    return data;
}

// This function is not fully fleshed out as I do not have the definitions for dpp::message_create_t and dpp::cluster.
void Database::initUser(const dpp::message_create_t& event, dpp::cluster& bot) {
    int64_t guildId = event.msg.guild_id;
    int64_t userId = event.msg.author.id;
    std::string username = event.msg.author.username;

    // Check if the guild is already in the database
    GuildData guildData = this->getGuildData(guildId);
    if (!guildData.isValid) {
        // If it's not, try to find it using dpp
        dpp::guild* g = dpp::find_guild(guildId);
        if (g) {
            std::string guildName = g->name;
            // If found, insert it into the database
            this->insertGuild(guildId, guildName);
        } else {
            // If not found, log an error
            bot.log(dpp::ll_error, "Guild not found");
        }
    }

    // Check if the user is already in the database
    UserData user = this->getUserData(userId, guildId);
    if (!user.isValid) {
        // If not, insert the user with initial values
        this->insertUser(userId, username, 0, 0, 1, guildId);
    }
}

double Database::returnPing() {
    auto start = std::chrono::high_resolution_clock::now();
    if (mysql_ping(con)) {
        return -1.0; // If there's an error in pinging
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    return diff.count() * 1000.0; // Convert to milliseconds
}
