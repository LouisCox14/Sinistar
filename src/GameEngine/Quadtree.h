#pragma once
#include "ECS.h"
#include "Collisions.h"
#include <vector>
#include <queue>
#include <unordered_set>

namespace Weave
{
	struct ObjectHash
	{
		std::size_t operator()(const std::pair<ECS::EntityID, Physics::Collider>& obj) const
		{
			return std::hash<ECS::EntityID>{}(obj.first);
		}
	};

	struct ObjectEqual
	{
		bool operator()(const std::pair<ECS::EntityID, Physics::Collider>& lhs,
			const std::pair<ECS::EntityID, Physics::Collider>& rhs) const
		{
			return lhs.first == rhs.first;
		}
	};

	struct Node
	{
	private:
		using NodeID = std::size_t;
		using ObjectID = std::size_t;

		static const NodeID NullNode = SIZE_MAX;

	public:
		bool isLeaf = true;

		Shapes::Rectangle bounds;
		std::uint16_t layer;

		std::vector<ObjectID> objects;
		NodeID children[4] {NullNode, NullNode, NullNode, NullNode};

		Node(Shapes::Rectangle bounds, std::uint16_t layer);
	};

	class QuadTree
	{
	public:
		using NodeID = std::size_t;
		using ObjectID = std::size_t;

		static const NodeID NullNode = SIZE_MAX;

	private:
		NodeID root;
		std::vector<Node> nodes;
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> objects;

		const std::uint16_t maxObjects;
		const std::uint16_t maxLayers;

		void PushObject(ObjectID object);
		void QueryNode(Physics::Collider queryCollider, NodeID nodeID, std::unordered_set<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>, ObjectHash, ObjectEqual>& queryConsiderations);

	public:
		QuadTree(Shapes::Rectangle bounds, std::uint16_t maxObjects, std::uint16_t maxLayers);
		
		void Insert(ECS::EntityID entity, Physics::Collider collider);
		void Reserve(std::size_t space);

		std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> Query(Physics::Collider queryCollider);
		std::vector<NodeID> GetAllLeafNodes() const;
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> GetLeafObjects(NodeID leafNode);
	};
}