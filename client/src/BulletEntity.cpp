#include "BulletEntity.hpp"
#include "PlayerEntity.hpp"

BulletEntity::BulletEntity(const sf::Texture& bulletTexture)
{
    setTexture(bulletTexture);
    setActive(false);
    setHP(0);
}

BulletEntity::~BulletEntity()
{
    //dtor
}

void BulletEntity::Draw(sf::RenderWindow& rw)
{
    if(isActive())
        rw.draw(m_sprite);
}

void BulletEntity::Update(const sf::Time& ur)
{
    if(isActive())
    {
        sf::Vector2i dir = getDirectionVector();
        sf::Vector2f newPos  = m_pos + (getSpeed() * sf::Vector2f(dir));
        setPos(newPos);
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
