#include "Node.h"

namespace Wanted
{
	Node::Node(const Bounds& bounds, int depth)
		:bounds(bounds), depth(depth)
	{
	}

	Node::~Node()
	{
		
	}

	void Node::Insert(Node* node)
	{
		if (!node)
		{
			return;
		}
		// 겹치는 영역 확인.
		NodeIndex result = testRigion(node->GetBounds());

		if (result == NodeIndex::Stradding)
		{
			points.emplace_back(node);
		}

		else if (result != NodeIndex::OutOfArea)
		{
			if (SubDivide())
			{
				if (result == NodeIndex::TopLeft)
				{
					topLeft->Insert(node);
				}
			}
		}
	}

	void Node::Query(const Bounds& bounds, std::vector<Node*>& possibleNodes)
	{
	}

	void Node::Clear()
	{
		points.clear();
	}

	bool Node::SubDivide()
	{
		// Todo: 나중에 동적으로 만들어야할거같음. 일단 하드코딩.
		if (depth == 5)
		{
			return false;
		}

		if (!IsDivided)
		{
			int x = bounds.X();
			int y = bounds.Y();
			int halfWidth = bounds.Width() / 2;
			int halfHeight = bounds.Width() / 2;

			topLeft = new Node(Bounds(x, y, halfWidth, halfHeight), depth + 1);
			topRight = new Node(Bounds(x + halfWidth, y, halfWidth, halfHeight), depth + 1);
			bottomLeft = new Node(Bounds(x, y + halfHeight, halfWidth, halfHeight), depth + 1);
			bottomRight = new Node(Bounds(x + halfWidth, y + halfHeight, halfWidth, halfHeight), depth + 1);

		}
		return true;
	}

	bool Node::IsDivided()
	{
		return topLeft != nullptr;
	}

	NodeIndex Node::testRigion(const Bounds& bounds)
	{
		std::vector<NodeIndex> quads = GetQuads(bounds);

		if (quads.empty())
		{
			return NodeIndex::OutOfArea;
		}
		if (quads.size() == 1)
		{
			return quads[0];
		}

		return NodeIndex::Stradding;
	}

	std::vector<NodeIndex> Node::GetQuads(const Bounds& bounds)
	{
		std::vector<NodeIndex> quads;

		int x = this->bounds.X();
		int y = this->bounds.Y();
		int halfWidth = this->bounds.Width() / 2;
		int halfHeight = this->bounds.Height() / 2;
		int centerX = x + halfWidth;
		int centerY = y + halfHeight;

		bool left = bounds.X() < centerX && 
		


		return std::vector<NodeIndex>();
	}

	void Node::ClearChildren()
	{
	}
}


