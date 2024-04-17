#pragma once

#include <iterator>
#include <string>
#include <vector>
#include "wrappers.h"
#include "GuestTask.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			struct LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleIterator
			{
			public:
				explicit ScheduleIterator(IEnumeratorReference* pRef) : m_pRef(pRef) {}
				~ScheduleIterator();
				int current();
				bool moveNext();

			private:
				IEnumeratorReference* m_pRef;
			};

			struct LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestDataEntry
			{
			public:
				int getAtFloor() const { return this->m_atFloor; }
				GuestTaskIterator* getTasks();
				ScheduleIterator* getSchedule();

				ScheduleTestDataEntry(
					int atFloor,
					IEnumerableReference* tasks,
					IEnumerableReference* schedule) :
					m_atFloor(atFloor),
					m_tasks(tasks),
					m_schedule(schedule)
				{}

				~ScheduleTestDataEntry();

			private:
				int m_atFloor;
				IEnumerableReference* m_tasks;
				IEnumerableReference* m_schedule;
			};

			struct LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestData
			{
			public:
				explicit ScheduleTestData(IEnumeratorReference* pRef) : m_pRef(pRef) {}
				~ScheduleTestData();
				ScheduleTestDataEntry* current();
				bool moveNext();

			private:
				IEnumeratorReference* m_pRef;
			};

			class LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestResources
			{
			public:
				static ScheduleTestData* GetSingleTaskTestData();
				static ScheduleTestData* GetDoubleTasksTestData();
				static ScheduleTestData* GetRandomTasksTestData(
					int sampleCount,
					int minLevelCount, int maxLevelCount,
					int minGuestCount, int maxGuestCount);
			};
		}
	}
}
