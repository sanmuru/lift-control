#pragma once

#include <memory>
#include "exports.h"
#include "wrappers.internal.h"
#include "GuestTask.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			// iterator

			class GuestTaskIEnumeratorWrapperIterator : public GenericIEnumeratorWrapper<GuestTask, ValueTuple<int, int>>
			{
			public:
				GuestTaskIEnumeratorWrapperIterator() = delete;
				GuestTaskIEnumeratorWrapperIterator(Generic::IEnumerator<ValueTuple<int, int>>^ underlying) : GuestTaskIEnumeratorWrapperIterator(underlying, IEnumeratorVersion_Init) {}
				GuestTaskIEnumeratorWrapperIterator(Generic::IEnumerator<ValueTuple<int, int>>^ underlying, IEnumeratorVersionType version) : GenericIEnumeratorWrapper<GuestTask, ValueTuple<int, int>>(underlying, version) {}
				GuestTaskIEnumeratorWrapperIterator(const GuestTaskIEnumeratorWrapperIterator& other) = delete;
				~GuestTaskIEnumeratorWrapperIterator() { this->m_current.reset(); }

			private:
				shared_ptr<GuestTask> current()
				{
					if (!this->m_current)
						this->m_current = this->convert(this->get()->Current);
					return this->m_current;
				}

				void move_next()
				{
					this->m_current.reset();
					this->IEnumeratorWrapper<GuestTask>::move_next();
				}

				shared_ptr<GuestTask> convert(ValueTuple<int, int> tuple)
				{
					return make_shared<GuestTask>(tuple.Item1, tuple.Item2);
				}

				std::shared_ptr<GuestTask> m_current;
			};

			class GuestTaskIEnumerableWrapperSequence : public GenericIEnumerableWrapper<GuestTask, ValueTuple<int, int>>
			{
			public:
				GuestTaskIEnumerableWrapperSequence(Generic::IEnumerable<ValueTuple<int, int>>^ underlying) : GenericIEnumerableWrapper<GuestTask, ValueTuple<int, int>>(underlying) {}

				iterator_base<GuestTask>* begin() { return new GuestTaskIEnumeratorWrapperIterator(this->get()->GetEnumerator()); }
				iterator_base<GuestTask>* end() { return new end_iterator_base<GuestTask>(); }
			};

		}
	}
}