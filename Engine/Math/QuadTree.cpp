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

	void QuadTree::Insert(Actor* node)
	{
		if (!node)
		{
			return;
		}

		root->Insert(node);
	}

	std::vector<Actor*> QuadTree::Query(Actor* targetActor)
	{
		if (!targetActor)
		{
			return { };
		}

		std::vector<Actor*> possibleActors;
		root->Query(targetActor->GetBounds(), possibleActors);

		std::vector<Actor*> intersects;
		for (Actor* actor : possibleActors)
		{
			if (actor == targetActor) continue;

			if (actor->GetBounds().Intersects(targetActor->GetBounds()))
			{
				intersects.emplace_back(actor);
			}
		}
		return intersects;

	}
}
