#include "PlayerEntity.hpp"

PlayerEntity::PlayerEntity(eEntitySide s, const std::string& m_dn, std::function<bool(PlayerEntity&)& shootFunction) : m_shootFunct(shootFunction), m_name(m_dn)
{
    setEntitySide(s);
    if(m_dn == GameEngine::Instance().getNetworkManager().getDisplayName())
    {
        if(!setTexture("Media/Textures/Game/Self.png"))
        {
            Log(l_WARN) << "Could not load the self texture for the player " + m_dn;
        }
    {
}

PlayerEntity::~PlayerEntity()
{
}

void PlayerEntity::Draw(sf::RenderWindow& rw)
{
    m_sprite.draw();
}

void PlayerEntity::Update(const sf::Time& ur)
{

}

void PlayerEntity::shoot()
{
    if(m_ammo == 0)
        return;
    shootFunction(*this, 10);
    m_ammo -= 1;
}

void PlayerEntity::addAmmo(int ammoToAdd)
{
    m_ammo += ammoToAdd;
    if(m_ammo > m_maxAmmo)
        m_ammo = m_maxAmmo;
}

int PlayerEntity::getAmmo() const
{
    return m_ammo;
}
