#pragma once
#include "World.h"

Weave::ECS::EntityID Weave::ECS::World::CreateEntity()
{
	if (!availableEntityIDs.empty())
		return availableEntityIDs.extract(availableEntityIDs.begin()).value();

	return nextEntityID++;
}

void Weave::ECS::World::DeleteEntity(EntityID entity)
{
	if (!IsEntityRegistered(entity))
		throw std::logic_error("Entity is not registered.");

	for (auto& pair : archetypes)
	{
		pair.second.get()->RemoveEntity(entity);
	}

	availableEntityIDs.insert(entity);
}

bool Weave::ECS::World::IsEntityRegistered(Weave::ECS::EntityID entity)
{
	if (entity > nextEntityID) return false;
	if (availableEntityIDs.find(entity) != availableEntityIDs.end()) return false;

	return true;
}

Weave::ECS::SystemGroupID Weave::ECS::World::CreateSystemGroup()
{
	if (!availableSystemGroupIDs.empty())
		return availableSystemGroupIDs.extract(availableSystemGroupIDs.begin()).value();

	systemGroups.resize(nextSystemGroupID + 1);
	return nextSystemGroupID++;
}

void Weave::ECS::World::RetireSystemGroup(Weave::ECS::SystemGroupID systemGroup)
{
	if (!IsSystemGroupRegistered(systemGroup)) return;

	systemGroups[systemGroup].clear();
	availableSystemGroupIDs.insert(systemGroup);
}

bool Weave::ECS::World::IsSystemGroupRegistered(Weave::ECS::SystemGroupID systemGroup)
{
	if (systemGroup > nextSystemGroupID) return false;
	if (availableSystemGroupIDs.find(systemGroup) != availableSystemGroupIDs.end()) return false;

	return true;
}

void Weave::ECS::World::RegisterSystem(std::function<void(World&)> function, Weave::ECS::SystemGroupID systemGroup)
{
	if (!IsSystemGroupRegistered(systemGroup)) throw std::logic_error("System group does not exist.");

	systemGroups[systemGroup].insert(System(function));
}

void Weave::ECS::World::DeregisterSystem(std::function<void(World&)> function, Weave::ECS::SystemGroupID systemGroup)
{
	if (!IsSystemGroupRegistered(systemGroup)) return;

	systemGroups[systemGroup].erase(System(function));
}


void Weave::ECS::World::CallSystemGroup(Weave::ECS::SystemGroupID systemGroup)
{
	if (!IsSystemGroupRegistered(systemGroup)) return;

	for (System system : systemGroups[systemGroup])
	{
		system(*this);
	}
}