#include "Node.h"

namespace Wanted
{
	Node::Node(const Bounds& bounds, int depth)
		:bounds(bounds), depth(depth)
	{
	}

	Node::~Node()
	{
		Clear();
	}

	void Node::Insert(Actor* actor)
	{
		if (!actor)
		{
			return;
		}
		// 겹치는 영역 확인.
		NodeIndex result = testRegion(actor->GetBounds());

		if (result == NodeIndex::OutOfArea)
		{
			return;
		}

		if (IsDivided())
		{
			if (result == NodeIndex::TopLeft)
			{
				topLeft->Insert(actor);
			}
			if (result == NodeIndex::TopRight)
			{
				topRight->Insert(actor);
			}
			if (result == NodeIndex::BottomLeft)
			{
				bottomLeft->Insert(actor);
			}
			if (result == NodeIndex::BottomRight)
			{
				bottomRight->Insert(actor);
			}
		}
		// 영역 밖이 아닌 객체를 일단 배열에 넣는다.
		points.emplace_back(actor);
		
		// 배열의 사이즈가 분할 조건에 맞는지 확인.
		if (points.size() > capacity && !IsDivided())
		{

		}

	}

	void Node::Query(const Bounds& bounds, std::vector<Actor*>& possibleActors)
	{
		for (Actor* actor : points)
		{
			possibleActors.emplace_back(actor);
		}

		if (!IsDivided())
		{
			return;
		}

		std::vector<NodeIndex> quads = GetQuads(bounds);

		for (const NodeIndex& index : quads)
		{
			if (index == NodeIndex::TopLeft)
			{
				topLeft->Query(bounds, possibleActors);
			}
			else if (index == NodeIndex::TopRight)
			{
				topRight->Query(bounds, possibleActors);
			}
			else if (index == NodeIndex::BottomLeft)
			{
				bottomLeft->Query(bounds, possibleActors);
			}
			else if (index == NodeIndex::BottomRight)
			{
				bottomRight->Query(bounds, possibleActors);
			}
		}
	}

	void Node::Clear()
	{
		points.clear();

		if (IsDivided())
		{
			topLeft->Clear();
			topRight->Clear();
			bottomLeft->Clear();
			bottomRight->Clear();

			ClearChildren();
		}
	}

	bool Node::SubDivide()
	{
		// Todo: 나중에 동적으로 만들어야할거같음. 일단 하드코딩.
		if (depth == 5)
		{
			return false;
		}

		if (!IsDivided())
		{
			int x = bounds.X();
			int y = bounds.Y();
			int halfWidth = bounds.Width() / 2;
			int halfHeight = bounds.Height() / 2;

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

	NodeIndex Node::testRegion(const Bounds& bounds)
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

		// 왼쪽이랑 겹치는지 확인.
		bool left = bounds.X() < centerX && bounds.MaxX() >= x;

		// 오른쪽이랑 겹치는지 확인.
		bool right = bounds.MaxX() >= centerX && bounds.X() < this->bounds.MaxX();

		// 위쪽이랑 겹치는지 확인.
		bool top = bounds.Y() < centerY && bounds.MaxY() >= y;

		// 아래쪽이랑 겹치는지 확인.
		bool bottom = bounds.MaxY() >= centerY && bounds.Y() < this->bounds.MaxY();

		if (top && left)
		{
			quads.emplace_back(NodeIndex::TopLeft);
		}
		if (top && right)
		{
			quads.emplace_back(NodeIndex::TopRight);
		}
		if (bottom && left)
		{
			quads.emplace_back(NodeIndex::BottomLeft);
		}
		if (bottom && right)
		{
			quads.emplace_back(NodeIndex::BottomRight);
		}


		return quads;
	}

	void Node::ClearChildren()
	{
		if (IsDivided())
		{
			SafeDelete(topLeft);
			SafeDelete(topRight);
			SafeDelete(bottomLeft);
			SafeDelete(bottomRight);
		}
	}
}


