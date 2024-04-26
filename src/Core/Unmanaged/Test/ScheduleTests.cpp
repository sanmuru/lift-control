#include "pch.h"
#include "CppUnitTest.h"
#include <format>
#include <unordered_set>
#include "Lift.h"
#include "ScheduleTestResources.h"
#include "GuestTask.hash.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LiftControl::Generation1;
using namespace LiftControl::UnitTests::Interop;

namespace LiftControl
{
	namespace UnitTests
	{
		TEST_CLASS(ScheduleTests)
		{
		public:

			TEST_METHOD(TestSingleTask)
			{
				auto data = ScheduleTestResources::GetSingleTaskTestData();
				for (auto& entry : data)
				{
					VerifySchedule(entry);
				}
			}

			TEST_METHOD(TestDoubleTasks)
			{
				auto data = ScheduleTestResources::GetDoubleTasksTestData();
				for (auto& entry : data)
				{
					VerifySchedule(entry);
				}
			}

			TEST_METHOD(TestMultipleTasks)
			{
				auto data = ScheduleTestResources::GetRandomTasksTestData(
					100,    // sampleCount
					100,    // minLevelCount
					500,    // maxLevelCount
					10,     // minGuestCount
					100		// maxGuestCount
				);
				for (auto& entry : data)
				{
					VerifySchedule(entry);
				}
			}

		private:
			static void VerifySchedule(ScheduleTestDataEntry& entry)
			{
				auto lift = new Lift(entry.at_floor);
				auto tasks = entry.tasks;
				for (auto& task : tasks)
				{
					lift->add(task.from_floor, task.to_floor);
				}

				vector<int> actual;
				for (auto floor : lift->get_schedule())
				{
					actual.push_back(floor);
				}
				if (entry.has_schedule())
				{
					auto expected = entry.get_schedule();
					ScheduleTestResources::VerifySchedule(expected, actual, entry.at_floor, tasks);
				}
				else
				{
					ScheduleTestResources::VerifySchedule(actual, entry.at_floor, tasks);
				}

				delete lift;
			}
		};
	}
}
