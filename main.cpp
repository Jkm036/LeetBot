#include<iostream>
#include<fstream>
#include<string>
#include"sleepy_discord/sleepy_discord.h"
#include<sstream>
#include<vector>
#include<stack>
#include<algorithm>
#include<curl/curl.h>
#include<jsoncpp/json/json.h>
#include<random>
#include<ctime>
#include"LeetBotClient.hpp"

// Global declaration of the token used for the bot
std::string token = "";

// Function to get token from bot_token.txt file
bool readToken(std::string filePath = "./tokens/"){
    std::string fileName = "bot_token.txt";
    std::ifstream configFile(filePath + fileName);
    std::string line;
    std::string valueName = "DISCORD_TOKEN=";
    if(!configFile.is_open()){
        std::cout << "Error opening config file "
                  << filePath 
                  << fileName
                  << std::endl;
        // Closing for good practice
        configFile.close();
        return false;
    }

    // READ contents of the bot config file
    while(std::getline(configFile, line)){
            if(line.find(valueName) == 0  ){
                token = line.substr(valueName.size());
                configFile.close();
                return true;
            }
    }

    // We didn't find what we were looking for
    // here
    std::cout << "Couldn't find " 
              << valueName 
              << " in "
              << filePath << fileName
              << std::endl;
    configFile.close();
    return false;
}


// Start client 
int main(){
    std::cout << "Starting leetbot..." <<std::endl;
    // Get the bot's token information
    if(!readToken()){
        std::cout << "There was an error reading the token file " 
                 << std::endl;
        return -1;
    }

    // Got the token we were looking for
    std::cout << "Successfully extracted token " << std::endl;

    // Create discord client object
    LeetBotClient client(token , SleepyDiscord::USER_CONTROLED_THREADS);
    client.setIntents(SleepyDiscord::Intent::SERVER_MESSAGES,
                      SleepyDiscord::Intent::MESSAGE_CONTENT);
    // Run the discord client
    client.run();
    return 0;
}
