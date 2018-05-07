#include "Logger.hpp"
#include "GameEngine.hpp"

int main()
{
    //Change this value to control what type of messages will be printed to the console (Debug will print everything, including messages that are only used to debug (get the current game state))
    Logger::Instance().SetLogerLevel(l_DEBUG);
    //Start the game
    Log(eLogType::l_INFO) << "Starting Program";
    GameEngine::Instance().run();
    Log(eLogType::l_INFO) << "Exiting program";
    return 0;
}
