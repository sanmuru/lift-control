#include "pch.h"

#include <assert.h>
#include "ScheduleTestResources.h"
#include "wrappers.internal.h"

using namespace System;
using namespace LiftControl::UnitTests::Interop;

static IEnumerableReference* WrapGuestTasks(IEnumerable^ tasks)
{
	return new IEnumerableReference(tasks);
}

static IEnumerableReference* WrapSchedule(IEnumerable^ schedule)
{
	return new IEnumerableReference(schedule);
}

static ScheduleTestData* WrapScheduleTestData(IEnumerator^ data)
{
	return new ScheduleTestData(new IEnumeratorReference(data));
}

ScheduleIterator::~ScheduleIterator()
{
	delete this->m_pRef;
}
int ScheduleIterator::current()
{
	return (int)this->m_pRef->get()->Current;
}
bool ScheduleIterator::moveNext()
{
	return this->m_pRef->get()->MoveNext();
}

ScheduleTestDataEntry::~ScheduleTestDataEntry()
{
	delete this->m_tasks;
	if (this->m_schedule != nullptr)
	{
		delete this->m_schedule;
	}
}
GuestTaskIterator* ScheduleTestDataEntry::getTasks()
{
	return new GuestTaskIterator(this->m_tasks->getEnumerator());
}
ScheduleIterator* ScheduleTestDataEntry::getSchedule()
{
	assert(this->m_schedule != nullptr);
	return new ScheduleIterator(this->m_schedule->getEnumerator());
}

ScheduleTestData::~ScheduleTestData()
{
	delete this->m_pRef;
}
ScheduleTestDataEntry* ScheduleTestData::current()
{
	auto entry = (array<Object^>^)this->m_pRef->get()->Current;
	auto atFloor = (int)entry[0];
	auto tasks = WrapGuestTasks((IEnumerable^)entry[1]);
	auto schedule = entry->Length <= 2 ? nullptr : WrapSchedule((IEnumerable^)entry[2]);
	return new ScheduleTestDataEntry(atFloor, tasks, schedule);
}
bool ScheduleTestData::moveNext()
{
	return this->m_pRef->get()->MoveNext();
}

ScheduleTestData* ScheduleTestResources::GetSingleTaskTestData()
{
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetSingleTaskTestData()->GetEnumerator());
}

ScheduleTestData* ScheduleTestResources::GetDoubleTasksTestData()
{
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetDoubleTasksTestData()->GetEnumerator());
}

ScheduleTestData* ScheduleTestResources::GetRandomTasksTestData(
	int sampleCount,
	int minLevelCount, int maxLevelCount,
	int minGuestCount, int maxGuestCount)
{
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetRandomTaskTestData(
		sampleCount,
		minLevelCount, maxLevelCount,
		minGuestCount, maxGuestCount)->GetEnumerator());
}
