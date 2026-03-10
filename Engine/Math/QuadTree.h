#pragma once
#include "Node.h"

namespace Wanted
{
	class WANTED_API QuadTree
	{
	public:

		QuadTree(const Bounds& bounds);
		~QuadTree();

		void Insert(Actor* actor);

		std::vector<Actor*> Query(Bounds& range);

		void Draw();
		//std::vector<Bounds> GetPlayerBounds(const Bounds& bounds);

		std::vector<Actor*> GetPlayerBullet(const Bounds& target);
	public:
		static const int maxDepth = 3;
	private:
		Node* root = nullptr;

	};
}


