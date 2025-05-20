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

		enum RenderMode
		{
			World,
			UI
		};

		class Renderer
		{
		private:
			sf::RenderWindow* window;

			std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

			std::string rootDirectory;

			Mathematics::Vector2<float> WorldToPixelPos(Mathematics::Vector2<float> worldPos) const;
			Mathematics::Vector2<float> PixelToWorldPos(Mathematics::Vector2<float> worldPos) const;

			const std::uint16_t pixelsPerUnit = 25;

			Mathematics::Vector2<float> cameraPosition;

		public:
			Renderer(sf::RenderWindow* window, std::string rootDirectory);
			~Renderer();

			void Draw(const sf::Drawable& drawable);

			std::shared_ptr<sf::Texture> GetTexture(std::string textureName);

			void RenderSprites(ECS::World& world);
			void TargetCamera(Mathematics::Vector2<float> worldPos);

			Mathematics::Vector2<float> ScreenToWorldPos(Mathematics::Vector2<float> screenPos);

			void SetRenderMode(RenderMode renderMode);
		};

        struct Sprite
        {
            std::shared_ptr<sf::Texture> texture;
            Mathematics::Vector2<std::uint16_t> subTexOrigin;
            Mathematics::Vector2<std::uint16_t> subTexSize;
        };

		struct SpriteSheet
		{
			std::shared_ptr<sf::Texture> texture;
			Mathematics::Vector2<uint16_t> cellSize;

			Sprite GetSprite(uint16_t cellIndex)
			{
				div_t cellPosition = div(cellIndex, texture->getSize().x / cellSize.x);

				if ((std::uint16_t)(cellPosition.quot * cellSize.y) >= texture->getSize().y) throw std::runtime_error("Cell index out of texture bounds.");

				return { texture, Mathematics::Vector2<uint16_t>(cellPosition.rem * cellSize.x, cellPosition.quot * cellSize.y), cellSize };
			}

			SpriteSheet(std::shared_ptr<sf::Texture> _texture, Mathematics::Vector2<uint16_t> _cellSize) : texture(_texture), cellSize(_cellSize) { }
			SpriteSheet() { }
		};
	}
}