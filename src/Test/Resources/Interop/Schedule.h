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
				friend Schedule;

				using iterator_concept = std::input_iterator_tag;
				using iterator_category = std::input_iterator_tag;
				using difference_type = IEnumeratorVersionType;
				using value_type = int;
				using pointer = value_type*;
				using reference = value_type&;

				ScheduleIterator() = delete;
				ScheduleIterator(IEnumeratorReference* pRef) : ScheduleIterator(pRef, IEnumeratorVersion_Init) {}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleIterator(const ScheduleIterator* other) :
					m_pRef(other->m_pRef),
					m_version(other->m_version),
					m_current(nullptr)
				{}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ~ScheduleIterator();

				LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleIterator& operator++() noexcept;
				LIFTCONTROL_TESTRESOURCES_INTEROP_API reference operator*() noexcept;

				friend bool operator==(const ScheduleIterator& a, const ScheduleIterator& b) noexcept
				{
					if (a.m_version < IEnumeratorVersion_Min)
						return b.m_version < IEnumeratorVersion_Min;
					else if (b.m_version < IEnumeratorVersion_Min)
						return a.m_version < IEnumeratorVersion_Min;
					else
						return a.m_pRef == b.m_pRef && a.m_version == b.m_version;
				}
				friend bool operator!=(const ScheduleIterator& a, const ScheduleIterator& b) noexcept { return !(a == b); };

			private:
				std::shared_ptr<IEnumeratorReference> m_pRef;
				difference_type m_version;
				std::unique_ptr<value_type> m_current;

				ScheduleIterator(IEnumeratorReference* pRef, difference_type version);
			};

			struct Schedule
			{
				using iterator = ScheduleIterator;
				using entry = int;

				Schedule() = delete;
				Schedule(IEnumerableReference* pRef);
				LIFTCONTROL_TESTRESOURCES_INTEROP_API Schedule(const Schedule& other) : m_pRef(other.m_pRef) {}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ~Schedule();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API iterator begin();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API iterator end() { return iterator(nullptr, IEnumeratorVersion_End); }

			private:
				std::shared_ptr<IEnumerableReference> m_pRef;
			};
		}
	}
}