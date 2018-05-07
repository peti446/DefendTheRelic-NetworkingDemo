#ifndef OPTIONSSCENE_H
#define OPTIONSSCENE_H

#include <Scene.hpp>


class OptionsScene : public Scene
{
    public:
        OptionsScene();
        virtual ~OptionsScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;
};

#endif // OPTIONSSCENE_H
