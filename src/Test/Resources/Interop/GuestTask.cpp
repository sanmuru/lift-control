#include "pch.h"
#include <assert.h>
#include "GuestTask.h"
#include "wrappers.internal.h"

using namespace System;
using namespace LiftControl::UnitTests::Interop;

static GuestTask* WrapGuestTask(Object^ obj)
{
	auto tuple = (ValueTuple<int, int>^)obj;
	return new GuestTask(tuple->Item1, tuple->Item2);
}

GuestTaskSequence::GuestTaskSequence(IEnumerableReference* pRef)
{
	assert(nullptr != pRef);
	this->m_pRef = std::shared_ptr<IEnumerableReference>(pRef);
}

GuestTaskSequence::~GuestTaskSequence()
{
	this->m_pRef.reset();
}

GuestTaskSequence::iterator GuestTaskSequence::begin()
{
	return this->m_pRef->getEnumerator();
}

GuestTaskIterator::GuestTaskIterator(IEnumeratorReference* pRef, GuestTaskIterator::difference_type version)
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

GuestTaskIterator::~GuestTaskIterator()
{
	this->m_current.reset();
	this->m_pRef.reset();
}

GuestTaskIterator& GuestTaskIterator::operator++() noexcept
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

GuestTaskIterator::reference GuestTaskIterator::operator*() noexcept
{
	if (this->m_version == IEnumeratorVersion_End)
		assert(0 ==IEnumeratorVersion_End);
	else if (this->m_version == IEnumeratorVersion_Init)
		assert(this->m_version != IEnumeratorVersion_Init);
	else
	assert(this->m_version >= IEnumeratorVersion_Min);
	assert(this->m_pRef);
	if (!this->m_current)
	{
		this->m_current = std::unique_ptr<value_type>(WrapGuestTask(this->m_pRef->get()->Current));
	}
	assert(this->m_current);

	return *this->m_current;
}
