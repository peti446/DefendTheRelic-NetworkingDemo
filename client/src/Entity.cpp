#include "Entity.hpp"

Entity::Entity() : m_active(false), m_hp(100), m_texture(), m_pos(0,0), m_oldPos(0,0), m_dir(eEntityDirection::eStaticEntity), m_sprite(), m_side(eEntitySide::eNeutral), m_speed(0)
{
}

Entity::~Entity()
{
}

bool Entity::isDead() const
{
    return m_hp == 0;
}

void Entity::Die()
{
    setActive(false);
    m_hp = 0;
}

void Entity::Damage(int damage)
{
    m_hp -= damage;
    if(m_hp < 0)
        m_hp = 0;
    if(isDead())
        Die();
}

void Entity::Heal(int healAmount)
{
    m_hp += healAmount;
    if(isDead())
        Die();
}

void Entity::setHP(int newHP)
{
    m_hp = newHP;
    if(m_hp < 0)
        m_hp = 0;
    if(isDead())
        Die();
}

void Entity::setPos(const sf::Vector2f& pos)
{
    m_oldPos = m_pos;
    m_pos = pos;
    if(m_pos.x < 0)
        m_pos.x = 0;
    if(m_pos.x >= GameEngine::Instance().getRenderWindow().getSize().x)
        m_pos.x = GameEngine::Instance().getRenderWindow().getSize().x;
    if(m_pos.y < 0)
        m_pos.y = 0;
    if(m_pos.y >= GameEngine::Instance().getRenderWindow().getSize().y)
        m_pos.y = GameEngine::Instance().getRenderWindow().getSize().y;
    m_sprite.setPosition(m_pos);
}

bool Entity::setTexture(const std::string& newTexture)
{
    if(m_texture.loadFromFile(newTexture))
    {
        m_sprite.setTexture(m_texture);
        setDirection(m_dir);
        return true;
    }

    return false;
}

void Entity::setTexture(const sf::Texture& newTexture)
{
    m_texture = newTexture;
    m_sprite.setTexture(m_texture);
    setDirection(m_dir);
}

void Entity::setDirection(eEntityDirection newDire, bool shouldRotate)
{
    m_dir = newDire;
    if(shouldRotate)
    {
        switch(m_dir)
        {
            case eEntityDirection::eNorth:
                    //0 Degrees as the input texture is expected to be up and then rotated based on direction
                    m_sprite.setRotation(0);
                break;
            case eEntityDirection::eSorth:
                    //180 Degrees
                    m_sprite.setRotation(180);
                break;
            case eEntityDirection::eEast:
                    //90 Degrees
                    m_sprite.setRotation(90);
                break;
            case eEntityDirection::eWest:
                    //270 Degrees
                    m_sprite.setRotation(270);
                break;
        }
    }
}

void Entity::setActive(bool active)
{
    m_active = active;
}

void Entity::setEntitySide(eEntitySide side)
{
    m_side = side;
}

void Entity::setSpeed(float speed)
{
    m_speed = speed;
}

const sf::Vector2f& Entity::getPos() const
{
    return m_pos;
}

const sf::Vector2f& Entity::getOldPos() const
{
    return m_oldPos;
}

const sf::Texture& Entity::getTexture() const
{
    return m_texture;
}

int Entity::getHP() const
{
    return m_hp;
}

Entity::eEntityDirection Entity::getCurrentDirrection() const
{
    return m_dir;
}

sf::Vector2i Entity::getDirectionVector() const
{
    switch(m_dir)
    {
        case eEntityDirection::eNorth:
                return sf::Vector2i(0,-1);
            break;
        case eEntityDirection::eSorth:
                return sf::Vector2i(0, 1);
            break;
        case eEntityDirection::eEast:
                return sf::Vector2i(1, 0);
            break;
        case eEntityDirection::eWest:
                return sf::Vector2i(-1, 0);
            break;
    }
    return sf::Vector2i(0, 0);
}

bool Entity::isActive() const
{
    return m_active;
}

Entity::eEntitySide Entity::getEntitySide() const
{
    return m_side;
}

bool Entity::is_ally_of(const Entity& e) const
{
    if(!(m_side == eEntitySide::eNeutral || e.m_side == eEntitySide::eNeutral) && m_side == e.m_side)
        return true;

    return false;
}

float Entity::getSpeed() const
{
    return m_speed;
}

bool Entity::isColidingWith(const Entity& e) const
{
    if(!isActive() || !e.isActive())
        return false;
    return m_sprite.getGlobalBounds().intersects(e.m_sprite.getGlobalBounds());
}
