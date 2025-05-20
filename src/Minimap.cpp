#pragma once
#include "Minimap.h"

Sinistar::MinimapManager::MinimapManager(Weave::Engine& engine)
{
	renderer = &engine.GetRenderer();

	minimapBorder = new sf::Sprite(*renderer->GetTexture("minimap_border_78x67.png"));
	minimapBorder->setOrigin({ -121.0f, 0.0f });
}

Sinistar::MinimapManager::~MinimapManager()
{
	delete minimapBorder;
}

void Sinistar::MinimapManager::DrawMinimap(Weave::ECS::World& world)
{
	sf::RectangleShape rectangle({ 48.0f, 48.0f });
	rectangle.setOrigin({ -136.0f, -19.0f });
	rectangle.setFillColor(sf::Color::Black);

	renderer->Draw(rectangle);
	renderer->Draw(*minimapBorder);

	sf::RectangleShape point({ 2.0f, 2.0f });

	for (auto [entity, transform, minimapEntity] : world.GetView<Weave::Transform, MinimapEntity>())
	{
		point.setFillColor(minimapEntity.colour);

		point.setOrigin(transform.position * Weave::Mathematics::Vector2<float>(-1.0f, 1.0f) / 200.0f * 45.0f + Weave::Mathematics::Vector2<float>(-159.0f, -42.0f));
		renderer->Draw(point);
	}

	rectangle.setOutlineColor(sf::Color::Blue);
	rectangle.setFillColor(sf::Color::Transparent);
	rectangle.setOutlineThickness(2.0f);

	renderer->Draw(rectangle);
}
