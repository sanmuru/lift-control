#pragma once

#include <iterator>
#include <optional>
#include "GuestTask.h"
#include "Schedule.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			struct ScheduleTestDataEntry
			{
				const int at_floor;
				const GuestTaskSequence tasks;

				ScheduleTestDataEntry() = delete;
				ScheduleTestDataEntry(int at_floor, IEnumerableReference* tasks, IEnumerableReference* schedule) :
					at_floor(at_floor),
					tasks(tasks),
					m_schedule(nullptr == schedule ? std::nullopt : std::optional(Schedule(schedule)))
				{}

				bool has_schedule() const { return this->m_schedule.has_value(); }
				Schedule get_schedule() const { return this->m_schedule.value(); }

			private:
				const std::optional<Schedule> m_schedule;
			};

			struct ScheduleTestData;

			struct ScheduleTestDataIterator
			{
				friend ScheduleTestData;

				using iterator_concept = std::input_iterator_tag;
				using iterator_category = std::input_iterator_tag;
				using difference_type = IEnumeratorVersionType;
				using value_type = ScheduleTestDataEntry;
				using pointer = value_type*;
				using reference = value_type&;

				ScheduleTestDataIterator() = delete;
				ScheduleTestDataIterator(IEnumeratorReference* pRef) : ScheduleTestDataIterator(pRef, IEnumeratorVersion_Init) {}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestDataIterator(const ScheduleTestDataIterator* other) :
					m_pRef(other->m_pRef),
					m_version(other->m_version),
					m_current(nullptr)
				{}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ~ScheduleTestDataIterator();

				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestDataIterator& operator++() noexcept;
				LIFTCONTROL_TESTRESOURCES_INTEROP_API reference operator*() noexcept;

				friend bool operator==(const ScheduleTestDataIterator& a, const ScheduleTestDataIterator& b) noexcept
				{
					if (a.m_version < IEnumeratorVersion_Min)
						return b.m_version < IEnumeratorVersion_Min;
					else if (b.m_version < IEnumeratorVersion_Min)
						return a.m_version < IEnumeratorVersion_Min;
					else
						return a.m_pRef == b.m_pRef && a.m_version == b.m_version;
				}
				friend bool operator!=(const ScheduleTestDataIterator& a, const ScheduleTestDataIterator& b) noexcept { return !(a == b); };

			private:
				std::shared_ptr<IEnumeratorReference> m_pRef;
				difference_type m_version;
				std::unique_ptr<value_type> m_current;

				ScheduleTestDataIterator(IEnumeratorReference* pRef, difference_type version);
			};

			struct ScheduleTestData
			{
				using iterator = ScheduleTestDataIterator;
				using entry = ScheduleTestDataEntry;

				ScheduleTestData() = delete;
				ScheduleTestData(IEnumerableReference* pRef);
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestData(const ScheduleTestData& other) : m_pRef(other.m_pRef) {}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ~ScheduleTestData();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API iterator begin();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API iterator end() { return iterator(nullptr, IEnumeratorVersion_End); }

			private:
				std::shared_ptr<IEnumerableReference> m_pRef;
			};
		}
	}
}
