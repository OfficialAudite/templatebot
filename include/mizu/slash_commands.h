#pragma once
#include <dpp/dpp.h>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>

void RegisterSlashCommands(dpp::cluster& bot);
void RegisterOnReadyHandler(dpp::cluster& bot);