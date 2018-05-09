#include "BulletEntity.hpp"

BulletEntity::BulletEntity(const sf::Texture& bulletTexture) : m_active(false), m_hp(1)
{
    setTexture(bulletTexture);
}

BulletEntity::~BulletEntity()
{
    //dtor
}

void BulletEntity::Draw(sf::RenderWindow& rw)
{

}
void BulletEntity::Update(const sf::Time& ur)
{
    if(isActive())
    {
        sf::Vector2i dir = getDirectionVector();
        dir *= getSpeed();
        setPos(m_pos + dir);
    }
}


void BulletEntity::Instansiate(PlayerEntity& whoShoot, float speed)
{
    setSpeed(speed);
    setDirection(whoShoot.getCurrentDirrection());
    setEntitySide(whoShoot.getEntitySide());
    setHP(1);
    setActive(true);
    m_damage = 10;
}
