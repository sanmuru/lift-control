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
			class LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestResources
			{
			public:
				static ScheduleTestData GetSingleTaskTestData();
				static ScheduleTestData GetDoubleTasksTestData();
				static ScheduleTestData GetRandomTasksTestData(
					int sample_count,
					int min_level_count, int max_level_count,
					int min_guest_count, int max_guest_count);
			};
		}
	}
}
