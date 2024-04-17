#include "pch.h"

#include <cassert>
#include "Lift.h"

using namespace LiftControl;

static Direction GetDirection(int fromFloor, int atFloor)
{
	assert(fromFloor != atFloor);
	return fromFloor < atFloor ? Direction::Up : Direction::Down;
}

vector<LiftTask> Lift::GetSchedule()
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

    int previousFloor = 0;
    bool flag = false;
    auto pass = this->m_pass1;
    LoopStart:
    for (auto floor : *pass)
    {
        if (false == flag)
        {
            flag = true;
            previousFloor = floor;
            continue;
        }
        else if (previousFloor == floor)
        {
            continue;
        }
        result.push_back(LiftTask(previousFloor, GetDirection(previousFloor, floor)));
        previousFloor = floor;
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
        result.push_back(LiftTask(previousFloor, (Direction)NULL));
    }

    return result;
}

void Lift::Add(int fromFloor, int toFloor)
{
	if (NULL == this->m_direction)
	{
		this->Initialize(fromFloor, toFloor);
	}

	auto guestDirection = GetDirection(fromFloor, toFloor);
    if (this->m_direction == guestDirection)
    {
        if (this->m_atFloor == fromFloor || this->m_direction == GetDirection(this->m_atFloor, fromFloor))
        {
            this->m_pass1->insert(fromFloor);
            this->m_pass1->insert(toFloor);
        }
        else
        {
            assert(this->m_direction == GetDirection(fromFloor, this->m_atFloor));
            this->m_pass3->insert(fromFloor);
            this->m_pass3->insert(toFloor);
        }
    }
    else
    {
        assert(this->m_direction != guestDirection);
        this->m_pass2->insert(fromFloor);
        this->m_pass2->insert(toFloor);
    }
}

void Lift::Initialize(int fromFloor, int toFloor)
{
	auto guestDirection = GetDirection(fromFloor, toFloor);
	this->m_direction = this->m_atFloor == fromFloor ? guestDirection : GetDirection(this->m_atFloor, fromFloor);
	FloorComparer comparer(this->m_direction), reversedComparer = comparer.reverse();
	this->m_pass1 = new set<int, FloorComparer>(comparer);
	this->m_pass2 = new set<int, FloorComparer>(reversedComparer);
	this->m_pass3 = new set<int, FloorComparer>(comparer);
}

Lift::~Lift()
{
    delete this->m_pass1;
    delete this->m_pass2;
    delete this->m_pass3;
}