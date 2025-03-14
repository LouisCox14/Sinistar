#include "Renderer.h"
#include "Physics.h"

void Weave::Graphics::Renderer::RenderSprites(ECS::World& world)
{
	for (auto [entity, transform, sprite] : world.GetView<Weave::Transform, Weave::Graphics::Sprite>())
	{
		sf::Sprite renderSprite(*sprite.texture, sf::IntRect(sprite.subTexOrigin, sprite.subTexSize));

		renderSprite.setOrigin(sprite.subTexSize / 2);
		renderSprite.setPosition(WorldToPixelPos(transform.position));
		renderSprite.setRotation(sf::degrees(transform.rotation));
		renderSprite.setScale(transform.scale);

		window->draw(renderSprite);
	}
}

void Weave::Graphics::Renderer::TargetCamera(Mathematics::Vector2<float> worldPos)
{
	sf::View camera(sf::FloatRect({ 0.f, 0.f }, { 320.f, 240.f }));
	camera.setCenter(WorldToPixelPos(worldPos));
	window->setView(camera);
}

Weave::Mathematics::Vector2<float> Weave::Graphics::Renderer::ScreenToWorldPos(Weave::Mathematics::Vector2<float> screenPos)
{
	screenPos /= Weave::Mathematics::Vector2<unsigned>(window->getSize());
	screenPos *= Weave::Mathematics::Vector2<float>(window->getView().getSize());
	screenPos -= Weave::Mathematics::Vector2<float>(window->getView().getCenter() + (window->getView().getSize() / 2.0f));

	return PixelToWorldPos(screenPos);
}

std::shared_ptr<sf::Texture> Weave::Graphics::Renderer::GetTexture(std::string textureName)
{
	if (textures.contains(textureName)) return textures[textureName];

	textures[textureName] = std::make_shared<sf::Texture>(rootDirectory + textureName);

	return textures[textureName];
}

Weave::Mathematics::Vector2<float> Weave::Graphics::Renderer::WorldToPixelPos(Weave::Mathematics::Vector2<float> worldPos) const
{
	Mathematics::Vector2<float> adjustedPosition = worldPos * (float)pixelsPerUnit;
	adjustedPosition.y *= -1;
	return adjustedPosition;
}

Weave::Mathematics::Vector2<float> Weave::Graphics::Renderer::PixelToWorldPos(Weave::Mathematics::Vector2<float> worldPos) const
{
	Mathematics::Vector2<float> adjustedPosition = worldPos / (float)pixelsPerUnit;
	adjustedPosition.y *= -1;
	return adjustedPosition;
}

Weave::Graphics::Renderer::Renderer(sf::RenderWindow* _window, std::string _rootDirectory) : window(_window), rootDirectory(_rootDirectory) { }

Weave::Graphics::Renderer::~Renderer() { }
