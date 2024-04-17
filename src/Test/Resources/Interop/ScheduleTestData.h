#pragma once

#include "exports.h"
#include "wrappers.h"
#include <iterator>

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			struct ScheduleTestDataEntry;

			struct ScheduleTestDataIterator
			{
			public:
				using iterator_concept = std::input_iterator_tag;
				using iterator_category = std::input_iterator_tag;
				using value_type = ScheduleTestDataEntry;
				using pointer = value_type*;
				using reference = value_type&;

				explicit ScheduleTestDataIterator(IEnumeratorReference* pRef) : m_pRef(pRef) {}
				~ScheduleTestDataIterator();

				//https://internalpointers.com/post/writing-custom-iterators-modern-cpp
				constexpr reference operator*() const noexcept;
				constexpr pointer operator->() const noexcept;

			private:
				IEnumeratorReference* m_pRef;
			};

			struct LIFTCONTROL_TESTRESOURCES_INTEROP_API ScheduleTestData
			{
			public:
				explicit ScheduleTestData(IEnumeratorReference* pRef) : m_pRef(pRef) {}
				~ScheduleTestData();
				ScheduleTestDataEntry* current();
				bool moveNext();
				void moo()
				{
					auto v = new std::vector<int>();
					v->begin();
				}

			private:
				IEnumeratorReference* m_pRef;
			};
		}
	}
}
