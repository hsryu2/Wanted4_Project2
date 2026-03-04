#pragma once
#include <iostream>
#include <vector>
#include "../Actor/Actor.h"
#include "Bounds.h"
#include "Util/Util.h"


namespace Wanted
{
	enum class NodeIndex
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Stradding,
		OutOfArea
	};

	class Node
	{

	public:
		Node(const Bounds& bounds, int depth = 0);
		~Node();

		void Insert(Node* node);

		void Query(const Bounds& bounds, std::vector<Node*>& possibleNodes);

		void Clear();

		inline const Bounds& GetBounds() { return bounds; }
		inline const std::vector<Node*>& Points() { return points; }

		inline Node* TopLeft() const { return topLeft; }
		inline Node* TopRight() const { return topRight; }
		inline Node* BottomLeft() const { return bottomLeft; }
		inline Node* BottomRight() const { return bottomRight; }

	private:
		bool SubDivide();

		bool IsDivided();

		Bounds bounds;
		int depth = 0;

		NodeIndex testRigion(const Bounds& bounds);

		std::vector<NodeIndex> GetQuads(const Bounds& bounds);

		void ClearChildren();

		std::vector<Node*> points;

		Node* topLeft = nullptr;
		Node* topRight = nullptr;
		Node* bottomLeft = nullptr;
		Node* bottomRight = nullptr;

	};
}
