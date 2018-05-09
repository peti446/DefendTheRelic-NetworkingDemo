#include "TextureManager.hpp"
#include "Logger.hpp"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
    for(auto miter : m_textures)
    {
        delete miter.second.m_texture;
    }
}


sf::Texture& TextureManager::getTexture(std::string path)
{
    if(m_textures.find(path) != m_textures.end())
    {
        //Get already loaded texture
        TextureBatch& currentTexture = m_textures.at(path);
        currentTexture.m_amountUsed++;
        //Double check if the texutre exits
        if(currentTexture.m_texture == nullptr)
        {
            currentTexture.m_texture = new sf::Texture();
            if(currentTexture.m_texture->loadFromFile(path))
            {
                Log(l_WARN) << "Could not load texture from the file: " + path;
            }
        }
        return *currentTexture.m_texture;
    } else
    {
        //Load texture
        sf::Texture* t = new sf::Texture();
        if(t->loadFromFile(path))
        {
            Log(l_WARN) << "Could not load texture from the file: " + path;
        }
        m_textures.insert(std::make_pair(path, TextureBatch(t,1)));
        return *t;
    }
}

void TextureManager::unloadTexture(std::string path)
{
    if(m_textures.find(path) != m_textures.end())
    {
        TextureBatch& b = m_textures.at(path);
        b.m_amountUsed--;
        if(b.m_amountUsed <= 0)
        {
            delete b.m_texture;
            m_textures.erase(path);
        }
    }
}
