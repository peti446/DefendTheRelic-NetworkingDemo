#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Logger.hpp"
#include "GameEngine.hpp"

class Entity
{
    public:
        enum class eEntityDirection
        {
            eNorth,
            eSorth,
            eWest,
            eEast,
            eStaticEntity
        };
        enum class eEntitySide
        {
            eTeam1,
            eTeam2,
            eNeutral
        };

        Entity();
        virtual ~Entity();

        virtual void Draw(sf::RenderWindow& rw) = 0;
        virtual void Update(const sf::Time& ur) = 0;
        virtual bool isDead() const;
        virtual void Damage(int damage);
        virtual void Heal(int healAmount);
        virtual void setDirection(eEntityDirection newDire, bool shouldRotate = false);
        void setHP(int newHP);
        void setPos(const sf::Vector2f& pos);
        bool setTexture(const std::string& newTexture);
        void setTexture(const sf::Texture& newTexture);
        void setActive(bool active);
        void setEntitySide(eEntitySide side);
        void setSpeed(float speed);

        const sf::Vector2f& getPos() const;
        const sf::Vector2f& getOldPos() const;
        const sf::Texture& getTexture() const;
        int getHP() const;
        bool isActive() const;
        eEntityDirection getCurrentDirrection() const;
        sf::Vector2i getDirectionVector() const;
        eEntitySide getEntitySide() const;
        bool is_ally_of(const Entity& e) const;
        float getSpeed() const;
        bool isColidingWith(const Entity& e) const;

    protected:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
        int m_hp;
        sf::Vector2f m_pos;
        sf::Vector2f m_oldPos;
        bool m_active;
        eEntityDirection m_dir;
        eEntitySide m_side;
        float m_speed;
    private:
};

#endif // ENTITY_HPP
