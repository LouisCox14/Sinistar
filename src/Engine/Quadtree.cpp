#include "Quadtree.h"

void Weave::QuadTree::PushObject(ObjectID object)
{
	std::queue<NodeID> nodesToProcess;
	nodesToProcess.push(root);

	while (!nodesToProcess.empty())
	{
		NodeID currentNodeID = nodesToProcess.front();
		nodesToProcess.pop();

		if (!Physics::Collides(objects[object].second, Physics::Collider(nodes[currentNodeID].bounds))) continue;

		if (!nodes[currentNodeID].isLeaf)
		{
			for (NodeID childID : nodes[currentNodeID].children)
			{
				if (childID != NullNode) nodesToProcess.push(childID);
			}

			continue;
		}

		if (nodes[currentNodeID].objects.size() <= maxObjects)
		{
			nodes[currentNodeID].objects.emplace_back(object);
			continue;
		}

		nodes[currentNodeID].isLeaf = false;

		Weave::Mathematics::Vector2<float> nodeCenter = (nodes[currentNodeID].bounds.min + nodes[currentNodeID].bounds.max) / 2;

		nodes.emplace_back(Node({ nodes[currentNodeID].bounds.min, nodeCenter }));
		nodes.emplace_back(Node({ nodeCenter, nodes[currentNodeID].bounds.max }));
		nodes.emplace_back(Node({ { nodeCenter.x, nodes[currentNodeID].bounds.min.y }, { nodes[currentNodeID].bounds.max.x, nodeCenter.y } }));
		nodes.emplace_back(Node({ { nodes[currentNodeID].bounds.min.x, nodeCenter.y }, { nodeCenter.x, nodes[currentNodeID].bounds.max.y } }));

		nodes[currentNodeID].children[0] = nodes.size() - 1;
		nodes[currentNodeID].children[1] = nodes.size() - 2;
		nodes[currentNodeID].children[2] = nodes.size() - 3;
		nodes[currentNodeID].children[3] = nodes.size() - 4;

		for (ObjectID objectID : nodes[currentNodeID].objects)
		{
			for (NodeID childID : nodes[currentNodeID].children)
			{
				if (childID != NullNode && Physics::Collides(objects[objectID].second, Physics::Collider(nodes[childID].bounds)))
					nodes[childID].objects.emplace_back(objectID);
			}
		}

		nodes[currentNodeID].objects.clear();

		nodesToProcess.push(currentNodeID);
	}
}

void Weave::QuadTree::QueryNode(Physics::Collider queryCollider, NodeID nodeID, std::vector<std::pair<ECS::EntityID, Physics::Collider>>& queryConsiderations)
{
	const Node& node = nodes[nodeID];

	if (!Physics::Collides(queryCollider, Physics::Collider(node.bounds)))
		return;

	for (ObjectID objectID : node.objects)
	{
		const auto& [entity, collider] = objects[objectID];
		if (Physics::Collides(queryCollider, collider))
		{
			queryConsiderations.emplace_back(entity, collider);
		}
	}

	if (!node.isLeaf)
	{
		for (NodeID childID : node.children)
		{
			if (childID != NullNode)
			{
				QueryNode(queryCollider, childID, queryConsiderations);
			}
		}
	}
}

Weave::QuadTree::QuadTree(Shapes::Rectangle bounds, std::uint16_t _maxObjects) : maxObjects(_maxObjects), root(0) 
{
	nodes.push_back(Node(bounds));
}

void Weave::QuadTree::Insert(ECS::EntityID entity, Physics::Collider collider)
{
	ObjectID newObject = objects.size();
	objects.push_back({ entity, collider });

	PushObject(newObject);
}

std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> Weave::QuadTree::Query(Physics::Collider queryCollider)
{
	std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> queryConsiderations; // TODO: Update to use a set to avoid duplicate collisions. Remember, entities appear in all leaves they intersect.

	QueryNode(queryCollider, root, queryConsiderations);

	return queryConsiderations;
}

Weave::Node::Node(Shapes::Rectangle _bounds) : bounds(_bounds) { }