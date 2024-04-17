#include "pch.h"
#include "GuestTask.h"
#include "wrappers.internal.h"

using namespace System;
using namespace LiftControl::UnitTests::Interop;

GuestTaskIterator::~GuestTaskIterator()
{
	delete this->m_pRef;
}

GuestTask* GuestTaskIterator::current()
{
	auto tuple = (ValueTuple<int, int>^)this->m_pRef->get()->Current;
	return new GuestTask(tuple->Item1, tuple->Item2);
}

bool GuestTaskIterator::moveNext()
{
	return this->m_pRef->get()->MoveNext();
}