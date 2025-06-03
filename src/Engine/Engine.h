#pragma once
#include "ECS.h"
#include "Renderer.h"
#include "Animator.h"
#include "Events.h"
#include "InputHandler.h"
#include "Time.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

namespace Weave
{
	using SystemGroupID = std::size_t;
	using SystemGroup = Utilities::Event<ECS::World&>;

	class Engine
	{
	private:
		ECS::World world;

		sf::RenderWindow window;
		Graphics::Renderer renderer;
		Input::InputHandler inputHandler;

		const float fixedUpdateInterval = 0.02f;

		std::vector<SystemGroup> systemGroups;

		SystemGroupID nextSystemGroupID = 0;
		std::set<SystemGroupID> availableSystemGroupIDs;

	public:
		Engine(std::string windowName);
		~Engine();

		const SystemGroupID updateGroup;
		const SystemGroupID lateUpdateGroup;
		const SystemGroupID fixedUpdateGroup;
		const SystemGroupID lateFixedUpdateGroup;
		const SystemGroupID worldRenderGroup;
		const SystemGroupID uiRenderGroup;

		ECS::World& GetWorld();
		Graphics::Renderer& GetRenderer();
		Input::InputHandler& GetInputHandler();

		SystemGroupID CreateSystemGroup();
		void RetireSystemGroup(SystemGroupID systemGroup);
		bool IsSystemGroupRegistered(SystemGroupID systemGroup);

		SystemGroup& GetSystemGroup(SystemGroupID groupID);
		void CallSystemGroup(SystemGroupID groupID);

		void Run();
	};
}