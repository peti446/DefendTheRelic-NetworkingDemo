#include "BulletEntity.hpp"
#include "PlayerEntity.hpp"

BulletEntity::BulletEntity(const sf::Texture& bulletTexture) : m_damage(5)
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
        if(newPos.x <= 0 || newPos.y <= 0 || newPos.x >= GameEngine::Instance().getRenderWindow().getSize().x || newPos.y >= GameEngine::Instance().getRenderWindow().getSize().y)
        {
            setActive(false);
        }
    }
}


void BulletEntity::Instantiate(PlayerEntity& whoShoot, float speed)
{
    setSpeed(speed);
    setDirection(whoShoot.getCurrentDirrection());
    setEntitySide(whoShoot.getEntitySide());
    setHP(1);
    setPos(whoShoot.getPos());
    setActive(true);
    m_damage = 10;
    m_Owner = &whoShoot;
}


void BulletEntity::Instantiate(PlayerEntity& whoShoot, sf::Vector2f startPos, Entity::eEntityDirection dir, float speed)
{
    setSpeed(speed);
    setDirection(dir);
    setEntitySide(whoShoot.getEntitySide());
    setHP(1);
    setPos(startPos);
    setActive(true);
    m_damage = 10;
    m_Owner = &whoShoot;
}

int BulletEntity::getDamage() const
{
    return m_damage;
}


const PlayerEntity& BulletEntity::getOwner() const
{
    return *m_Owner;
}
