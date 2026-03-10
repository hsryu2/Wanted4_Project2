#pragma once
#include <iostream>
#include <vector>
#include "../Actor/Actor.h"
#include "Bounds.h"
#include "Util/Util.h"
#include "../Game/Actor/Player.h"

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

	class WANTED_API Node
	{

	public:
		Node(const Bounds& bounds, int depth = 0);
		~Node();

		void Insert(Actor* actor);

		void Query(const Bounds& bounds, std::vector<Actor*>& possibleActors);

		void Clear();

		void GetPlayerBound(const Bounds& targetBounds, std::vector<Bounds>& outBounds);
		

		inline const Bounds& GetBounds() { return bounds; }
		inline const std::vector<Actor*>& Points() { return points; }

		inline Node* TopLeft() const { return topLeft; }
		inline Node* TopRight() const { return topRight; }
		inline Node* BottomLeft() const { return bottomLeft; }
		inline Node* BottomRight() const { return bottomRight; }

		void Draw();



	private:
		bool SubDivide();

		bool IsDivided();


		Bounds bounds;
		int depth = 0;
		const int maxDepth = 3;

		// 분할면 안에 객체가 6개 있으면 분할.
		int capacity = 6;

		NodeIndex testRegion(const Bounds& bounds);

		std::vector<NodeIndex> GetQuads(const Bounds& bounds);

		void ClearChildren();

		std::vector<Actor*> points;

		Node* topLeft = nullptr;
		Node* topRight = nullptr;
		Node* bottomLeft = nullptr;
		Node* bottomRight = nullptr;



	};
}
