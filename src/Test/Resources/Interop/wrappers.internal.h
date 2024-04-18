#pragma once

#include <assert.h>
#include <msclr\auto_gcroot.h>
#include "wrappers.h"

using namespace msclr;
using namespace System::Collections;

// IEnumerator

struct IEnumeratorReference
{
	IEnumeratorReference(IEnumerator^ underlying) : m_underlying(underlying) {}

	IEnumerator^ get() { return this->m_underlying.get(); }

private:
	const auto_gcroot<IEnumerator^> m_underlying;
};

// ==========================================

// IEnumerable

struct IEnumerableReference
{
	IEnumerableReference(IEnumerable^ underlying) : m_underlying(underlying) {}

	IEnumerable^ get() { return this->m_underlying.get(); }

	IEnumeratorReference* getEnumerator()
	{
		return new IEnumeratorReference(this->get()->GetEnumerator());
	}

private:
	const auto_gcroot<IEnumerable^> m_underlying;
};

// ==========================================