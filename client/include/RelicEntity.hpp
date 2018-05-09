#ifndef RELICENTITY_HPP
#define RELICENTITY_HPP

#include <Entity.hpp>


class RelicEntity : public Entity
{
    public:
        RelicEntity();
        virtual ~RelicEntity();

        void Draw(sf::RenderWindow& rw) final override;

    protected:

    private:
};

#endif // RELICENTITY_HPP
