#pragma once

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

	enum Direction { Up = 1, Down = -1 };

	struct FloorComparer
	{
	public:
		explicit FloorComparer(Direction d) : m_direction(d) {}

		bool operator()(const int& i1, const int& i2) const
		{
			if (Direction::Up == this->m_direction)
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
		Direction m_direction;
	};

	struct LiftTask
	{
	public:
		int floor;
		Direction direction;
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

		void Add(int fromFloor, int toFloor);

#if _DEBUG
		int AtFloor() { return this->m_atFloor; }
		vector<LiftTask> GetSchedule();
#endif

	private:
		int m_atFloor;
		Direction m_direction;
		set<int, FloorComparer>* m_pass1;
		set<int, FloorComparer>* m_pass2;
		set<int, FloorComparer>* m_pass3;

		void Initialize(int fromFloor, int toFloor);
	};
}