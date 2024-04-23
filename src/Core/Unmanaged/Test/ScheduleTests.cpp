#include "pch.h"
#include "CppUnitTest.h"
#include <format>
#include <unordered_set>
#include "Lift.h"
#include "ScheduleTestResources.h"
#include "GuestTask.hash.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LiftControl;
using namespace LiftControl::UnitTests::Interop;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			class AssertEx
			{
			public:
				template <typename _TIt_expected, typename _TIt_actual>
				static void AreSequenceEqual(
					_TIt_expected expected_BIt, _TIt_expected expected_EIt,
					_TIt_actual actual_BIt, _TIt_actual actual_EIt)
				{
					auto& expected_It = expected_BIt;
					auto& actual_It = actual_BIt;
					long long i = 0;
					while (true)
					{
						bool expectedHasNext = expected_It != expected_EIt;
						bool actualHasNext = actual_It != actual_EIt;

						if (expectedHasNext == actualHasNext)
						{
							if (false == actualHasNext) return;

							Assert::AreEqual(*expected_It, *actual_It, format(L"At {0}: expected {1}, actual {2}", i, *expected_It, *actual_It).data());
						}
						else
						{
							if (true == expectedHasNext)
							{
								Assert::Fail(format(L"At {0}: expected {1}", i, *expected_It).data());
							}
							else // true == actualHasNext
							{
								Assert::Fail(format(L"At {0}: unexpected {1}", i, *actual_It).data());
							}
						}

						++expected_It;
						++actual_It;
						++i;
					}
				}
			};
		}
	}
}

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
				for (auto& liftTask : lift->get_schedule())
				{
					actual.push_back(liftTask.floor);
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
