#include "PlayerEntity.hpp"

PlayerEntity::PlayerEntity(eEntitySide s, const std::string& m_dn, std::function<bool(PlayerEntity&, float)> shootFunction) : m_shootFunct(shootFunction), m_name(m_dn),
                                                                                                                                m_state(ePlayerState::eIDLE)
{
    setEntitySide(s);
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
    if(isActive())
    {

    }
}

void PlayerEntity::shoot()
{
    if(m_ammo == 0)
        return;
    if(m_shootFunct(*this, 10))
    {
        m_ammo -= 1;
    } else
    {
        Log(l_WARN) << "There was not any bullet left to instanciate";
    }
}

void PlayerEntity::addAmmo(int ammoToAdd)
{
    m_ammo += ammoToAdd;
    if(m_ammo > m_maxAmmo)
        m_ammo = m_maxAmmo;
}

void PlayerEntity::setPlayerStatus(ePlayerState newState)
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
