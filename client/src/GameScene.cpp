#include "GameScene.hpp"
#include "BulletEntity.hpp"
#include "PlayerEntity.hpp"
#include "PlayerStatusUpdateNetMessage.hpp"
#include "BulletInstanciateNetMessage.hpp"
#include "HPPlayerUpdateNetMessage.hpp"
#include "PlayerBulletCountUpdateNetMessage.hpp"
#include "PlayerDieNetMessage.hpp"
#include "PlayerRespawnNetMessage.hpp"
#include "PlayerExitMatch.hpp"

constexpr int m_AmountOfbullets = 200;

GameScene::GameScene(std::string t1_p1, std::string t1_p2, std::string t2_p1, std::string t2_p2, std::string gameID) : m_t1_p1(t1_p1), m_t1_p2(t1_p2), m_t2_p1(t2_p1), m_t2_p2(t2_p2)
                                                                                                                        , m_gameID(gameID)
{
    //ctor
}

GameScene::~GameScene()
{
    for(BulletEntity* e : m_Activebullets)
    {
        delete e;
    }
    for(BulletEntity* e : m_bullets)
    {
        delete e;
    }
    for(auto mit : m_players)
    {
        delete mit.second;
    }

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
    //Update all the active bullets
    for(BulletEntity* e : m_Activebullets)
    {
        e->Update(ur);
    }

    //Update all players
    for(auto mit : m_players)
    {
        mit.second->Update(ur);
    }

    //Check for colission between bullets and player to make damage
    for(auto mit : m_players)
    {
        PlayerEntity* player = mit.second;
        for(BulletEntity* bullet : m_Activebullets)
        {
            if(bullet->isActive() && bullet->getOwner().getEntitySide() != player->getEntitySide())
            {
                if(bullet->isColidingWith(*player))
                {
                    player->Damage(bullet->getDamage());
                    bullet->setActive(false);
                    if(player->isDead() && player->getName() == GameEngine::Instance().getNetworkManager().getDisplayName())
                    {
                        GameEngine::Instance().getNetworkManager().send_tcp(new PlayerDieNetMessage(player->getName(), bullet->getOwner().getName()));
                    }
                }
            }
        }
    }

    //Destroy any inactive bullets
    DestroyBulltet();
}


void GameScene::HandleNetworkInput(NetMessage* msg)
{
    Log() << (sf::Uint16)msg->getType();
    switch(msg->getType())
    {
    case eNetMessageType::eInGamePlayerPosUpdate:
        {
            PlayerStatusUpdateNetMessage* psunm = (PlayerStatusUpdateNetMessage*)msg;
            if(m_players.find(psunm->Who) != m_players.end())
            {
                PlayerEntity* p = m_players.at(psunm->Who);
                p->setPos(psunm->Position);
                p->setPlayerStatus((PlayerEntity::ePlayerState)psunm->Status, false);
                p->setDirection((Entity::eEntityDirection)psunm->Direction);
            }
            break;
        }
    case eNetMessageType::eBulletInstanciateMessage:
        {
            BulletInstanciateNetMessage* bi = (BulletInstanciateNetMessage*)msg;
            if(m_players.find(bi->WhoShoot) != m_players.end())
            {
                InstantiateBulletNet(*m_players.at(bi->WhoShoot), bi->StartPosition, bi->Direction, bi->BulletSpeed);
            }
            break;
        }
    case eNetMessageType::eHpPlayerUpdate:
        {
            HPPlayerUpdateNetMessage* hpUpdate = (HPPlayerUpdateNetMessage*)msg;
            if(m_players.find(hpUpdate->Who) != m_players.end())
            {
                m_players.at(hpUpdate->Who)->setHP(hpUpdate->newHp);
            }
            break;
        }
    case eNetMessageType::ePlayerBulletCountUpdate:
        {
            PlayerBulletCountUpdateNetMessage* ammoUpdate = (PlayerBulletCountUpdateNetMessage*)msg;
            if(m_players.find(ammoUpdate->WhoUpdating) != m_players.end())
            {
                m_players.at(ammoUpdate->WhoUpdating)->setAmmo(ammoUpdate->NewAmount);
                m_players.at(ammoUpdate->WhoUpdating)->setMaxAmmo(ammoUpdate->NewMaxAmount);
            }
            break;
        }
    case eNetMessageType::ePlayerRespanwsMessage:
        {
            PlayerRespawnNetMessage* respawnPlayer = (PlayerRespawnNetMessage*)msg;
            if(m_players.find(respawnPlayer->WhoRespawned) != m_players.end())
            {
                m_players.at(respawnPlayer->WhoRespawned)->Respawn(respawnPlayer->RespawnPos);
            }
            break;
        }
    case eNetMessageType::ePlayerDiesMessage:
        {
            PlayerDieNetMessage* diePLAYER = (PlayerDieNetMessage*)msg;
            if(m_players.find(diePLAYER->WhoDied) != m_players.end())
            {
                m_players.at(diePLAYER->WhoDied)->Die();
            }
            break;
        }
    case eNetMessageType::ePlayerQuitMessage:
        {
            PlayerExitMatch* playerQuit = (PlayerExitMatch*)msg;
            if(m_players.find(playerQuit->WhoDisconnected) != m_players.end())
            {
                m_players.at(playerQuit->WhoDisconnected)->setActive(false);
            }
            break;
        }
    }
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
                                                                   std::bind(&GameScene::InstantiateBullet, std::ref(*this), std::placeholders::_1, std::placeholders::_2)))));

    if(m_t1_p2 != "NONE")
        m_players.insert(std::make_pair(m_t1_p2, (new PlayerEntity(Entity::eEntitySide::eTeam1, m_t1_p2,
                                                                   std::bind(&GameScene::InstantiateBullet, this, std::placeholders::_1, std::placeholders::_2)))));

    if(m_t2_p1 != "NONE")
        m_players.insert(std::make_pair(m_t2_p1, (new PlayerEntity(Entity::eEntitySide::eTeam2, m_t2_p1,
                                                                   std::bind(&GameScene::InstantiateBullet, this, std::placeholders::_1, std::placeholders::_2)))));

    if(m_t2_p2 != "NONE")
        m_players.insert(std::make_pair(m_t2_p2, (new PlayerEntity(Entity::eEntitySide::eTeam2, m_t2_p2,
                                                                    std::bind(&GameScene::InstantiateBullet, this, std::placeholders::_1, std::placeholders::_2)))));

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
    for(BulletEntity* e : m_Activebullets)
    {
        delete e;
    }
    for(BulletEntity* e : m_bullets)
    {
        delete e;
    }
    for(auto mit : m_players)
    {
        delete mit.second;
    }
    m_Activebullets.clear();
    m_bullets.clear();
    m_players.clear();
    return true;
}

bool GameScene::InstantiateBullet(PlayerEntity& whoIsShooting, float speed)
{
    if(m_bullets.size() > 0)
    {
        BulletEntity* b = m_bullets.back();
        m_bullets.pop_back();
        b->Instantiate(whoIsShooting, speed);
        m_Activebullets.push_back(b);
        return true;
    }
    return false;
}

bool GameScene::InstantiateBulletNet(PlayerEntity& whoIsShooting, sf::Vector2f startPos, sf::Uint16 dir, float speed)
{
    if(m_bullets.size() > 0)
    {
        BulletEntity* b = m_bullets.back();
        m_bullets.pop_back();
        b->Instantiate(whoIsShooting, startPos, (Entity::eEntityDirection)dir, speed);
        m_Activebullets.push_back(b);
        return true;
    }
    Log(l_WARN) << "Could not instantiate a bullet that was received over the network";
    return false;
}



void GameScene::DestroyBulltet()
{
    for(std::vector<BulletEntity*>::iterator it = m_Activebullets.begin(); it != m_Activebullets.end();)
    {
        if(!(*it)->isActive())
        {
            m_bullets.push_back(*it);
            it = m_Activebullets.erase(it);
        } else {
            it++;
        }
    }
}


void GameScene::handlePlayerInput(sf::Keyboard::Key key, bool pressed)
{
    PlayerEntity* p = m_players.at(GameEngine::Instance().getNetworkManager().getDisplayName());
    Entity::eEntityDirection oldDir = p->getCurrentDirrection();
    PlayerEntity::ePlayerState oldState = p->getPlayerStatus();
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
        case sf::Keyboard::Space:
            if(p->canShoot())
            {
                p->shoot();
            }
            break;
        case sf::Keyboard::R:
            p->setAmmo(p->getMaxAmmo());
            break;
    }

    if(oldDir != p->getCurrentDirrection() || oldState != p->getPlayerStatus())
    {
        GameEngine::Instance().getNetworkManager().send_udp(new PlayerStatusUpdateNetMessage(p->getName(), p->getPos(), (sf::Uint16)p->getCurrentDirrection(), (sf::Uint16)p->getPlayerStatus()));
    }
}


