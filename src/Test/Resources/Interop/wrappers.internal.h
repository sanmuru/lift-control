#pragma once

#include "wrappers.h"
#include <msclr\auto_gcroot.h>

using namespace msclr;
using namespace System::Collections;

// IEnumerator

struct IEnumeratorReference
{
public:
	IEnumeratorReference(IEnumerator^ underlying) : m_underlying(underlying) {}

	IEnumerator^ get() { return this->m_underlying.get(); }

private:
	auto_gcroot<IEnumerator^> m_underlying;
};

// ==========================================

// IEnumerable

struct IEnumerableReference
{
public:
	IEnumerableReference(IEnumerable^ underlying) : m_underlying(underlying) {}

	IEnumerable^ get() { return this->m_underlying.get(); }

	IEnumeratorReference* getEnumerator()
	{
		return new IEnumeratorReference(this->get()->GetEnumerator());
	}

private:
	auto_gcroot<IEnumerable^> m_underlying;
};

// ==========================================