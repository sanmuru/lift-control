#pragma once

#include <assert.h>
#include <queue>
#include <set>

#ifdef LIFTCONTROL_EXPORTS
#define LIFTCONTROL_API __declspec(dllexport)
#else
#define LIFTCONTROL_API __declspec(dllimport)
#endif

using namespace std;

namespace LiftControl
{
	enum Direction { UP = 1, DOWN = -1 };

	static Direction GetDirection(int from_floor, int at_floor)
	{
		assert(from_floor != at_floor);
		return from_floor < at_floor ? Direction::UP : Direction::DOWN;
	}

	struct FloorComparer
	{
		explicit FloorComparer(Direction d) : m_direction(d) {}

		bool operator()(const int& i1, const int& i2) const
		{
			if (Direction::UP == this->m_direction)
			{
				return i1 < i2;
			}
			else
			{
				return i2 < i1;
			}
		}

		FloorComparer reverse() const
		{
			return FloorComparer((Direction)-(int)this->m_direction);
		}

	private:
		const Direction m_direction;
	};

	struct LiftTask
	{
		const int floor;
		const Direction direction;
	};

	class LIFTCONTROL_API Lift
	{
	public:
		explicit Lift(int atFloor) : m_atFloor(atFloor),
			m_direction((Direction)0),
			m_pass1(nullptr),
			m_pass2(nullptr),
			m_pass3(nullptr)
		{}
		~Lift();

		void add(int from_floor, int to_floor);

#if _DEBUG
		int get_at_floor() { return this->m_atFloor; }
		vector<LiftTask> get_schedule();
#endif

	private:
		int m_atFloor;
		Direction m_direction;
		set<int, FloorComparer>* m_pass1;
		set<int, FloorComparer>* m_pass2;
		set<int, FloorComparer>* m_pass3;

		void initialize(int from_floor, int to_floor);
	};
}