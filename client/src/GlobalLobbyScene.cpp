#include "GlobalLobbyScene.hpp"

GlobalLobbyScene::GlobalLobbyScene()
{
    //ctor
}

GlobalLobbyScene::~GlobalLobbyScene()
{
    //dtor
}

void GlobalLobbyScene::Draw(sf::RenderWindow& rw)
{
}

void GlobalLobbyScene::Update(const sf::Time& ur)
{

}

void GlobalLobbyScene::HandleInput(const sf::Event& event)
{

}

const std::string GlobalLobbyScene::Name() const
{
    return "Main Lobby";
}

bool GlobalLobbyScene::LoadScene()
{
    return true;
}

bool GlobalLobbyScene::UnloadScene()
{
    return true;
}
