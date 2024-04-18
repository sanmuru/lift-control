#pragma once

#include "GuestTask.h"

using namespace LiftControl::UnitTests::Interop;

namespace std
{
	template<> struct hash<GuestTask>
	{
		size_t operator()(GuestTask const& gt) const noexcept
		{
			size_t h1 = hash<int>{}(gt.from_floor);
			size_t h2 = hash<int>{}(gt.to_floor);
			return h1 ^ (h2 << 1);
		}
	};
}