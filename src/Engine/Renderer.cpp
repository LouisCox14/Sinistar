#include "Renderer.h"
#include "Physics.h"

void Weave::Graphics::Renderer::RenderSprites(ECS::World& world)
{
	for (auto [entity, transform, sprite] : world.GetView<Weave::Transform, Weave::Graphics::Sprite>())
	{
		sf::Sprite renderSprite(*sprite.texture, sf::IntRect(sprite.subTexOrigin, sprite.subTexSize));

		/*
		*	Creating a new sf::Sprite for every entity every frame might seem inefficient, 
		*	but sf::Sprites take up 280 bytes to store information I either don't need or 
		*	already store as part of the transform component. Using a Weave::Sprite instead 
		*	takes up 24 bytes, which really helps cache lines and does end up being faster.
		*/

		renderSprite.setOrigin(sprite.subTexSize / 2);
		renderSprite.setPosition(WorldToPixelPos(transform.position));
		renderSprite.setRotation(sf::degrees(transform.rotation));
		renderSprite.setScale(transform.scale);

		window->draw(renderSprite);
	}

	for (auto [entity, transform, collision] : world.GetView<Weave::Transform, Weave::Physics::Colliding>())
	{
		sf::CircleShape circle(5.0f);

		circle.setOrigin( { 5.0f, 5.0f } );
		circle.setPosition(WorldToPixelPos(transform.position));
		circle.setFillColor(sf::Color::Red);

		switch (collision.collisions.begin()->second.state)
		{
		case Weave::Physics::CollisionState::Started:
			std::cout << "[Entity " << entity << "] Collision started!\n";
			circle.setFillColor(sf::Color::Red);
			break;
		case Weave::Physics::CollisionState::Continued:
			circle.setFillColor(sf::Color::Yellow);
			break;
		case Weave::Physics::CollisionState::Ended:
			std::cout << "[Entity " << entity << "] Collision ended!\n";
			circle.setFillColor(sf::Color::Green);
			break;
		default:
			break;
		}

		window->draw(circle);
	}
}

void Weave::Graphics::Renderer::TargetCamera(Mathematics::Vector2<float> worldPos)
{
	sf::View camera(sf::FloatRect({ 0.f, 0.f }, { 320.f, 240.f })); // 320x240 was the resolution of the original Sinistar. I should probably make this a variable.
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
