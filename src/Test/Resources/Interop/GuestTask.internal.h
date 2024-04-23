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
			typedef ValueTuple<int, int> GuestTask_managed_t;
			typedef Generic::IEnumerable<GuestTask_managed_t>^ GuestTaskSequence_managed_t;

			// iterator

			class GuestTaskIEnumeratorWrapperIterator : public GenericIEnumeratorWrapper<GuestTask, GuestTask_managed_t>
			{
			public:
				GuestTaskIEnumeratorWrapperIterator() = delete;
				GuestTaskIEnumeratorWrapperIterator(Generic::IEnumerator<GuestTask_managed_t>^ underlying) : GuestTaskIEnumeratorWrapperIterator(underlying, IEnumeratorVersion_Init) {}
				GuestTaskIEnumeratorWrapperIterator(Generic::IEnumerator<GuestTask_managed_t>^ underlying, IEnumeratorVersionType version) : GenericIEnumeratorWrapper<GuestTask, GuestTask_managed_t>(underlying, version) {}
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

				shared_ptr<GuestTask> convert(GuestTask_managed_t tuple)
				{
					return make_shared<GuestTask>(tuple.Item1, tuple.Item2);
				}

				std::shared_ptr<GuestTask> m_current;
			};

			class GuestTaskIEnumerableWrapperSequence : public GenericIEnumerableWrapper<GuestTask, GuestTask_managed_t>
			{
			public:
				GuestTaskIEnumerableWrapperSequence(Generic::IEnumerable<GuestTask_managed_t>^ underlying) : GenericIEnumerableWrapper<GuestTask, GuestTask_managed_t>(underlying) {}

				iterator_base<GuestTask>* begin() { return new GuestTaskIEnumeratorWrapperIterator(this->get()->GetEnumerator()); }
				iterator_base<GuestTask>* end() { return new end_iterator_base<GuestTask>(); }
			};

			template <typename iterator_t>
			ref class GuestTaskIteratorWrapper : public IteratorGenericWrapper<GuestTask_managed_t, iterator_t>
			{
			public:
				GuestTaskIteratorWrapper(iterator_t begin, iterator_t end) : IteratorGenericWrapper<GuestTask_managed_t, iterator_t>(begin, end) {}

			protected:
				virtual GuestTask_managed_t convert(iterator_t iterator) sealed override
				{
					GuestTask gt = *iterator;
					return GuestTask_managed_t(gt.from_floor, gt.to_floor);
				}
			};

			template <typename container_t, typename iterator_t = container_t::iterator>
			ref class GuestTaskSequenceWrapper : public ContainerGenericWrapper<GuestTask_managed_t, container_t, iterator_t>
			{
			public:
				GuestTaskSequenceWrapper(container_t container) : ContainerGenericWrapper<GuestTask_managed_t, container_t, iterator_t>(container) {}

			protected:
				virtual IteratorGenericWrapper<GuestTask_managed_t, iterator_t>^ wrap_range(iterator_t begin, iterator_t end) sealed override
				{
					return gcnew GuestTaskIteratorWrapper<iterator_t>(begin, end);
				}
			};

		}
	}
}