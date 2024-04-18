#include "pch.h"

#include <cassert>
#include "Lift.h"

using namespace LiftControl;

vector<LiftTask> Lift::get_schedule()
{
    vector<LiftTask> result;
    if (NULL == this->m_direction)
    {
        return result;
    }
    else
    {
        assert(nullptr != this->m_pass1);
        assert(nullptr != this->m_pass2);
        assert(nullptr != this->m_pass3);
    }

    int previous_floor = 0;
    bool flag = false;
    auto pass = this->m_pass1;
    LoopStart:
    for (auto floor : *pass)
    {
        if (false == flag)
        {
            flag = true;
            previous_floor = floor;
            continue;
        }
        else if (previous_floor == floor)
        {
            continue;
        }
        result.push_back(LiftTask(previous_floor, GetDirection(previous_floor, floor)));
        previous_floor = floor;
    }
    if (pass == this->m_pass1)
    {
        pass = this->m_pass2;
        goto LoopStart;
    }
    else if (pass == this->m_pass2)
    {
        pass = this->m_pass3;
        goto LoopStart;
    }
    else
    {
        assert(pass = this->m_pass3);
        result.push_back(LiftTask(previous_floor, (Direction)NULL));
    }

    return result;
}

void Lift::add(int from_floor, int to_floor)
{
	if (NULL == this->m_direction)
	{
		this->initialize(from_floor, to_floor);
	}

	auto guestDirection = GetDirection(from_floor, to_floor);
    if (this->m_direction == guestDirection)
    {
        if (this->m_atFloor == from_floor || this->m_direction == GetDirection(this->m_atFloor, from_floor))
        {
            this->m_pass1->insert(from_floor);
            this->m_pass1->insert(to_floor);
        }
        else
        {
            assert(this->m_direction == GetDirection(from_floor, this->m_atFloor));
            this->m_pass3->insert(from_floor);
            this->m_pass3->insert(to_floor);
        }
    }
    else
    {
        assert(this->m_direction != guestDirection);
        this->m_pass2->insert(from_floor);
        this->m_pass2->insert(to_floor);
    }
}

void Lift::initialize(int from_floor, int to_floor)
{
	auto guest_direction = GetDirection(from_floor, to_floor);
	this->m_direction = this->m_atFloor == from_floor ? guest_direction : GetDirection(this->m_atFloor, from_floor);
	FloorComparer comparer(this->m_direction), reversed_comparer = comparer.reverse();
	this->m_pass1 = new set<int, FloorComparer>(comparer);
	this->m_pass2 = new set<int, FloorComparer>(reversed_comparer);
	this->m_pass3 = new set<int, FloorComparer>(comparer);
}

Lift::~Lift()
{
    delete this->m_pass1;
    delete this->m_pass2;
    delete this->m_pass3;
}
