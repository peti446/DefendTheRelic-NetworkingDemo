#include "GameScene.hpp"
#include "BulletEntity.hpp"
#include "PlayerEntity.hpp"

constexpr int m_AmountOfbullets = 200;

GameScene::GameScene(std::string t1_p1, std::string t1_p2, std::string t2_p1, std::string t2_p2, std::string gameID) : m_t1_p1(t1_p1), m_t1_p2(t1_p2), m_t2_p1(t2_p1), m_t2_p2(t2_p2)
                                                                                                                        , m_gameID(gameID)
{
    //ctor
}

GameScene::~GameScene()
{
    //dtor
}

void GameScene::Draw(sf::RenderWindow& rw)
{
    m_gui.draw();
    for(BulletEntity* e : m_Activebullets)
    {
        e->Draw(rw);
    }
    for(auto mit : m_players)
    {
        mit.second->Draw(rw);
    }
}

void GameScene::Update(const sf::Time& ur)
{
    for(BulletEntity* e : m_Activebullets)
    {
        e->Update(ur);
    }
    for(auto mit : m_players)
    {
        mit.second->Update(ur);
    }

}


void GameScene::HandleNetworkInput(NetMessage* msg)
{
    delete msg;
}

void GameScene::HandleInput(const sf::Event& event)
{
    m_gui.handleEvent(event);
    switch(event.type)
    {
    case sf::Event::KeyPressed:
        handlePlayerInput(event.key.code, true);
        break;
    case sf::Event::KeyReleased:
        handlePlayerInput(event.key.code, false);
        break;
    }
}

const std::string GameScene::Name() const
{
    return "Game Scene";
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

    if(m_t1_p1 != "NONE")
        m_players.insert(std::make_pair(m_t1_p1, (new PlayerEntity(Entity::eEntitySide::eTeam1, m_t1_p1,
                                                                   std::bind(&GameScene::InstanciateBullet, std::ref(*this), std::placeholders::_1, std::placeholders::_2)))));

    if(m_t1_p2 != "NONE")
        m_players.insert(std::make_pair(m_t1_p2, (new PlayerEntity(Entity::eEntitySide::eTeam1, m_t1_p2,
                                                                   std::bind(&GameScene::InstanciateBullet, this, std::placeholders::_1, std::placeholders::_2)))));

    if(m_t2_p1 != "NONE")
        m_players.insert(std::make_pair(m_t2_p1, (new PlayerEntity(Entity::eEntitySide::eTeam2, m_t2_p1,
                                                                   std::bind(&GameScene::InstanciateBullet, this, std::placeholders::_1, std::placeholders::_2)))));

    if(m_t2_p2 != "NONE")
        m_players.insert(std::make_pair(m_t2_p2, (new PlayerEntity(Entity::eEntitySide::eTeam2, m_t2_p2,
                                                                    std::bind(&GameScene::InstanciateBullet, this, std::placeholders::_1, std::placeholders::_2)))));

    if(m_players.find(GameEngine::Instance().getNetworkManager().getDisplayName()) != m_players.end())
    {
        PlayerEntity* p = m_players.at(GameEngine::Instance().getNetworkManager().getDisplayName());
        for(auto iter : m_players)
        {
            if(p == iter.second)
                continue;

            if(iter.second->is_ally_of(*p))
            {
                iter.second->setTexture(GameEngine::Instance().getTextureManager().getTexture("Media/Textures/Game/Ally.png"));
            } else
            {
                iter.second->setTexture(GameEngine::Instance().getTextureManager().getTexture("Media/Textures/Game/Enemy.png"));
            }
        }
    }

    return true;
}

bool GameScene::UnloadScene()
{
    return true;
}

bool GameScene::InstanciateBullet(PlayerEntity& whoIsShooting, float speed)
{
    if(m_bullets.size() > 0)
    {
        BulletEntity* b = m_bullets.back();
        m_bullets.pop_back();
        b->Instansiate(whoIsShooting, speed);
        m_Activebullets.push_back(b);
        return true;
    }
    return false;
}

void GameScene::handlePlayerInput(sf::Keyboard::Key key, bool pressed)
{
    PlayerEntity* p = m_players.at(GameEngine::Instance().getNetworkManager().getDisplayName());
    switch(key)
    {
        case sf::Keyboard::W:
            p->setDirection(Entity::eEntityDirection::eNorth);
            p->setPlayerStatus(pressed ? PlayerEntity::ePlayerState::eWalking : PlayerEntity::ePlayerState::eIDLE);
            break;
        case sf::Keyboard::S:
            p->setDirection(Entity::eEntityDirection::eSorth);
            p->setPlayerStatus(pressed ? PlayerEntity::ePlayerState::eWalking : PlayerEntity::ePlayerState::eIDLE);
            break;
        case sf::Keyboard::D:
            p->setDirection(Entity::eEntityDirection::eEast);
            p->setPlayerStatus(pressed ? PlayerEntity::ePlayerState::eWalking : PlayerEntity::ePlayerState::eIDLE);
            break;
        case sf::Keyboard::A:
            p->setDirection(Entity::eEntityDirection::eWest);
            p->setPlayerStatus(pressed ? PlayerEntity::ePlayerState::eWalking : PlayerEntity::ePlayerState::eIDLE);
            break;
    }
}


