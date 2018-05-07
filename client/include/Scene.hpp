#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "Logger.hpp"
#include "GameEngine.hpp"
#include "StringHelpers.hpp"
#include "NetMessage.hpp"

class Scene
{
    public:
        Scene();
        virtual ~Scene();

        virtual void Draw(sf::RenderWindow& rw) = 0;
        virtual void Update(const sf::Time& ur) = 0;
        virtual void HandleInput(const sf::Event& event) = 0;
        virtual const std::string Name() const = 0;
        virtual void HandleNetworkInput(NetMessage* msg);
        void Load();
        void Unload();

        bool isLoaded() const;
    protected:
        bool m_loaded{false};
        tgui::Gui m_gui;
        tgui::Theme::Ptr m_theme;
    private:
        virtual bool LoadScene() = 0;
        virtual bool UnloadScene() = 0;
};

#endif // SCENE_H
