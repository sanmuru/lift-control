#pragma once

#include <iterator>
#include <string>
#include <vector>
#include "ScheduleTestData.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			namespace ScheduleTestResources
			{
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestData GetSingleTaskTestData();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestData GetDoubleTasksTestData();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestData GetRandomTasksTestData(
					int sample_count,
					int min_level_count, int max_level_count,
					int min_guest_count, int max_guest_count);

				LIFTCONTROL_TESTRESOURCES_INTEROP_API void VerifySchedule(
					vector<int> actual,
					int at_floor, GuestTaskSequence tasks);
				LIFTCONTROL_TESTRESOURCES_INTEROP_API void VerifySchedule(
					Schedule expected,
					vector<int> actual,
					int at_floor, GuestTaskSequence tasks);
			}
		}
	}
}
