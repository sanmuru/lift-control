#pragma once

#include "exports.h"
#include "wrappers.h"

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			struct LIFTCONTROL_TESTRESOURCES_INTEROP_API GuestTask
			{
			public:
				int getFromFloor() const { return this->m_fromFloor; }
				int getToFloor() const { return this->m_toFloor; }

				explicit GuestTask(int fromFloor, int toFloor) : m_fromFloor(fromFloor), m_toFloor(toFloor) {}

			private:
				int m_fromFloor;
				int m_toFloor;
			};

			struct LIFTCONTROL_TESTRESOURCES_INTEROP_API GuestTaskIterator
			{
			public:
				explicit GuestTaskIterator(IEnumeratorReference* pRef) : m_pRef(pRef) {}
				~GuestTaskIterator();
				GuestTask* current();
				bool moveNext();

			private:
				IEnumeratorReference* m_pRef;
			};
		}
	}
}