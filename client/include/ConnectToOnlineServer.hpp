#ifndef CONNECTTOONLINESERVER_H
#define CONNECTTOONLINESERVER_H

#include <Scene.hpp>

class ConnectToOnlineServer : public Scene
{
    public:
        ConnectToOnlineServer(sf::IpAddress a = sf::IpAddress::None, unsigned short port = 0);
        virtual ~ConnectToOnlineServer();

        void Draw(sf::RenderWindow& rw) final override;
        void Update(const sf::Time& ur) final override;
        void HandleInput(const sf::Event& event) final override;
        const std::string Name() const final override;

    protected:

    private:
        bool LoadScene() final override;
        bool UnloadScene() final override;

        void onClickCancel();
        void onConnectClick(std::string ip, std::string port);

        sf::IpAddress m_offlineAddres;
        unsigned short m_offlinePort;
        tgui::Label::Ptr m_error;
};

#endif // CONNECTTOONLINESERVER_H
