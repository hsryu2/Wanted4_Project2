#pragma once
#include "Node.h"

namespace Wanted
{
	class QuadTree
	{
	public:
		QuadTree(const Bounds& bounds);
		~QuadTree();

		void Insert(Actor* actor);

		std::vector<Actor*> Query(Actor* targetActor);


	public:
		// Todo: depth 동적으로 나중에 바꿔야함.
		static const int maxDepth = 5;
	private:
		Node* root = nullptr;

	};
}


