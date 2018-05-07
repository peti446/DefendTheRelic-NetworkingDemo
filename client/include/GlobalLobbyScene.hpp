#ifndef GLOBALLOBBYSCENE_H
#define GLOBALLOBBYSCENE_H

#include <Scene.hpp>


class GlobalLobbyScene : public Scene
{
    public:
        GlobalLobbyScene();
        virtual ~GlobalLobbyScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;
};

#endif // GLOBALLOBBYSCENE_H
