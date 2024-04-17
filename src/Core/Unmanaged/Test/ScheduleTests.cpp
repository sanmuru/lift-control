#include "pch.h"
#include "CppUnitTest.h"
#include <format>
#include "Lift.h"
#include "ScheduleTestResources.h"

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
				auto enumerator = ScheduleTestResources::GetSingleTaskTestData();
				while (enumerator->moveNext())
				{
					auto entry = enumerator->current();
					VerifySchedule(entry);
					delete entry;
				}
				delete enumerator;
			}

			TEST_METHOD(TestDoubleTasks)
			{
				auto enumerator = ScheduleTestResources::GetDoubleTasksTestData();
				while (enumerator->moveNext())
				{
					auto entry = enumerator->current();
					VerifySchedule(entry);
					delete entry;
				}
				delete enumerator;
			}

			TEST_METHOD(TestRandomTasks)
			{
				auto enumerator = ScheduleTestResources::GetRandomTasksTestData(
					100,    // sampleCount
					100,    // minLevelCount
					500,    // maxLevelCount
					10,     // minGuestCount
					100		// maxGuestCount
				);
				while (enumerator->moveNext())
				{
					auto entry = enumerator->current();
					VerifySchedule(entry);
					delete entry;
				}
				delete enumerator;
			}

		private:
			static void VerifySchedule(ScheduleTestDataEntry* entry)
			{
				auto lift = new Lift(entry->getAtFloor());
				auto taskEnumerator = entry->getTasks();
				while (taskEnumerator->moveNext())
				{
					auto task = taskEnumerator->current();
					lift->Add(task->getFromFloor(), task->getToFloor());
					delete task;
				}
				delete taskEnumerator;

				auto expectedEnumerator = entry->getSchedule();
				auto actual = lift->GetSchedule();
				auto actualSize = actual.size();

				for (size_t i = 0; ; i++)
				{
					bool expectedHasNext = expectedEnumerator->moveNext();
					bool actualHasNext = i < actualSize;

					if (expectedHasNext == actualHasNext)
					{
						if (false == actualHasNext) goto AssertionPassed;

						auto expectedItem = expectedEnumerator->current();
						auto actualItem = actual[i].floor;
						Assert::AreEqual(expectedItem, actualItem, std::format(L"At {0}: expected {1}, actual {2}", i, expectedItem, actualItem).data());
					}
					else
					{
						if (true == expectedHasNext)
						{
							auto expectedItem = expectedEnumerator->current();
							Assert::Fail(std::format(L"At {0}: expected {1}", i, expectedItem).data());
						}
						else // true == actualHasNext
						{
							auto actualItem = actual[i].floor;
							Assert::Fail(std::format(L"At {0}: unexpected {1}", i, actualItem).data());
						}
					}
				}

			AssertionPassed:
				delete expectedEnumerator;
				delete lift;
			}
		};
	}
}
