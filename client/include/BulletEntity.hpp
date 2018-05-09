#ifndef BULLETENTITY_HPP
#define BULLETENTITY_HPP

#include <Entity.hpp>
class PlayerEntity;


class BulletEntity : public Entity
{
    public:
        BulletEntity(const sf::Texture& bulletTexture);
        virtual ~BulletEntity();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;

        void Instansiate(PlayerEntity& whoShoot, float speed);
    protected:

    private:
        int m_damage;
};

#endif // BULLETENTITY_HPP
