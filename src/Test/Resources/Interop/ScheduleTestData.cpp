#include "pch.h"
#include <assert.h>
#include "ScheduleTestData.h"
#include "wrappers.internal.h"

using namespace System;
using namespace LiftControl::UnitTests::Interop;

ScheduleTestData::entry* WrapScheduleTestDataEntry(Object^ obj)
{
	auto args = (array<Object^>^)obj;
	auto atFloor = (int)args[0];
	auto tasks = (IEnumerable^)args[1];
	auto schedule = args->Length == 2 ? nullptr : (IEnumerable^)args[2];
	return new ScheduleTestDataEntry(
		atFloor,
		new IEnumerableReference(tasks),
		nullptr == schedule ? nullptr : new IEnumerableReference(schedule));
}

ScheduleTestData::ScheduleTestData(IEnumerableReference* pRef)
{
	assert(nullptr != pRef);
	this->m_pRef = std::shared_ptr<IEnumerableReference>(pRef);
}

ScheduleTestData::~ScheduleTestData()
{
	this->m_pRef.reset();
}

ScheduleTestData::iterator ScheduleTestData::begin()
{
	return this->m_pRef->getEnumerator();
}

ScheduleTestDataIterator::ScheduleTestDataIterator(IEnumeratorReference* pRef, ScheduleTestDataIterator::difference_type version)
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
	this->m_current = nullptr;
}

ScheduleTestDataIterator::~ScheduleTestDataIterator()
{
	this->m_current.reset();
	this->m_pRef.reset();
}

ScheduleTestDataIterator& ScheduleTestDataIterator::operator++() noexcept
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

ScheduleTestDataIterator::reference ScheduleTestDataIterator::operator*() noexcept
{
	assert(this->m_version >= IEnumeratorVersion_Min);
	assert(this->m_pRef);
	if (!this->m_current)
	{
		this->m_current = std::unique_ptr<value_type>(WrapScheduleTestDataEntry(this->m_pRef->get()->Current));
	}
	assert(this->m_current);

	return *(this->m_current);
}
