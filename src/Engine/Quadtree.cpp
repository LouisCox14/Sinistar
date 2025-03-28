#include "Quadtree.h"

void Weave::QuadTree::PushObject(ObjectID object)
{
	std::queue<NodeID> nodesToProcess;
	nodesToProcess.push(root);

	while (!nodesToProcess.empty())
	{
		NodeID currentNodeID = nodesToProcess.front();
		nodesToProcess.pop();

		if (!Physics::Collides(Physics::Collider(nodes[currentNodeID].bounds), objects[object].second)) continue;

		if (!nodes[currentNodeID].isLeaf)
		{
			for (NodeID childID : nodes[currentNodeID].children)
			{
				if (childID != NullNode) nodesToProcess.push(childID);
			}

			continue;
		}

		if (nodes[currentNodeID].objects.size() <= maxObjects || nodes[currentNodeID].layer >= maxLayers)
		{
			nodes[currentNodeID].objects.emplace_back(object);
			continue;
		}

		nodes[currentNodeID].isLeaf = false;

		Weave::Mathematics::Vector2<float> nodeCenter = (nodes[currentNodeID].bounds.min + nodes[currentNodeID].bounds.max) / 2;

		nodes.emplace_back(Node({ nodes[currentNodeID].bounds.min, nodeCenter }, nodes[currentNodeID].layer + 1));
		nodes.emplace_back(Node({ nodeCenter, nodes[currentNodeID].bounds.max }, nodes[currentNodeID].layer + 1));
		nodes.emplace_back(Node({ { nodeCenter.x, nodes[currentNodeID].bounds.min.y }, { nodes[currentNodeID].bounds.max.x, nodeCenter.y } }, nodes[currentNodeID].layer + 1));
		nodes.emplace_back(Node({ { nodes[currentNodeID].bounds.min.x, nodeCenter.y }, { nodeCenter.x, nodes[currentNodeID].bounds.max.y } }, nodes[currentNodeID].layer + 1));

		nodes[currentNodeID].children[0] = nodes.size() - 1;
		nodes[currentNodeID].children[1] = nodes.size() - 2;
		nodes[currentNodeID].children[2] = nodes.size() - 3;
		nodes[currentNodeID].children[3] = nodes.size() - 4;

		for (ObjectID objectID : nodes[currentNodeID].objects)
		{
			for (NodeID childID : nodes[currentNodeID].children)
			{
				if (childID != NullNode && Physics::Collides(Physics::Collider(nodes[childID].bounds), objects[objectID].second))
					nodes[childID].objects.emplace_back(objectID);
			}
		}

		nodes[currentNodeID].objects.clear();

		nodesToProcess.push(currentNodeID);
	}
}

void Weave::QuadTree::QueryNode(Physics::Collider queryCollider, NodeID nodeID, std::unordered_set<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>, ObjectHash, ObjectEqual>& queryConsiderations)
{
	const Node& node = nodes[nodeID];

	if (!Physics::Collides(Physics::Collider(node.bounds), queryCollider))
		return;

	for (ObjectID objectID : node.objects)
	{
		const auto& [entity, collider] = objects[objectID];
		if (Physics::Collides(queryCollider, collider))
		{
			queryConsiderations.insert({ entity, collider });
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

Weave::QuadTree::QuadTree(Shapes::Rectangle bounds, std::uint16_t _maxObjects, std::uint16_t _maxLayers) : maxObjects(_maxObjects), root(0),  maxLayers(_maxLayers)
{
	nodes.push_back(Node(bounds, 0));
}

void Weave::QuadTree::Insert(ECS::EntityID entity, Physics::Collider collider)
{
	ObjectID newObject = objects.size();
	objects.push_back({ entity, collider });

	PushObject(newObject);
}

void Weave::QuadTree::Reserve(std::size_t space)
{
	objects.reserve(space);
	nodes.reserve(space / maxObjects);
}

std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> Weave::QuadTree::Query(Physics::Collider queryCollider)
{
	std::unordered_set<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>, ObjectHash, ObjectEqual> queryConsiderations;

	QueryNode(queryCollider, root, queryConsiderations);

	return std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>>(queryConsiderations.begin(), queryConsiderations.end());
}

std::vector<Weave::QuadTree::NodeID> Weave::QuadTree::GetAllLeafNodes() const
{
    std::vector<NodeID> leafNodes;
    std::queue<NodeID> nodesToProcess;
    nodesToProcess.push(root);

    while (!nodesToProcess.empty())
    {
        NodeID currentNodeID = nodesToProcess.front();
        nodesToProcess.pop();

        const Node& currentNode = nodes[currentNodeID];

        if (currentNode.isLeaf)
        {
            leafNodes.push_back(currentNodeID);
        }
        else
        {
            for (NodeID childID : currentNode.children)
            {
                if (childID != NullNode)
                {
                    nodesToProcess.push(childID);
                }
            }
        }
    }

    return leafNodes;
}

std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>> Weave::QuadTree::GetLeafObjects(NodeID leafNode)
{
	Node& node = nodes[leafNode];

	if (!node.isLeaf) return std::vector<std::pair<Weave::ECS::EntityID, Weave::Physics::Collider>>();

	std::vector<std::pair<ECS::EntityID, Physics::Collider>> returnObjects;
	returnObjects.reserve(node.objects.size());

	for (ObjectID id : node.objects)
		returnObjects.emplace_back(objects[id]);

	return returnObjects;
}

Weave::Node::Node(Shapes::Rectangle _bounds, std::uint16_t _layer) : bounds(_bounds), layer(_layer) { }