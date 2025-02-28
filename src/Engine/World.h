#pragma once
#include "Archetype.h"
#include <unordered_map>
#include <set>
#include <typeindex>
#include <memory>
#include <stdexcept>
#include <map>
#include <algorithm>
#include <iterator>
#include <utility>
#include <iostream>
#include <functional>

namespace Weave
{
	namespace ECS
	{
        template <typename... Components>
        class WorldView
        {
        private:
            std::vector<ArchetypeView<Components...>> archetypeViews;

        public:
            WorldView(std::vector<ArchetypeView<Components...>> views)
                : archetypeViews(std::move(views)) {}

            class Iterator
            {
            private:
                using ArchetypeIterator = typename ArchetypeView<Components...>::Iterator;
                std::vector<ArchetypeView<Components...>>* archetypeViews;
                size_t currentArchetypeIndex;
                ArchetypeIterator currentIterator;

                void AdvanceToNextValidArchetype()
                {
                    while (currentArchetypeIndex < archetypeViews->size() && currentIterator == (*archetypeViews)[currentArchetypeIndex].end())
                    {
                        ++currentArchetypeIndex;
                        if (currentArchetypeIndex < archetypeViews->size())
                        {
                            currentIterator = (*archetypeViews)[currentArchetypeIndex].begin();
                        }
                    }
                }

            public:
                Iterator(std::vector<ArchetypeView<Components...>>* views, size_t archetypeIndex, const ArchetypeIterator& iterator)
                    : archetypeViews(views), currentArchetypeIndex(archetypeIndex), currentIterator(iterator)
                {
                    AdvanceToNextValidArchetype();
                }

                Iterator& operator++()
                {
                    ++currentIterator;
                    AdvanceToNextValidArchetype();
                    return *this;
                }

                bool operator!=(const Iterator& other) const
                {
                    return currentArchetypeIndex != other.currentArchetypeIndex || currentIterator != other.currentIterator;
                }

                auto operator*()
                {
                    return *currentIterator;
                }
            };

            Iterator begin()
            {
                if (archetypeViews.empty())
                {
                    return end();
                }

                return Iterator(&archetypeViews, 0, archetypeViews[0].begin());
            }

            Iterator end()
            {
                return Iterator(&archetypeViews, archetypeViews.size(), archetypeViews.back().end());
            }
        };

        using SystemGroupID = std::size_t;

        class World;

        struct System
        {
        protected:
            std::size_t hash;
            std::function<void(World&)> function;

        public:
            System(std::function<void(World&)> func) : function(std::move(func)) 
            {
                typedef void(fnType)(World&);
                fnType** fnPointer = function.template target<fnType*>();

                if (fnPointer)
                {
                    hash = (size_t)*fnPointer;
                    return;
                }

                hash = function.target_type().hash_code();
            };

            bool operator==(const System& other) { return hash == other.hash; }
            bool operator!=(const System& other) { return hash != other.hash; }
            bool operator<(const System& other) const { return hash < other.hash; }
            constexpr std::size_t hash_code() const throw() { return hash; }

            void operator()(World& world) { function(world); }
        };

		class World
		{
		private:
			EntityID nextEntityID = 0;
			std::set<EntityID> availableEntityIDs;

            std::map<EntityID, Archetype*> entityToArchetype;
            std::map<std::type_index, std::set<Archetype*>> componentToArchetypes;
            std::map<std::set<std::type_index>, std::unique_ptr<Archetype>> archetypes;

            std::map<std::type_index, std::size_t> componentSizes;

            std::vector<std::set<System>> systemGroups;

            SystemGroupID nextSystemGroupID = 0;
            std::set<SystemGroupID> availableSystemGroupIDs;

            template <typename... Components>
            Archetype& GetArchetype()
            {
                std::set<ComponentData> typeSet;

                (typeSet.insert({ typeid(Components), sizeof(Components) }), ...);

                return GetArchetype(typeSet);
            }

            Archetype& GetArchetype(const std::set<ComponentData> dataSet)
            {
                std::set<std::type_index> typeSet;

                for (ComponentData data : dataSet)
                {
                    typeSet.insert(data.index);
                }

                auto it = archetypes.find(typeSet);

                if (it == archetypes.end()) 
                {
                    auto archetype = std::make_unique<Archetype>(dataSet);
                    archetypes.emplace(typeSet, std::move(archetype));
                    Archetype* archetypePtr = archetypes[typeSet].get();

                    for (const auto& type : typeSet) {
                        componentToArchetypes[type].insert(archetypePtr);
                    }

                    return *archetypePtr;
                }
                return *it->second.get();
            }

            template <typename... Components>
            void TransferEntity(EntityID entity, Archetype* newArchetype, Archetype* oldArchetype, Components&&... newComponents)
            {
                ((std::cout << " " << typeid(Components).name()), ...);
                std::cout << "\n";

                if (!oldArchetype)
                {
                    newArchetype->AddEntity(entity, std::forward<Components>(newComponents)...);
                    entityToArchetype[entity] = newArchetype;
                    return;
                }

                std::vector<std::type_index> oldComponentTypes;
                for (const auto& component : oldArchetype->GetComponentTypes())
                {
                    oldComponentTypes.push_back(component);
                }

                newArchetype->AddEntity(entity, std::forward<Components>(newComponents)...);

                std::size_t newEntityIndex = newArchetype->GetEntityVector().size() - 1;

                for (std::type_index typeIndex : oldArchetype->GetComponentTypes())
                {
                    std::size_t typeSize = componentSizes[typeIndex];

                    void* oldComponentPtr = oldArchetype->GetComponent(entity, typeIndex);
                    void* newComponentPtr = static_cast<std::vector<std::byte>*>(newArchetype->GetComponentStore(typeIndex, typeSize).data)->data() + newEntityIndex * typeSize;

                    std::memcpy(newComponentPtr, oldComponentPtr, typeSize);

                }

                oldArchetype->RemoveEntity(entity);
                entityToArchetype[entity] = newArchetype;
            }

		public:
			EntityID CreateEntity();
			void DeleteEntity(EntityID entity);
			bool IsEntityRegistered(EntityID entity);

            SystemGroupID CreateSystemGroup();
            void RetireSystemGroup(SystemGroupID systemGroup);
            bool IsSystemGroupRegistered(SystemGroupID systemGroup);

            void RegisterSystem(std::function<void(World&)> system, SystemGroupID systemGroup);
            void DeregisterSystem(std::function<void(World&)> system, SystemGroupID systemGroup);
            void CallSystemGroup(SystemGroupID systemGroup);

            template <typename... Components>
            void AddComponents(EntityID entity)
            {
                AddComponents(entity, Components{}...);
            }

            template <typename... Components>
            void AddComponents(EntityID entity, Components... components)
            {
                ((std::cout << typeid(Components).name() << " ", void()), ...);
                std::cout << "\n";

                ((componentSizes[typeid(Components)] = sizeof(Components)), ...);

                std::set<ComponentData> newTypeSet;
                Archetype* oldArchetype = nullptr;

                if (entityToArchetype.contains(entity)) {
                    oldArchetype = entityToArchetype[entity];
                    newTypeSet = oldArchetype->GetComponentData();
                }

                (newTypeSet.insert({ typeid(Components), sizeof(Components)}), ...);

                Archetype* newArchetype = &GetArchetype(newTypeSet);

                TransferEntity(entity, newArchetype, oldArchetype, std::forward<Components>(components)...);
            }

            template <typename... Components>
            void RemoveComponents(EntityID entity)
            {
                if (!entityToArchetype.contains(entity)) return;

                Archetype* oldArchetype = entityToArchetype[entity];
                std::set<std::type_index> newTypeSet = oldArchetype->GetComponentTypes();
                (newTypeSet.erase(typeid(Components)), ...);

                Archetype* newArchetype = GetArchetype(newTypeSet);

                TransferEntity(entity, newArchetype, oldArchetype);
            }

            template <typename... QueryComponents>
            WorldView<QueryComponents...> GetView()
            {
                std::vector<ArchetypeView<QueryComponents...>> views;

                std::set<Archetype*> matchingArchetypes;

                if constexpr (sizeof...(QueryComponents) > 0)
                {
                    matchingArchetypes = componentToArchetypes[typeid(std::tuple_element_t<0, std::tuple<QueryComponents...>>)];
                }

                ([&]<typename T>(T*) {
                    const auto& archetypesForComponent = componentToArchetypes[typeid(T)];
                    std::set<Archetype*> intersection;
                    std::set_intersection(
                        matchingArchetypes.begin(), matchingArchetypes.end(),
                        archetypesForComponent.begin(), archetypesForComponent.end(),
                        std::inserter(intersection, intersection.begin())
                    );
                    matchingArchetypes = std::move(intersection);
                }(static_cast<QueryComponents*>(nullptr)), ...);

                for (Archetype* archetype : matchingArchetypes)
                {
                    std::vector<EntityID>& entityVector = archetype->GetEntityVector();

                    auto componentVectors = std::make_tuple(
                        static_cast<std::vector<QueryComponents>*>(&archetype->GetComponentVector<QueryComponents>())...
                    );

                    views.emplace_back(ArchetypeView<QueryComponents...>(entityVector, *std::get<std::vector<QueryComponents>*>(componentVectors)...));
                }

                return WorldView<QueryComponents...>(std::move(views));
            }
		};
	}
}