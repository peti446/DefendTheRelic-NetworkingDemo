#include "GameScene.hpp"
#include "BulletEntity.hpp"
#include "PlayerEntity.hpp"

constexpr int m_AmountOfbullets = 200;

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
    Log(l_DEBUG) << "Loading bullets";
    sf::Texture& t = GameEngine::Instance().getTextureManager().getTexture("Media/Textures/Game/Bullet.png");
    m_bullets.reserve(m_AmountOfbullets);
    for(size_t i = 0; i < m_bullets.capacity(); i++)
    {
        m_bullets.push_back(new BulletEntity(t));
    }

    Log() << "Loading Players";

    return true;
}

bool GameScene::UnloadScene()
{
    return true;
}
