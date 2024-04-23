#include "pch.h"

#include "xunit_assert.internal.h"
#include "ScheduleTestResources.h"
#include "ScheduleTestData.internal.h"

using namespace LiftControl::UnitTests::Interop;
using namespace LiftControl::UnitTests::Interop::ScheduleTestResources;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void ScheduleTestResources::VerifySchedule(
	vector<int> actual,
	int at_floor, GuestTaskSequence tasks)
{
	Generic::IEnumerable<int>^ expected_wrapped = nullptr;
	auto actual_wrapped = gcnew ScheduleSequenceWrapper<vector<int>>(actual);
	auto tasks_wrapped = gcnew GuestTaskSequenceWrapper<GuestTaskSequence>(tasks);

	XunitAssert(LiftControl::UnitTests::ScheduleTestResources::VerifySchedule,
		expected_wrapped, actual_wrapped, at_floor, tasks_wrapped);
}

void ScheduleTestResources::VerifySchedule(
	Schedule expected,
	vector<int> actual,
	int at_floor, GuestTaskSequence tasks)
{
	auto expected_wrapped = gcnew ScheduleSequenceWrapper<Schedule>(expected);
	auto actual_wrapped = gcnew ScheduleSequenceWrapper<vector<int>>(actual);
	auto tasks_wrapped = gcnew GuestTaskSequenceWrapper<GuestTaskSequence>(tasks);

	XunitAssert(LiftControl::UnitTests::ScheduleTestResources::VerifySchedule,
		expected_wrapped, actual_wrapped, at_floor, tasks_wrapped);
}
