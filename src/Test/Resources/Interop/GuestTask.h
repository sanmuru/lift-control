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

			struct GuestTaskSequence;

			struct GuestTaskIterator
			{
				friend GuestTaskSequence;

				using iterator_concept = std::input_iterator_tag;
				using iterator_category = std::input_iterator_tag;
				using difference_type = IEnumeratorVersionType;
				using value_type = GuestTask;
				using pointer = value_type*;
				using reference = value_type&;

				GuestTaskIterator() = delete;
				GuestTaskIterator(IEnumeratorReference* pRef) : GuestTaskIterator(pRef, IEnumeratorVersion_Init) {}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API GuestTaskIterator(const GuestTaskIterator& other) :
					m_pRef(other.m_pRef),
					m_version(other.m_version),
					m_current(nullptr)
				{}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ~GuestTaskIterator();

				LIFTCONTROL_TESTRESOURCES_INTEROP_API GuestTaskIterator& operator++() noexcept;
				LIFTCONTROL_TESTRESOURCES_INTEROP_API reference operator*() noexcept;

				friend bool operator==(const GuestTaskIterator& a, const GuestTaskIterator& b) noexcept
				{
					if (a.m_version < IEnumeratorVersion_Min)
						return b.m_version < IEnumeratorVersion_Min;
					else if (b.m_version < IEnumeratorVersion_Min)
						return a.m_version < IEnumeratorVersion_Min;
					else
						return a.m_pRef == b.m_pRef && a.m_version == b.m_version;
				}
				friend bool operator!=(const GuestTaskIterator& a, const GuestTaskIterator& b) noexcept { return !(a == b); };

			private:
				std::shared_ptr<IEnumeratorReference> m_pRef;
				difference_type m_version;
				std::unique_ptr<value_type> m_current;

				GuestTaskIterator(IEnumeratorReference* pRef, difference_type version);
			};

			struct GuestTaskSequence
			{
				using iterator = GuestTaskIterator;
				using entry = GuestTask;

				GuestTaskSequence() = delete;
				GuestTaskSequence(IEnumerableReference* pRef);
				LIFTCONTROL_TESTRESOURCES_INTEROP_API GuestTaskSequence(const GuestTaskSequence& other) : m_pRef(other.m_pRef) {}
				LIFTCONTROL_TESTRESOURCES_INTEROP_API ~GuestTaskSequence();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API iterator begin();
				LIFTCONTROL_TESTRESOURCES_INTEROP_API iterator end() { return iterator(nullptr, IEnumeratorVersion_End); }

			private:
				std::shared_ptr<IEnumerableReference> m_pRef;
			};
		}
	}
}