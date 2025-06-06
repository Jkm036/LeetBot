#include<iostream>
#include<stack>
#include<string>
#include<jsoncpp/json/json.h>
#include<curl/curl.h>
#include"sleepy_discord/sleepy_discord.h"

struct HTTPResponse{
    std::string data;
};

class LeetBotClient : public SleepyDiscord::DiscordClient{
    public:
        using SleepyDiscord::DiscordClient::DiscordClient;
        void onError(SleepyDiscord::ErrorCode error,
                     const std::string errorMessage) override;
        void onReady(SleepyDiscord::Ready ready) override;
        void onMessage(SleepyDiscord::Message message) override;
    private:
        inline bool isInitiatorPhrase(std::string phrase){
            return (phrase == "lb" || phrase =="leetbot" );
        }
        void splitMessageContent(std::stack<std::string>& commandStack, 
                                 std::string content);
        void leetbotHandleCommand(std::string& content,
                                  const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&,
                                  const std::string& username);
        void leetbotHandleHelp(std::stack<std::string>& commandStack,
                                  const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void leetbotHandleDaily(std::stack<std::string>& commandStack,
                                  const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void leetbotHandleRandom(std::stack<std::string>& commandStack,
                                  const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void fetchLeetCodeRandom(const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void fetchLeetCodeDaily(const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void curlLeetCodePublicAPIAll(const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void parsePublicAPIRandomProblem(std::string& jsonResponse,
                                          const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        void leetbotDisplayProblem(const Json::Value& problem,
                                   const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
        std::string difficultyToString(unsigned int difficultyVal);
        void difficultyToEmoji(std::string& emoji, unsigned int difficulty);
        bool validateProblemsList(const Json::Value& root, 
                                  const std::string& problemsListMember);
    private:
        // Separation of curl specific commands
        void setPublicAPIHTTPHeader(struct curl_slist*& headers);
        void setPublicAPICurlOptions(CURL* curl,
                                     const std::string& url,
                                     HTTPResponse& response);
        void handleCurlReturnCode(CURLcode res,
                                  HTTPResponse& response,
                                  const SleepyDiscord::Snowflake<SleepyDiscord::Channel>&);
};
