#include "pch.h"
#include <assert.h>
#include "Schedule.h"
#include "wrappers.internal.h"

using namespace LiftControl::UnitTests::Interop;

Schedule::Schedule(IEnumerableReference* pRef)
{
	assert(nullptr != pRef);
	this->m_pRef = std::shared_ptr<IEnumerableReference>(pRef);
}

Schedule::~Schedule()
{
	this->m_pRef.reset();
}

Schedule::iterator Schedule::begin()
{
	return this->m_pRef->getEnumerator();
}

ScheduleIterator::ScheduleIterator(IEnumeratorReference* pRef, ScheduleIterator::difference_type version)
{
	assert(version <= IEnumeratorVersion_End || nullptr != pRef);
	this->m_pRef = std::shared_ptr<IEnumeratorReference>(pRef);
	if (version == IEnumeratorVersion_Init)
	{
		this->m_version = this->m_pRef->get()->MoveNext() ? IEnumeratorVersion_Min : IEnumeratorVersion_End;
	}
	else
	{
		this->m_version = version;
	}
	this->m_current = std::unique_ptr<value_type>(nullptr);
}

ScheduleIterator::~ScheduleIterator()
{
	this->m_current.reset();
	this->m_pRef.reset();
}

ScheduleIterator& ScheduleIterator::operator++() noexcept
{
	if (this->m_version >= IEnumeratorVersion_Min) // not at end
	{
		auto has_next = this->m_pRef->get()->MoveNext();
		if (has_next)
		{
			this->m_version++;
		}
		else // at end
		{
			this->m_version = IEnumeratorVersion_End;
		}

		this->m_current.reset();
	}
	return *this;
}

ScheduleIterator::reference ScheduleIterator::operator*() noexcept
{
	assert(this->m_version >= IEnumeratorVersion_Min);
	assert(this->m_pRef);
	if (!this->m_current)
	{
		auto current = (int)this->m_pRef->get()->Current;
		this->m_current = std::make_unique<int>(current);
	}
	assert(this->m_current);

	return *this->m_current;
}