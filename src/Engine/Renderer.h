#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "ECS.h"
#include "Transform.h"

namespace Weave
{
	namespace Graphics
	{
		using TextureID = std::size_t;

		class Renderer
		{
		private:
			sf::RenderWindow* window;

			std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

			std::string rootDirectory;

		public:
			Renderer(sf::RenderWindow* window, std::string rootDirectory);
			~Renderer();

			std::shared_ptr<sf::Texture> GetTexture(std::string textureName);

			void RenderSprites(ECS::World& world);
		};

        struct Sprite
        {
            std::shared_ptr<sf::Texture> texture;
            Mathematics::Vector2<std::uint16_t> subTexOrigin;
            Mathematics::Vector2<std::uint16_t> subTexSize;
        };

	}
}