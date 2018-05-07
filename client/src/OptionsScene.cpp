#include "OptionsScene.hpp"

//1024×576, 1152×648, 1280×720, 1366×768, 1600×900, 1920×1080, 2560×1440 and 3840×2160. 16:9
OptionsScene::OptionsScene()
{
    //ctor
}

OptionsScene::~OptionsScene()
{
    //dtor
}


void OptionsScene::Draw(sf::RenderWindow& rw)
{
}

void OptionsScene::Update(const sf::Time& ur)
{

}

void OptionsScene::HandleInput(const sf::Event& event)
{

}

const std::string OptionsScene::Name() const
{
    return "Options";
}

bool OptionsScene::LoadScene()
{
    return true;
}

bool OptionsScene::UnloadScene()
{
    return true;
}
