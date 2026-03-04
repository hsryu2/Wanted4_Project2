#include "QuadTree.h"


namespace Wanted
{
	QuadTree::QuadTree(const Bounds& bounds)
	{
		root = new Node(bounds);
	}
	QuadTree::~QuadTree()
	{
		SafeDelete(root);
	}

	void QuadTree::Insert(Node* node)
	{
		if (!node)
		{
			return;
		}

		root->Insert(node);
	}

	std::vector<Node*> QuadTree::Query(Node* QueryNode)
	{
		if (!QueryNode)
		{
			return { };
		}

		std::vector<Node*> possibleNodes;
		root->Query(QueryNode->GetBounds(), possibleNodes);

		std::vector<Node*> intersects;
		for (Node* const node : possibleNodes)
		{
			for (Node* const point : node->Points())
			{
				if (point->GetBounds().Intersects(QueryNode->GetBounds()))
				{
					intersects.emplace_back(point);
					continue;
				}
			}
		}
		return intersects;

	}
}
