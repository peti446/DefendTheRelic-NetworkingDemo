#include "PlayerEntity.hpp"
#include "BulletInstanciateNetMessage.hpp"
#include "PlayerBulletCountUpdateNetMessage.hpp"
#include "HPPlayerUpdateNetMessage.hpp"
#include "PlayerRespawnNetMessage.hpp"
#include <cstdlib>

PlayerEntity::PlayerEntity(eEntitySide s, const std::string& m_dn, std::function<bool(PlayerEntity&, float)> shootFunction) : m_shootFunct(shootFunction), m_name(m_dn),
                                                                                                                              m_state(ePlayerState::eIDLE), m_shootVelocity(0.5f),
                                                                                                                              m_timePassedSinceLastShoot(0.6f), m_maxAmmo(60),
                                                                                                                              m_ammo(60), m_respawnTime(5), m_timeDead(0)
{
    setEntitySide(s);
    setActive(true);
    setSpeed(100);
    m_sprite.setOrigin(sf::Vector2f(16,16));
    if(m_dn == GameEngine::Instance().getNetworkManager().getDisplayName())
    {
        setTexture(GameEngine::Instance().getTextureManager().getTexture("Media/Textures/Game/Self.png"));
    }
}

PlayerEntity::~PlayerEntity()
{

}

void PlayerEntity::Draw(sf::RenderWindow& rw)
{
    if(isActive())
        rw.draw(m_sprite);
}

void PlayerEntity::Update(const sf::Time& ur)
{
    if(isDead())
    {
        m_timeDead += ur.asSeconds();
        if(m_timeDead > m_respawnTime)
        {
            m_timeDead = 0;
            Try_respawn();
        }
    }

    if(!isActive())
        return;

    m_timePassedSinceLastShoot+= ur.asSeconds();
    if(m_state == ePlayerState::eWalking)
    {
        sf::Vector2i dir = getDirectionVector();
        sf::Vector2f newPos  = m_pos + (getSpeed() * ur.asSeconds() * sf::Vector2f(dir));
        setPos(newPos);
    }
}

void PlayerEntity::Try_respawn()
{
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
    {
        sf::Vector2f newPos = sf::Vector2f(rand() % GameEngine::Instance().getRenderWindow().getSize().x, rand() % GameEngine::Instance().getRenderWindow().getSize().y);
        GameEngine::Instance().getNetworkManager().send_tcp(new PlayerRespawnNetMessage(m_name, newPos));
    }
}

void PlayerEntity::Respawn(sf::Vector2f pos)
{
    if(isDead())
    {
        setActive(true);
        setHP(100);
        setAmmo(m_maxAmmo);
        setPos(pos);
    }
}


void PlayerEntity::Damage(int damage)
{
    Entity::Damage(damage);
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        GameEngine::Instance().getNetworkManager().send_udp(new HPPlayerUpdateNetMessage(m_name, m_hp));
}

void PlayerEntity::Heal(int healAmount)
{
    Entity::Heal(healAmount);
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        GameEngine::Instance().getNetworkManager().send_udp(new HPPlayerUpdateNetMessage(m_name, m_hp));
}

void PlayerEntity::setHP(int newHP)
{
    Entity::Heal(newHP);
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        GameEngine::Instance().getNetworkManager().send_udp(new HPPlayerUpdateNetMessage(m_name, m_hp));
}


bool PlayerEntity::shoot()
{
    if(!canShoot())
        return false;
    if(m_shootFunct(*this, 10))
    {
        m_timePassedSinceLastShoot = 0;
        m_ammo -= 1;
        if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        {
            GameEngine::Instance().getNetworkManager().send_udp(new BulletInstanciateNetMessage(m_name, m_pos, (sf::Uint16)m_dir, 10));
            GameEngine::Instance().getNetworkManager().send_udp(new PlayerBulletCountUpdateNetMessage(m_name, m_ammo, m_maxAmmo));
        }
        return true;
    } else
    {
        Log(l_WARN) << "There was not any bullet left to Instantiate";
        return false;
    }
}
bool PlayerEntity::canShoot()
{
   return (m_ammo > 0 && m_timePassedSinceLastShoot >= m_shootVelocity);
}


void PlayerEntity::addAmmo(int ammoToAdd)
{
    m_ammo += ammoToAdd;
    if(m_ammo > m_maxAmmo)
        m_ammo = m_maxAmmo;
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        GameEngine::Instance().getNetworkManager().send_udp(new PlayerBulletCountUpdateNetMessage(m_name, m_ammo, m_maxAmmo));
}

void PlayerEntity::setPlayerStatus(ePlayerState newState, bool send)
{
    m_state = newState;
}

PlayerEntity::ePlayerState PlayerEntity::getPlayerStatus() const
{
    return m_state;
}


int PlayerEntity::getAmmo() const
{
    return m_ammo;
}

const std::string& PlayerEntity::getName() const
{
    return m_name;
}

void PlayerEntity::setAmmo(int newAmmo)
{
    m_ammo = newAmmo;
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        GameEngine::Instance().getNetworkManager().send_udp(new PlayerBulletCountUpdateNetMessage(m_name, m_ammo, m_maxAmmo));
}

void PlayerEntity::setMaxAmmo(int newMaxAmmo)
{
    m_maxAmmo = newMaxAmmo;
    if(m_name == GameEngine::Instance().getNetworkManager().getDisplayName())
        GameEngine::Instance().getNetworkManager().send_udp(new PlayerBulletCountUpdateNetMessage(m_name, m_ammo, m_maxAmmo));
}


int PlayerEntity::getMaxAmmo() const
{
    return m_maxAmmo;
}

