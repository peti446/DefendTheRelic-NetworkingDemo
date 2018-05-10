#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include <Entity.hpp>
#include <functional>

class PlayerEntity : public Entity
{
    public:
        enum class ePlayerState
        {
          eWalking,
          eIDLE
        };
        PlayerEntity(eEntitySide s, const std::string& m_dn, std::function<bool(PlayerEntity&, float)> shootFunction);
        virtual ~PlayerEntity();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void Damage(int damage) final override;
        void Heal(int healAmount) final override;
        void setHP(int newHP) final override;

        void Try_respawn();
        void Respawn(sf::Vector2f pos);
        bool shoot();
        bool canShoot();

        void setPlayerStatus(ePlayerState newState, bool send = true);
        ePlayerState getPlayerStatus() const;
        void addAmmo(int ammoToAdd);
        void setAmmo(int newAmmo);
        void setMaxAmmo(int newMaxAmmo);
        int getAmmo() const;
        int getMaxAmmo() const;


        const std::string& getName() const;

    protected:

    private:
        int m_ammo;
        int m_maxAmmo;
        float m_shootVelocity;
        float m_timePassedSinceLastShoot;
        float m_respawnTime;
        float m_timeDead;
        std::string m_name;
        std::function<bool(PlayerEntity&, float)> m_shootFunct;
        ePlayerState m_state;

};

#endif // PLAYERENTITY_HPP
