#pragma once

#include <iterator>
#include <memory>
#include "exports.h"
#include "wrappers.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			struct Schedule;

			struct ScheduleIterator
			{
				using iterator_concept = input_iterator_tag;
				using iterator_category = input_iterator_tag;
				using value_type = int;
				using pointer = value_type*;
				using reference = value_type&;

				ScheduleIterator() = delete;
				ScheduleIterator(iterator_base<value_type>* underlying) : m_underlying(shared_ptr<iterator_base<value_type>>(underlying)) {}
				~ScheduleIterator() { this->m_underlying.reset(); }

				ScheduleIterator& operator++() noexcept
				{
					this->m_underlying->move_next();
					return *this;
				}
				reference operator*() noexcept { return *this->m_underlying->current(); }

				friend bool operator==(const ScheduleIterator& a, const ScheduleIterator& b) noexcept
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
				friend bool operator!=(const ScheduleIterator& a, const ScheduleIterator& b) noexcept
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

			struct Schedule
			{
				using iterator = ScheduleIterator;
				using entry = iterator::value_type;

				Schedule() = delete;
				Schedule(sequence_base<entry>* underlying) : m_underlying(shared_ptr<sequence_base<entry>>(underlying)) {}
				~Schedule() { this->m_underlying.reset(); }
				iterator begin() { return iterator(this->m_underlying->begin()); }
				iterator end() { return iterator(this->m_underlying->end()); }

			private:
				shared_ptr<sequence_base<entry>> m_underlying;
			};
		}
	}
}