#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "Logger.hpp"

class Scene
{
    public:
        Scene();
        virtual ~Scene();

        virtual void Draw() = 0;
        virtual void Update(const float ur) = 0;
        virtual void HandleInput(const sf::Event& event) = 0;
        virtual const std::string Name() const = 0;

    protected:

    private:
};

#endif // SCENE_H
