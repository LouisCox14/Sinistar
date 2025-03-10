#pragma once
#include "ECS.h"
#include "Physics.h"
#include <vector>
#include <queue>
#include <set>

namespace Weave
{
	using NodeID = std::size_t;
	using ObjectID = std::size_t;

	static const NodeID NullNode = SIZE_MAX;

	struct Node
	{
		bool isLeaf = true;

		Shapes::Rectangle bounds;

		std::vector<ObjectID> objects;
		NodeID children[4] {NullNode, NullNode, NullNode, NullNode};

		Node(Shapes::Rectangle bounds);
	};

	class QuadTree
	{
	private:
		NodeID root;
		std::vector<Node> nodes;
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> objects;

		const std::uint16_t maxObjects;

		void PushObject(ObjectID object);
		void QueryNode(Physics::Collider queryCollider, NodeID nodeID, std::vector<std::pair<ECS::EntityID, Physics::Collider>>& queryConsiderations);

	public:
		QuadTree(Shapes::Rectangle bounds, std::uint16_t maxObjects);
		
		void Insert(ECS::EntityID entity, Physics::Collider collider);
		std::vector<std::pair<ECS::EntityID, Physics::Collider>> Query(Physics::Collider queryCollider);
	};
}