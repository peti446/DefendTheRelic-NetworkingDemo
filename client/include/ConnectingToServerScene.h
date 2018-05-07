#ifndef CONNECTINGTOSERVERSCENE_H
#define CONNECTINGTOSERVERSCENE_H

#include <Scene.hpp>
#include <future>


class ConnectingToServerScene : public Scene
{
    public:
        ConnectingToServerScene(sf::IpAddress ip = sf::IpAddress::Broadcast, unsigned short port = 13001);
        virtual ~ConnectingToServerScene();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;
        std::future<bool> m_connectToServer;
        sf::IpAddress m_ip;
        unsigned short m_port;
        size_t m_textSize;
        tgui::Label::Ptr Title;
        float m_secondsPassed;
        short m_textDir;
};

#endif // CONNECTINGTOSERVERSCENE_H
