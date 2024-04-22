#include "pch.h"

#include <assert.h>
#include "ScheduleTestResources.h"
#include "ScheduleTestData.internal.h"

using namespace LiftControl::UnitTests::Interop;

static ScheduleTestData WrapScheduleTestData(Generic::IEnumerable<cli::array<Object^>^>^ enumerable)
{
	return new ScheduleTestDataIEnumerableWrapperSequence(enumerable);
}

ScheduleTestData ScheduleTestResources::GetSingleTaskTestData()
{
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetSingleTaskTestData());
}

ScheduleTestData ScheduleTestResources::GetDoubleTasksTestData()
{
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetDoubleTasksTestData());
}

ScheduleTestData ScheduleTestResources::GetRandomTasksTestData(
	int sample_count,
	int min_level_count, int max_level_count,
	int min_guest_count, int max_uest_count)
{
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetRandomTaskTestData(
		sample_count,
		min_level_count, max_level_count,
		min_guest_count, max_uest_count));
}

void ScheduleTestResources::VerifySchedule(Schedule actual, int at_floor, GuestTaskSequence tasks)
{
}

void ScheduleTestResources::VerifySchedule(Schedule expected, Schedule actual, int at_floor, GuestTaskSequence tasks)
{
}
