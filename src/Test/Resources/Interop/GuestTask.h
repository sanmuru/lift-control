#pragma once

#include <iterator>
#include <memory>
#include "exports.h"
#include "wrappers.h"

using namespace std;

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			struct GuestTask
			{
				const int from_floor;
				const int to_floor;

				GuestTask() = delete;
				GuestTask(int from_floor, int to_floor) : from_floor(from_floor), to_floor(to_floor) {}

				friend bool operator==(const GuestTask& a, const GuestTask& b) noexcept {
					return a.from_floor == b.from_floor && a.to_floor == b.to_floor;
				}
			};

			struct GuestTaskIterator
			{
				using iterator_concept = input_iterator_tag;
				using iterator_category = input_iterator_tag;
				using value_type = GuestTask;
				using pointer = value_type*;
				using reference = value_type&;

				GuestTaskIterator() = delete;
				GuestTaskIterator(iterator_base<value_type>* underlying) : m_underlying(shared_ptr<iterator_base<value_type>>(underlying)) {}
				~GuestTaskIterator() { this->m_underlying.reset(); }

				GuestTaskIterator& operator++() noexcept
				{
					this->m_underlying->move_next();
					return *this;
				}
				reference operator*() noexcept { return *this->m_underlying->current(); }

				friend bool operator==(const GuestTaskIterator& a, const GuestTaskIterator& b) noexcept
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
				friend bool operator!=(const GuestTaskIterator& a, const GuestTaskIterator& b) noexcept
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

			struct GuestTaskSequence
			{
				using iterator = GuestTaskIterator;
				using entry = iterator::value_type;

				GuestTaskSequence() = delete;
				GuestTaskSequence(sequence_base<entry>* underlying) : m_underlying(shared_ptr<sequence_base<entry>>(underlying)) {}
				~GuestTaskSequence() { this->m_underlying.reset(); }
				iterator begin() { return iterator(this->m_underlying->begin()); }
				iterator end() { return iterator(this->m_underlying->end()); }

			private:
				shared_ptr<sequence_base<entry>> m_underlying;
			};
		}
	}
}