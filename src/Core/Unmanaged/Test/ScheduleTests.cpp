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

							Assert::AreEqual(*expected_It, *actual_It, std::format(L"At {0}: expected {1}, actual {2}", i, *expected_It, *actual_It).data());
						}
						else
						{
							if (true == expectedHasNext)
							{
								Assert::Fail(std::format(L"At {0}: expected {1}", i, *expected_It).data());
							}
							else // true == actualHasNext
							{
								Assert::Fail(std::format(L"At {0}: unexpected {1}", i, *actual_It).data());
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

			TEST_METHOD(TestRandomTasks)
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

				if (entry.has_schedule())
				{
					auto expected = entry.get_schedule();
					std::vector<int> actual;
					for (auto& liftTask : lift->get_schedule())
					{
						actual.push_back(liftTask.floor);
					}
					AssertEx::AreSequenceEqual(
						expected.begin(), expected.end(),
						actual.begin(), actual.end());
				}
				else
				{
					std::unordered_set<GuestTask> outsideGuests, insideGuests;
					outsideGuests.insert(tasks.begin(), tasks.end());

					int previousFloor = lift->get_at_floor();
					Direction previousDirection = (Direction)NULL;
					int directionChanged = 0;
					for (auto& liftTask : lift->get_schedule())
					{
						auto floor = liftTask.floor;
						auto direction = liftTask.direction;
						std::vector<GuestTask> enter;
						for (auto& outsideTask : outsideGuests)
						{
							auto guestDirection = GetDirection(outsideTask.from_floor, outsideTask.to_floor);
							auto canEnter = guestDirection == direction;
							if (outsideTask.from_floor == floor && canEnter)
								enter.push_back(outsideTask);
						}
						std::vector<GuestTask> leave;
						for (auto& insideTask : insideGuests)
						{
							if (insideTask.to_floor == floor)
								leave.push_back(insideTask);
						}
						Assert::IsTrue(!enter.empty() || !leave.empty()); // otherwise the stop makes no sense.

						for (auto& leaveTask : leave)
						{
							insideGuests.erase(leaveTask);
						}
						for (auto& enterTask : enter)
						{
							insideGuests.insert(enterTask);
							outsideGuests.erase(enterTask);
						}

						if (directionChanged == 0)
						{
							directionChanged = 1; // initialize.
						}
						else
						{
							if (previousDirection != 0 && direction != 0 && previousDirection != direction)
							{
								directionChanged++;
								Assert::IsFalse(directionChanged > 3); // too many passes, not optimistic.
							}
						}
						previousDirection = direction;
						previousFloor = floor;
					}

					Assert::IsTrue(outsideGuests.empty());
					Assert::IsTrue(insideGuests.empty());
				}

				delete lift;
			}
		};
	}
}
