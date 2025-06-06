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

//Callback function called by CURL on request to endpoints
size_t WriteCallback(void*contents, 
        size_t contentSize,
        size_t numContents,
        HTTPResponse* response){
    // We are going to process ALL of the data
    size_t dataProcessedSize = (contentSize*numContents);

    // Pull in all the information into our
    // HTTPResponse object
    response->data.append(static_cast<char*>(contents), dataProcessedSize);
    return dataProcessedSize;
}

void LeetBotClient::onError(SleepyDiscord::ErrorCode error,
                            const std::string errorMessage){
    std::cout << "LeetBot Client error: "<< errorMessage << std::endl;
    std::cout << "LeetBot Client error code : "<< error << std::endl;

    // Gotta dig more into what and SSL error even is....
    if(error == 5002 ){
        std::cout<< " Error code of 5002 may indicate SSL error..." << std::endl;
    }

}

void LeetBotClient::onReady(SleepyDiscord::Ready ready){
    std::cout << "Bot is ready and connected! " << std::endl;
    std::cout << "Bot name : " 
        << ready.user.username
        << std::endl;
}


void LeetBotClient::onMessage(SleepyDiscord::Message message){
    // Don't responde to messages from bots
    if(message.author.bot) return;

    if(message.startsWith("leetbot") || message.startsWith("lb")){
        // Collect the author information of the person 
        // who incited leetbot
        SleepyDiscord::User  author = message.author;

        // Print user info
        std::cout << " Received Message from " 
            <<  (author.username.size() ? author.username :"<username not specified>")
            << std::endl;
        std::cout << "message content -> " << message.content << std::endl;

        // Handle the Leetbot Commadn supplied by user
        leetbotHandleCommand(message.content, 
                message.channelID,
                author.username);
    }

}

void LeetBotClient::splitMessageContent(std::stack<std::string>& commandStack,
        std::string content){
    std::stringstream ss(content);
    std::vector<std::string> wordBuffer;
    std::string word;

    // Divide message contents into words and 
    // store them in our word buffer
    while(ss >> word)
        wordBuffer.push_back(word);

    // Iterate through our word buffer and
    // push elements  onto stack backwords
    for(auto it = wordBuffer.rbegin();
            it!= wordBuffer.rend();
            ++it){
        commandStack.push(*it);
    }
}

void LeetBotClient::leetbotHandleCommand(std::string& content,
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID,
        const std::string& username){
            std::string commandPhrase;
            std::stack<std::string> commandStack;
            std::string lowerContent = content;
            // Turn command into all lowercase
            std::transform(lowerContent.begin(),
                           lowerContent.end(),
                           lowerContent.begin(),
                           ::tolower);
            splitMessageContent(commandStack, lowerContent);
            if(commandStack.empty() || !isInitiatorPhrase(commandStack.top()))
                return;

            // Remove the initiator phrase and
            // get the first command word from the
            // command stack
            commandStack.pop();
            if(commandStack.empty()){
                sendMessage(channelID, "Hey... what's up " + username);
                return;
            }

            commandPhrase = commandStack.top();
            // Read command and branch control to functions
            // meant to handle specific commands
            if(commandPhrase == "help"){
                commandStack.pop();
                leetbotHandleHelp(commandStack, channelID);
            }else if(commandPhrase == "daily" || commandPhrase == "today" || commandPhrase == "problem" ){
                leetbotHandleDaily(commandStack, channelID);
            }else if(commandPhrase == "random"){
                leetbotHandleRandom(commandStack, channelID);
            }else{
                sendMessage(channelID, "Sorry, I don't know what \"" + commandStack.top() + "\" means");
                sendMessage(channelID, "Maybe ask Josh to implement whatever you're asking for...");
            }
}

void LeetBotClient::leetbotHandleHelp(std::stack<std::string>& commandStack, 
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    std::string helpMessage = "🤖 **LeetBot Commands** 🤖\n\n";
    helpMessage += "📅 `leetbot (daily | problem | today)` - Get today's daily coding challenge\n";
    helpMessage += "🎲 `leetbot random` - Roll the die and get a random problem to complete\n";
    helpMessage += "❓ `leetbot help` - Show this help message\n\n";
    helpMessage += "You can also use `lb` instead of `leetbot` for shorter commands!\n";
    helpMessage += "Example: `lb random` 🚀";
    sendMessage(channelID, helpMessage);
 }

void LeetBotClient::leetbotHandleDaily(std::stack<std::string>& commandStack,
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    sendMessage(channelID, "Leetcode problem of the day!");
    fetchLeetCodeDaily(channelID);
}

void LeetBotClient::leetbotHandleRandom(std::stack<std::string>& commandStack,
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    sendMessage(channelID, "🎲 You rolled the dice...");
    fetchLeetCodeRandom(channelID);
}

void LeetBotClient::fetchLeetCodeRandom(const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    // Initiate Curl command for public API
    curlLeetCodePublicAPIAll(channelID);
}

void LeetBotClient::fetchLeetCodeDaily(const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    sendMessage(channelID, "Daily problem is under development");
}

void LeetBotClient::curlLeetCodePublicAPIAll(const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    static const  std::string url = "https://leetcode.com/api/problems/all/";
    CURL* curl;
    CURLcode res;
    HTTPResponse response;

    curl = curl_easy_init();
    if(curl){
        struct curl_slist*  headers = nullptr;
        // Set HTTP header information 
        // fro the request were are going to make to leetcode
        setPublicAPIHTTPHeader(headers); 
        // Set this header information in the
        // http client we are using
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // Set behavior of the http client we are using
        setPublicAPICurlOptions(curl, url, response);
        // Perform curl command
        res = curl_easy_perform(curl);

        //Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        // Call parser based on received response
        handleCurlReturnCode(res, response, channelID); 
    }else{
        sendMessage(channelID,"❌ Failed to initialize HTTP client (curl)");
    }
}

void LeetBotClient::parsePublicAPIRandomProblem(std::string& jsonResponse,
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    static const  std::string problemsListMember = "stat_status_pairs";
    Json::Value root;
    Json::Reader reader;

    std::cout 
        << "Parsing public LeetCode's API json response for random selection" 
        << std::endl;

    // Parse Response
    if(!reader.parse(jsonResponse, root)){
        std::cout 
            << "Failed to parse jsonResponse" 
            << std::endl;
        return;
    }

    // Make sure we actually
    // got a list of problems from 
    // leetcode.com
    if(!validateProblemsList(root,
                problemsListMember))
        return;

    // List of problems
    const Json::Value& problemsList = root[problemsListMember];

    // Randomize seed for random function
    // And pull random problem
    std::srand(std::time(nullptr));
    int randomIndex = std::rand() % problemsList.size();
    const Json::Value randomProblem = problemsList[randomIndex];

    // Now that we have the problem, display it
    leetbotDisplayProblem(randomProblem, channelID);
}

void LeetBotClient::leetbotDisplayProblem(const Json::Value& problem,
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    const Json::Value&  stat = problem["stat"];
    std::string dEmoji = "";
    std::stringstream leetbotResponse;

    // Extracting problem stat information
    const Json::Value&  difficulty = problem.get("difficulty", Json::Value());
    const std::string title =  stat.get("question__title", "Unknown Problem").asString();
    const std::string title_slug = stat.get("question__title_slug", "Unknown Problem").asString();
    const int totalAccepted = stat.get("total_acs", 0).asInt();
    const int totalSubmitted = stat.get("total_submitted", 0).asInt();
    bool isPaidOnly = stat.get("paid_only", false).asBool();

    // Extact other problem information 
    // Acceptance rate of the problem
    double acRate = 0.0;
    if(totalSubmitted)
        acRate = (static_cast<double>(totalAccepted)/totalSubmitted) * 100.0;

    const unsigned int difficultyVal = problem.get("difficulty", Json::Value()).get("level",0).asInt();
    std::string difficultyString  = difficultyToString(difficultyVal);
    difficultyToEmoji(dEmoji, difficultyVal);
    const std::string problemURL = "https://leetcode.com/problems/"+title_slug+"/";

    //  Displaying extracted values
    //  Build message for user 
    std::cout  
        << "Problem selected: " 
        << title 
        << std::endl;
    leetbotResponse << "**" << title << "**"<< std::endl
        << "Acceptance rate -> " << std::to_string(static_cast<int>(acRate)) << std::endl
        << "Difficulty -> " << difficultyString << dEmoji << std::endl;
    if(acRate < 50)
        leetbotResponse << "⚠️  Low Acceptance rate! Be careful with this question! "<< std::endl;
    leetbotResponse << "Problem URL -> " << problemURL << std::endl;
    sendMessage(channelID, leetbotResponse.str());
}

std::string LeetBotClient::difficultyToString(unsigned int difficultyVal){
    switch(difficultyVal){
        case 1:
            return "Easy";
            break;
        case 2:
            return "Medium";
            break;
        case 3:
            return "Hard";
            break;
        default:
            return "Unknown";
    }
}

void LeetBotClient::difficultyToEmoji(std::string& emoji,
        unsigned int difficulty){
            switch(difficulty){
                case 1:
                    emoji = "🟢";
                    break;
                case 2:
                    emoji = "🟡";
                    break;
                case 3:
                    emoji = "🔴";
                    break;
                default:
                    emoji= "❔";
            }
}

bool LeetBotClient::validateProblemsList(const Json::Value& root, 
        const std::string& problemsListMember){
    bool ret = true;
    if(!root.isMember(problemsListMember)
            || !root[problemsListMember].isArray()
            || !root[problemsListMember].size()){
        ret = false;
        std::cout 
            << "Failed to receive valid list of problems from Leetcode's Json Response"
            << std::endl;
    }
    return ret;
}

void LeetBotClient::setPublicAPIHTTPHeader(struct curl_slist*& headers){
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");
    headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.9");
    headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br");
    headers = curl_slist_append(headers, "DNT: 1");
    headers = curl_slist_append(headers, "Connection: keep-alive");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests: 1");
}

void LeetBotClient::setPublicAPICurlOptions(CURL* curl, 
        const std::string& url, 
        HTTPResponse& response){
    //----
    // curl client configuration
    //----
    // Where to write information from curl
    // instead of printing
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    // Follow HTTP redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    // Timeout of request
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    //Secure socket layer
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L );
    curl_easy_setopt(curl, CURLOPT_ENCODING, "");
}


void LeetBotClient::handleCurlReturnCode(CURLcode res,
        HTTPResponse& response, 
        const SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID){
    if(res == CURLE_OK){
        std::cout << "Successfully fetched leetcode data " << std::endl;
        // We should do some parsing now
        std::cout << response.data.c_str() << std::endl;
        // Log output server-side
        // TODO: I need to differentiate between parsers for the different enpoints
        // I can use here. For now just implementing the public LeetCode "all problems"
        // API for random problems
        parsePublicAPIRandomProblem(response.data , channelID);
    } else{
        std::cout << "CURL error: " << curl_easy_strerror(res) << std::endl;
        sendMessage(channelID, "❌ Failed to fetch LeetCode problem. Please try again later.\n" +
                std::string("Error: ") + curl_easy_strerror(res));
    }
}
