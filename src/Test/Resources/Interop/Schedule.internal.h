#pragma once

#include <memory>
#include "exports.h"
#include "wrappers.internal.h"
#include "Schedule.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			// iterator

			class ScheduleIEnumeratorWrapperIterator : public GenericIEnumeratorWrapper<int, int>
			{
			public:
				ScheduleIEnumeratorWrapperIterator() = delete;
				ScheduleIEnumeratorWrapperIterator(Generic::IEnumerator<int>^ underlying) : ScheduleIEnumeratorWrapperIterator(underlying, IEnumeratorVersion_Init) {}
				ScheduleIEnumeratorWrapperIterator(Generic::IEnumerator<int>^ underlying, IEnumeratorVersionType version) : GenericIEnumeratorWrapper<int, int>(underlying, version) {}
				ScheduleIEnumeratorWrapperIterator(const ScheduleIEnumeratorWrapperIterator& other) = delete;
				~ScheduleIEnumeratorWrapperIterator() { this->m_current.reset(); }

			private:
				shared_ptr<int> current()
				{
					if (!this->m_current)
						this->m_current = this->convert(this->get()->Current);
					return this->m_current;
				}

				void move_next()
				{
					this->m_current.reset();
					this->IEnumeratorWrapper<int>::move_next();
				}

				shared_ptr<int> convert(int value)
				{
					return make_shared<int>(value);
				}

				std::shared_ptr<int> m_current;
			};

			class ScheduleIEnumerableWrapperSequence : public GenericIEnumerableWrapper<int, int>
			{
			public:
				ScheduleIEnumerableWrapperSequence(Generic::IEnumerable<int>^ underlying) : GenericIEnumerableWrapper<int, int>(underlying) {}

				iterator_base<int>* begin() { return new ScheduleIEnumeratorWrapperIterator(this->get()->GetEnumerator()); }
				iterator_base<int>* end() { return new end_iterator_base<int>(); }
			};

		}
	}
}