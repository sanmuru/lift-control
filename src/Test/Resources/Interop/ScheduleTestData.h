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

				bool has_schedule() const { return this->m_schedule.has_value(); }
				Schedule get_schedule() const { return this->m_schedule.value(); }

				ScheduleTestDataEntry(int at_floor, GuestTaskSequence tasks) :
					at_floor(at_floor),
					tasks(tasks),
					m_schedule(nullopt)
				{}
				ScheduleTestDataEntry(int at_floor, GuestTaskSequence tasks, Schedule schedule) :
					at_floor(at_floor),
					tasks(tasks),
					m_schedule(optional(schedule))
				{}

			private:
				const optional<Schedule> m_schedule;
			};

			struct ScheduleTestDataIterator
			{
				using iterator_concept = input_iterator_tag;
				using iterator_category = input_iterator_tag;
				using value_type = ScheduleTestDataEntry;
				using pointer = value_type*;
				using reference = value_type&;

				ScheduleTestDataIterator() = delete;
				ScheduleTestDataIterator(iterator_base<value_type>* underlying) : m_underlying(shared_ptr<iterator_base<value_type>>(underlying)) {}
				~ScheduleTestDataIterator() { this->m_underlying.reset(); }

				ScheduleTestDataIterator& operator++() noexcept
				{
					this->m_underlying->move_next();
					return *this;
				}
				reference operator*() noexcept { return *this->m_underlying->current(); }

				friend bool operator==(const ScheduleTestDataIterator& a, const ScheduleTestDataIterator& b) noexcept
				{
					auto pa = a.m_underlying.get();
					auto pb = b.m_underlying.get();
					if (nullptr == pa)
						return nullptr == pb;
					else if (nullptr == pb)
						return nullptr == pa;
					else
						return pa->equals(pb);
				}
				friend bool operator!=(const ScheduleTestDataIterator& a, const ScheduleTestDataIterator& b) noexcept
				{
					auto pa = a.m_underlying.get();
					auto pb = b.m_underlying.get();
					if (nullptr == pa)
						return nullptr != pb;
					else if (nullptr == pb)
						return nullptr != pa;
					else
						return !pa->equals(pb);
				}

			private:
				shared_ptr<iterator_base<value_type>> m_underlying;
			};

			struct ScheduleTestData
			{
				using iterator = ScheduleTestDataIterator;
				using entry = iterator::value_type;

				ScheduleTestData() = delete;
				ScheduleTestData(sequence_base<entry>* underlying) : m_underlying(shared_ptr<sequence_base<entry>>(underlying)) {}
				~ScheduleTestData() { this->m_underlying.reset(); }
				iterator begin() { return iterator(this->m_underlying->begin()); }
				iterator end() { return iterator(this->m_underlying->end()); }

			private:
				shared_ptr<sequence_base<entry>> m_underlying;
			};
		}
	}
}
