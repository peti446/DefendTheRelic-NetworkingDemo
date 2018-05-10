#ifndef BULLETENTITY_HPP
#define BULLETENTITY_HPP

#include <Entity.hpp>
#include <functional>
class PlayerEntity;


class BulletEntity : public Entity
{
    public:
        BulletEntity(const sf::Texture& bulletTexture);
        virtual ~BulletEntity();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;

        void Instantiate(PlayerEntity& whoShoot, float speed);
        void Instantiate(PlayerEntity& whoShoot, sf::Vector2f startPos, Entity::eEntityDirection dir, float speed);
        int getDamage() const;
        const PlayerEntity& getOwner() const;
    protected:

    private:
        int m_damage;
        PlayerEntity* m_Owner;
        std::function<void(BulletEntity*)> m_destroyFunction;
};

#endif // BULLETENTITY_HPP
