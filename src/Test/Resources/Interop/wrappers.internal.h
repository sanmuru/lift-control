#pragma once

#include <assert.h>
#include <msclr\auto_gcroot.h>
#include "wrappers.h"

using namespace msclr;
using namespace System;
using namespace System::Collections;

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{

			typedef long long IEnumeratorVersionType;
			constexpr IEnumeratorVersionType IEnumeratorVersion_Init = -1LL;
			constexpr IEnumeratorVersionType IEnumeratorVersion_End = -2LL;
			constexpr IEnumeratorVersionType IEnumeratorVersion_Min = 0LL;
			constexpr IEnumeratorVersionType IEnumeratorVersion_Max = LLONG_MAX;

			// IEnumerator

			template <typename unmanaged_t>
			class IEnumeratorWrapper : public iterator_base<unmanaged_t>
			{
			public:
				IEnumeratorWrapper(IEnumerator^ underlying, IEnumeratorVersionType version)
				{
					assert(version <= IEnumeratorVersion_End || nullptr != underlying);
					this->m_underlying = underlying;
					if (version == IEnumeratorVersion_Init)
					{
						this->m_version = this->m_underlying->MoveNext() ? IEnumeratorVersion_Min : IEnumeratorVersion_End;
					}
					else
					{
						this->m_version = version;
					}
				}

				bool is_end() { return this->m_version <= IEnumeratorVersion_End; }

			protected:
				IEnumeratorVersionType m_version;

				IEnumerator^ get() { return this->m_underlying.get(); }
				virtual shared_ptr<unmanaged_t> convert(Object^ obj) = 0;
				virtual void move_next()
				{
					if (!this->m_underlying->MoveNext())
						this->m_version = IEnumeratorVersion_End;
					else
						++this->m_version;
				}
				virtual bool equals(iterator_base<unmanaged_t>* other)
				{
					auto pEnd = dynamic_cast<end_iterator_base<unmanaged_t>*>(other);
					return nullptr != pEnd && this->is_end();

					auto pWrapper = dynamic_cast<IEnumeratorWrapper<unmanaged_t>*>(other);
					return nullptr != pWrapper && this->equals(pWrapper);
				}
				virtual bool equals(IEnumeratorWrapper<unmanaged_t>* other)
				{
					bool this_is_end = this->is_end();
					bool other_is_end = other->is_end();
					if (this_is_end) return other_is_end;
					else if (other_is_end) return this_is_end;
					else
						return Object::ReferenceEquals(this->m_underlying.get(), other->m_underlying.get()) && this->m_version == other->m_version;
				}

			protected:
				auto_gcroot<IEnumerator^> m_underlying;
			};

			template <typename unmanaged_t, typename managed_t>
			class GenericIEnumeratorWrapper : public IEnumeratorWrapper<unmanaged_t>
			{
			public:
				GenericIEnumeratorWrapper(Generic::IEnumerator<managed_t>^ underlying, IEnumeratorVersionType version) : IEnumeratorWrapper<unmanaged_t>(underlying, version) {}

			protected:
				Generic::IEnumerator<managed_t>^ get() { return (Generic::IEnumerator<managed_t>^)this->m_underlying.get(); }
				virtual shared_ptr<unmanaged_t> convert(managed_t obj) = 0;

			private:
				shared_ptr<unmanaged_t> convert(Object^ obj) { return convert((managed_t)obj); }
			};

			// ==========================================

			// IEnumerable

			template <typename unmanaged_t>
			class IEnumerableWrapper : public sequence_base<unmanaged_t>
			{
			public:
				IEnumerableWrapper(IEnumerable^ underlying) : m_underlying(underlying) {}

			protected:
				const auto_gcroot<IEnumerable^> m_underlying;
			};

			template <typename unmanaged_t, typename managed_t>
			class GenericIEnumerableWrapper : public IEnumerableWrapper<unmanaged_t>
			{
			public:
				GenericIEnumerableWrapper(Generic::IEnumerable<managed_t>^ underlying) : IEnumerableWrapper<unmanaged_t>(underlying) {}

			protected:
				Generic::IEnumerable<managed_t>^ get() { return (Generic::IEnumerable<managed_t>^)this->m_underlying.get(); }
			};

			// ==========================================

		}
	}
}