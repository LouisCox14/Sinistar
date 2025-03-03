#pragma once
#include "ECS.h"
#include "Renderer.h"
#include "Events.h"
#include "InputHandler.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace Weave
{
	class Engine
	{
	private:
		ECS::World world;

		sf::RenderWindow window;
		Graphics::Renderer renderer;
		Input::InputHandler inputHandler;

	public:
		Engine(std::string windowName);
		~Engine();

		const ECS::SystemGroupID updateGroup;

		ECS::World& GetWorld();
		Graphics::Renderer& GetRenderer();
		Input::InputHandler& GetInputHandler();

		void Run();
	};
}