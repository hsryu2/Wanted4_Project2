#pragma once
#include "Node.h"

namespace Wanted
{
	class QuadTree
	{
	public:
		QuadTree(const Bounds& bounds);
		~QuadTree();

		void Insert(Node* node);

		std::vector<Node*> Query(Node* QueryNode);


	public:
		// Todo: depth 동적으로 나중에 바꿔야함.
		static const int maxDepth = 5;
	private:
		Node* root = nullptr;

	};
}


