#pragma once

#ifdef LIFTCONTROL_TESTRESOURCES_INTEROP_EXPORT
#define LIFTCONTROL_TESTRESOURCES_INTEROP_API __declspec(dllexport)
#else
#define LIFTCONTROL_TESTRESOURCES_INTEROP_API __declspec(dllimport)
#endif
