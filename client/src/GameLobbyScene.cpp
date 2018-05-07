#include "GameLobbyScene.hpp"

GameLobbyScene::GameLobbyScene()
{
    //ctor
}

GameLobbyScene::~GameLobbyScene()
{
    //dtor
}


void GameLobbyScene::Draw(sf::RenderWindow& rw)
{
}

void GameLobbyScene::Update(const sf::Time& ur)
{

}

void GameLobbyScene::HandleInput(const sf::Event& event)
{

}

const std::string GameLobbyScene::Name() const
{
    return "Game Lobby";
}

bool GameLobbyScene::LoadScene()
{
    return true;
}

bool GameLobbyScene::UnloadScene()
{
    return true;
}
