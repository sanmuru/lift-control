#include "pch.h"

#include <cassert>
#include "ScheduleTestResources.h"
#include "ScheduleTestData.internal.h"

using namespace LiftControl::UnitTests::Interop;
using namespace LiftControl::UnitTests::Interop::ScheduleTestResources;

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
	return WrapScheduleTestData(LiftControl::UnitTests::ScheduleTestResources::GetRandomTasksTestData(
		sample_count,
		min_level_count, max_level_count,
		min_guest_count, max_uest_count));
}
