#include "Logger.hpp"
#include "GameEngine.hpp"

int main()
{
    Log(eLogType::l_INFO) << "Starting Program";
    GameEngine::Instance().run();
    Log(eLogType::l_INFO) << "Exiting program";
    return 0;
}
