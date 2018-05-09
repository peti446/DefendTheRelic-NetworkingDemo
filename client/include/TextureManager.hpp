#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <string>

class TextureManager
{
    public:
        TextureManager();
        virtual ~TextureManager();

        sf::Texture& getTexture(std::string path);
        void unloadTexture(std::string path);

    protected:

    private:
        struct TextureBatch
        {
            sf::Texture* m_texture;
            unsigned int m_amountUsed;
            TextureBatch(sf::Texture* tex, unsigned int amountUSed):m_texture(tex), m_amountUsed(amountUSed)
            {}
        };

        std::unordered_map<std::string, TextureBatch> m_textures;

};

#endif // TEXTUREMANAGER_HPP
