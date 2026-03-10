#include "Node.h"

#include "Render/Renderer.h"


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

	// 삽입.
	void Node::Insert(Actor* actor)
	{
		if (!actor)
		{
			return;
		}

		// 영역 밖인지 확인.
		NodeIndex result = testRegion(actor->GetBounds());

		if (result == NodeIndex::OutOfArea)
		{
			return;
		}

		// 해당 액터가 어느 영역에 포함되고 있는지 확인.
		std::vector<NodeIndex> quads = GetQuads(actor->GetBounds());

		// 이미 분할된 면인지 확인.
		// 다중 삽입으로 해당 노드가 경계선에 걸쳐있으면 양쪽 다 넣어줌.
		if (IsDivided())
		{
			for (const NodeIndex& index : quads)
			{
				if (result == NodeIndex::TopLeft)
				{
					topLeft->Insert(actor);
				}
				else if (result == NodeIndex::TopRight)
				{
					topRight->Insert(actor);
				}
				else if (result == NodeIndex::BottomLeft)
				{
					bottomLeft->Insert(actor);
				}
				else if (result == NodeIndex::BottomRight)
				{
					bottomRight->Insert(actor);
				}
				return;
			}

		}

		// 영역 밖이 아닌 객체를 일단 배열에 넣는다.
		points.emplace_back(actor);
		
		// 배열의 사이즈가 분할 조건에 맞는지 확인.
		// depth의 최대치보다 작고 한 분할면에 최대 객체 수를 지정하여 넘으면 분할한다.
		if (points.size() > capacity && !IsDivided())
		{	
			if (SubDivide())
			{
				// 경계선에 걸쳐 분리되지 못하는 객체들을 모을 배열.
				std::vector<Actor*> straddingActors;

				// 분할 조건 OK 분할 시킬 객체들을 옮겨주기.
				for (Actor* p : points)
				{
					NodeIndex pResult = testRegion(p->GetBounds());

					if (pResult == NodeIndex::TopLeft)
					{
						topLeft->Insert(p);
					}
					else if (pResult == NodeIndex::TopRight)
					{
						topRight->Insert(p);
					}
					else if (pResult == NodeIndex::BottomLeft)
					{
						bottomLeft->Insert(p);
					}
					else if (pResult == NodeIndex::BottomRight)
					{
						bottomRight->Insert(p);
					}
					else
					{
						// 경계선에 걸친 애들을 배열에 삽입.
						straddingActors.emplace_back(p);
					}
				}
				// 경계선에 걸친 애들로 교체.
				points = straddingActors;
			}
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

	// 플레이어 영역 가져오기. Quary랑 비슷함.
	void Node::GetPlayerBound(const Bounds& targetBounds, std::vector<Bounds>& outBounds)
	{
		if (IsDivided())
		{
			if (bounds.Intersects(targetBounds))
			{
				outBounds.emplace_back(targetBounds);
			}
			return;
		}
		else
		{
			std::vector<NodeIndex> quads = GetQuads(bounds);

			for (const NodeIndex& index : quads)
			{
				if (index == NodeIndex::TopLeft)
				{
					topLeft->GetPlayerBound(targetBounds, outBounds);
				}
				else if (index == NodeIndex::TopRight)
				{
					topRight->GetPlayerBound(targetBounds, outBounds);
				}
				else if (index == NodeIndex::BottomLeft)
				{
					bottomLeft->GetPlayerBound(targetBounds, outBounds);
				}
				else if (index == NodeIndex::BottomRight)
				{
					bottomRight->GetPlayerBound(targetBounds, outBounds);
				}
			}
		}
	}



	bool Node::SubDivide()
	{
		if (depth == maxDepth)
		{
			return false;
		}

		if (!IsDivided())
		{
			int x = bounds.X();
			int y = bounds.Y();
			int halfWidth = bounds.Width() / 2;
			int halfHeight = bounds.Height() / 2;

			topLeft = new Node(Bounds(x, y, halfWidth, halfHeight + 1), depth + 1);
			topRight = new Node(Bounds(x + halfWidth, y, halfWidth, halfHeight + 1), depth + 1);
			bottomLeft = new Node(Bounds(x, y + halfHeight, halfWidth, halfHeight + 1), depth + 1);
			bottomRight = new Node(Bounds(x + halfWidth, y + halfHeight, halfWidth, halfHeight + 1), depth + 1);

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
	
	// 분할선 그리기.
	void Node::Draw()
	{
		// 분할됐는지 확인하고 선을 그린다.
		if (IsDivided())
		{
			int startX = bounds.X();
			int startY = bounds.Y();
			int width = bounds.Width();
			int height = bounds.Height();

			int midX = startX + width / 2;
			int midY = startY + height / 2;

			// x,y좌표, color, sortingOrder
			for (int i = startX+1; i < startX + width; i++)
			{
				Renderer::Get().Submit("-", Vector2(i, midY), Color::Green , 8);
			}
			
			for (int i = startY; i < startY + height; i++)
			{
				Renderer::Get().Submit("|", Vector2(midX, i), Color::Green, 8);
			}

			Renderer::Get().Submit("+", Vector2(midX, midY), Color::Green, 9);

			topLeft->Draw();
			topRight->Draw();
			bottomLeft->Draw();
			bottomRight->Draw();
		}
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


