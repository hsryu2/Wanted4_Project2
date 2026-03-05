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


	public:
		// Todo: depth 동적으로 나중에 바꿔야함.
		static const int maxDepth = 4;
	private:
		Node* root = nullptr;

	};
}


