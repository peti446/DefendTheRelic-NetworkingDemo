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

        void shoot();

        void setPlayerStatus(ePlayerState newState);
        ePlayerState getPlayerStatus() const;
        void addAmmo(int ammoToAdd);
        int getAmmo() const;


        const std::string& getName() const;

    protected:

    private:
        int m_ammo;
        int m_maxAmmo;
        std::string m_name;
        std::function<bool(PlayerEntity&, float)>& m_shootFunct;
        ePlayerState m_state;

};

#endif // PLAYERENTITY_HPP
