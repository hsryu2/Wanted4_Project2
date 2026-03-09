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

	std::vector<Actor*> QuadTree::Query(Bounds& range)
	{
		std::vector<Actor*> possibleActors;
		if (!root)
		{
			return possibleActors;
		}

		
		root->Query(range, possibleActors);

		std::vector<Actor*> intersects;

		for (Actor* actor : possibleActors)
		{
			if (range.Intersects(actor->GetBounds()))
			{
				intersects.emplace_back(actor);
			}
		}
		return intersects;

	}

	void QuadTree::Draw()
	{
		if (root)
		{
			root->Draw();
		}
	}
	//std::vector<Bounds> QuadTree::GetPlayerBounds(const Bounds& bounds)
	//{
	//	std::vector<Bounds> outBounds;
	//
	//	if (root)
	//	{
	//		root->GetPlayerBound(bounds, outBounds);
	//	}
	//
	//	return outBounds;
	//}
}

// 플레이어가 포함된 영역에 있는 총알들의 리스트 반환해주는 함수.
std::vector<Actor*> QuadTree::GetPlayerBullet(const Bounds& target)
{
	std::vector<Actor*> bullets;
	if (root)
	{
		root->Query(target, bullets);
	}

	return bullets;
}