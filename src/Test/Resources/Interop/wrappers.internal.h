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

			template <typename iterator_t>
			ref class IteratorWrapper abstract : public IEnumerator
			{
			public:
				IteratorWrapper(iterator_t begin, iterator_t end) :
					m_init(false),
					m_current(new iterator_t(begin)), m_end(new iterator_t(end)) {}
				virtual ~IteratorWrapper()
				{
					delete this->m_current;
					delete this->m_end;
				}

				property Object^ Current
				{
					virtual Object^ get() sealed { return this->convert(*this->m_current); }
				}
				virtual bool MoveNext()
				{
					if (!this->m_init)
					{
						this->m_init = true;
					}
					else
					{
						++(*this->m_current);
					}
					return (*this->m_current) != (*this->m_end);
				}
				virtual void Reset() { throw gcnew NotSupportedException(); }

			protected:
				virtual System::Object^ convert(iterator_t iterator) = 0;

				bool m_init;
				iterator_t* m_current;
				const iterator_t* m_end;
			};

			template <typename managed_t, typename iterator_t>
			ref class IteratorGenericWrapper abstract : public IteratorWrapper<iterator_t>, Generic::IEnumerator<managed_t>
			{
			public:
				IteratorGenericWrapper(iterator_t begin, iterator_t end) : IteratorWrapper<iterator_t>(begin, end) {}

				property managed_t Current
				{
					virtual managed_t get() new sealed { return this->convert(*this->m_current); }
				}

			protected:
				virtual Object^ _base_convert(iterator_t iterator) sealed = IteratorWrapper<iterator_t>::convert
				{
					return this->convert(iterator);
				}
				virtual managed_t convert(iterator_t iterator) new = 0;
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

			template <typename container_t, typename iterator_t>
			ref class ContainerWrapper abstract : public IEnumerable
			{
			public:
				ContainerWrapper(container_t container) : m_container(new container_t(container)) {}
				virtual ~ContainerWrapper()
				{
					delete this->m_container;
				}

				virtual IEnumerator^ GetEnumerator() sealed
				{
					return this->wrap_range(this->get_begin(), this->get_end());
				}

			protected:
				container_t* m_container;

				virtual IteratorWrapper<iterator_t>^ wrap_range(iterator_t begin, iterator_t end) = 0;
				virtual iterator_t get_begin() { return this->m_container->begin(); }
				virtual iterator_t get_end() { return this->m_container->end(); }
			};

			template <typename managed_t, typename container_t, typename iterator_t = container_t::iterator>
			ref class ContainerGenericWrapper abstract : public ContainerWrapper<container_t, iterator_t>, Generic::IEnumerable<managed_t>
			{
			public:
				ContainerGenericWrapper(container_t container) : ContainerWrapper<container_t, iterator_t>(container) {}

				virtual Generic::IEnumerator<managed_t>^ GetEnumerator() new sealed
				{
					return this->wrap_range(this->get_begin(), this->get_end());
				}

			protected:
				virtual IteratorWrapper<iterator_t>^ _base_wrap_range(iterator_t begin, iterator_t end) sealed = ContainerWrapper<container_t, iterator_t>::wrap_range
				{
					return this->wrap_range(begin, end);
				}
				virtual IteratorGenericWrapper<managed_t, iterator_t>^ wrap_range(iterator_t begin, iterator_t end) new = 0;
			};

			// ==========================================

		}
	}
}