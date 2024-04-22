#pragma once

using namespace std;

namespace LiftControl
{
	namespace UnitTests
	{
		namespace Interop
		{
			template <typename item_t>
			class iterator_base
			{
			public:
				virtual shared_ptr<item_t> current() = 0;
				virtual void move_next() = 0;
				virtual bool equals(iterator_base<item_t>* other) = 0;
				virtual bool is_end() = 0;
			};

			template <typename item_t>
			class end_iterator_base : public iterator_base<item_t>
			{
			public:
				shared_ptr<item_t> current() { return shared_ptr<item_t>(nullptr); }
				void move_next() {}
				bool equals(iterator_base<item_t>* other)
				{
					auto pOther = dynamic_cast<end_iterator_base<item_t>*>(other);
					if (pOther) return true;
					else
						return other->equals(this);
				}
				bool is_end() { return true; }
			};

			template <typename item_t>
			class singleton_iterator_base : public iterator_base<item_t>
			{
			public:
				shared_ptr<item_t> current() { return at_end ? shared_ptr(nullptr) : this->m_item; }
				void move_next() { if (!this->at_end) this->at_end = true; }
				bool is_end() { return at_end; }

			protected:
				singleton_iterator_base(shared_ptr<item_t> item) : at_end(false), m_item(item) {}

				bool at_end;
				shared_ptr<item_t> m_item;
			};

			template <typename item_t>
			class doubleton_iterator_base : public iterator_base<item_t>
			{
			public:
				shared_ptr<item_t> current()
				{
					switch (this->m_version)
					{
					case 1:
						return this->m_item1;
					case 2:
						return this->m_item2;
					default:
						return shared_ptr<item_t>(nullptr);
					}
				}
				void move_next()
				{
					switch (this->m_version)
					{
					case 1:
						return this->m_version = 2;
					case 2:
						return this->m_version = 0;
					}
				}
				bool is_end() { return this->m_item1 == 0; }

			protected:
				doubleton_iterator_base(shared_ptr<item_t> item1, shared_ptr<item_t> item2) : m_version(1), m_item1(item1), m_item2(item2) {}

				int m_version;
				shared_ptr<item_t> m_item1;
				shared_ptr<item_t> m_item2;
			};

			template <typename item_t>
			class tripleton_iterator_base : public iterator_base<item_t>
			{
			public:
				shared_ptr<item_t> current()
				{
					switch (this->m_version)
					{
					case 1:
						return this->m_item1;
					case 2:
						return this->m_item2;
					case 3:
						return this->m_item3;
					default:
						return shared_ptr<item_t>(nullptr);
					}
				}
				void move_next()
				{
					switch (this->m_version)
					{
					case 1:
						return this->m_version = 2;
					case 2:
						return this->m_version = 3;
					case 3:
						return this->m_version = 0;
					}
				}
				bool is_end() { return this->m_item1 == 0; }

			protected:
				tripleton_iterator_base(shared_ptr<item_t> item1, shared_ptr<item_t> item2, shared_ptr<item_t> item3) :
					m_version(1),
					m_item1(item1), m_item2(item2), m_item3(item3) {}

				int m_version;
				shared_ptr<item_t> m_item1;
				shared_ptr<item_t> m_item2;
				shared_ptr<item_t> m_item3;
			};

			template <typename item_t>
			class sequence_base
			{
			public:
				virtual iterator_base<item_t>* begin() = 0;
				virtual iterator_base<item_t>* end() { return new end_iterator_base<item_t>(); }
			};

		}
	}
}
