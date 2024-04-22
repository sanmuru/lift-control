#pragma once

#include <memory>
#include "exports.h"
#include "GuestTask.internal.h"
#include "Schedule.internal.h"
#include "ScheduleTestData.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			// iterator

			class ScheduleTestDataIEnumeratorWrapperIterator : public GenericIEnumeratorWrapper<ScheduleTestDataEntry, cli::array<Object^>^>
			{
			public:
				ScheduleTestDataIEnumeratorWrapperIterator() = delete;
				ScheduleTestDataIEnumeratorWrapperIterator(Generic::IEnumerator<cli::array<Object^>^>^ underlying) : ScheduleTestDataIEnumeratorWrapperIterator(underlying, IEnumeratorVersion_Init) {}
				ScheduleTestDataIEnumeratorWrapperIterator(Generic::IEnumerator<cli::array<Object^>^>^ underlying, IEnumeratorVersionType version) : GenericIEnumeratorWrapper<ScheduleTestDataEntry, cli::array<Object^>^>(underlying, version) {}
				ScheduleTestDataIEnumeratorWrapperIterator(const ScheduleTestDataIEnumeratorWrapperIterator& other) = delete;
				~ScheduleTestDataIEnumeratorWrapperIterator() { this->m_current.reset(); }

			private:
				shared_ptr<ScheduleTestDataEntry> current()
				{
					if (!this->m_current)
						this->m_current = this->convert(this->get()->Current);
					return this->m_current;
				}

				void move_next()
				{
					this->m_current.reset();
					this->IEnumeratorWrapper<ScheduleTestDataEntry>::move_next();
				}

				shared_ptr<ScheduleTestDataEntry> convert(cli::array<Object^>^ arr)
				{
					assert(arr->Length >= 2);
					auto atFloor = (int)arr[0];
					auto tasks = GuestTaskSequence(new GuestTaskIEnumerableWrapperSequence((Generic::IEnumerable<ValueTuple<int, int>>^)arr[1]));
					if (arr->Length == 2)
						return make_shared<ScheduleTestDataEntry>(atFloor, tasks);
					
					auto schedule = Schedule(new ScheduleIEnumerableWrapperSequence((Generic::IEnumerable<int>^)arr[2]));
					return make_shared<ScheduleTestDataEntry>(atFloor, tasks, schedule);
				}

				std::shared_ptr<ScheduleTestDataEntry> m_current;
			};

			class ScheduleTestDataIEnumerableWrapperSequence : public GenericIEnumerableWrapper<ScheduleTestDataEntry, cli::array<Object^>^>
			{
			public:
				ScheduleTestDataIEnumerableWrapperSequence(Generic::IEnumerable<cli::array<Object^>^>^ underlying) : GenericIEnumerableWrapper<ScheduleTestDataEntry, cli::array<Object^>^>(underlying) {}

				iterator_base<ScheduleTestDataEntry>* begin() { return new ScheduleTestDataIEnumeratorWrapperIterator(this->get()->GetEnumerator()); }
				iterator_base<ScheduleTestDataEntry>* end() { return new end_iterator_base<ScheduleTestDataEntry>(); }
			};

		}
	}
}