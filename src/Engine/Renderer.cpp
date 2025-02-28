#include "Renderer.h"

void Weave::Graphics::Renderer::RenderSprites(ECS::World& world)
{
	std::cout << "\n ----- RenderSprites / Start ----- \n";

	for (auto [entity, transform, sprite] : world.GetView<Weave::Transform, Weave::Graphics::Sprite>())
	{
		std::cout << "[Renderer] Entity " << entity << " has a Sprite (this: " << &sprite << ").\n";
		std::cout << "[Renderer] Sprite texture: " << sprite.texture << "\n";

		std::cout << "[Renderer] Entity position: " << transform.position << "\n";
		std::cout << "[Renderer] Entity scale: " << transform.scale << "\n";

		sf::Sprite renderSprite(*sprite.texture, sf::IntRect(sprite.subTexOrigin, sprite.subTexSize));

		renderSprite.setPosition(transform.position);
		renderSprite.setRotation(sf::degrees(transform.rotation));
		renderSprite.setScale(transform.scale);

		window->draw(renderSprite);
	}

	std::cout << "\n ----- RenderSprites / End ----- \n";
}

std::shared_ptr<sf::Texture> Weave::Graphics::Renderer::GetTexture(std::string textureName)
{
	if (textures.contains(textureName)) return textures[textureName];

	textures[textureName] = std::make_shared<sf::Texture>(rootDirectory + textureName);

	return textures[textureName];
}

Weave::Graphics::Renderer::Renderer(sf::RenderWindow* _window, std::string _rootDirectory) : window(_window), rootDirectory(_rootDirectory) { }

Weave::Graphics::Renderer::~Renderer() { }
