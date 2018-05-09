#include "GameScene.hpp"

GameScene::GameScene()
{
    //ctor
}

GameScene::~GameScene()
{
    //dtor
}

void GameScene::Draw(sf::RenderWindow& rw)
{
}

void GameScene::Update(const sf::Time& ur)
{

}


void GameScene::HandleNetworkInput(NetMessage* msg)
{

    delete msg;
}

void GameScene::HandleInput(const sf::Event& event)
{

}

const std::string GameScene::Name() const
{
    return "Game";
}

bool GameScene::LoadScene()
{
    return true;
}

bool GameScene::UnloadScene()
{
    return true;
}
