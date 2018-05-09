#ifndef PLAYERENTITY_HPP
#define PLAYERENTITY_HPP

#include <Entity.hpp>
#include <functional>

class PlayerEntity : public Entity
{
    public:
        PlayerEntity(eEntitySide s, const std::string& m_dn, std::function<bool(PlayerEntity&, float)>& shootFunction);
        virtual ~PlayerEntity();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;

        void shoot();

        void addAmmo(int ammoToAdd);
        int getAmmo() const;

        //void InteractInFront();

    protected:

    private:
        int m_ammo;
        int m_maxAmmo;
        std::string m_name;
        std::function<bool(PlayerEntity&, float)>& m_shootFunct;

};

#endif // PLAYERENTITY_HPP
