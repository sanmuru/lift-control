#pragma once

#include "CppUnitTest.h"

#include <msclr/marshal.h>

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template <typename func_t, typename... args_t>
			void XunitAssert(func_t* method_throws_xunit_exception, args_t... args)
			{
				try
				{
					method_throws_xunit_exception(args...);
				}
				catch (Xunit::Sdk::XunitException^ xe)
				{
					auto context = gcnew msclr::interop::marshal_context();
					Assert::Fail(context->marshal_as<const wchar_t*>(xe->Message));
				}
				catch (System::Exception^ e)
				{
					auto context = gcnew msclr::interop::marshal_context();
					Assert::Fail(context->marshal_as<const wchar_t*>(e->Message));
				}
			}
		}
	}
}