#pragma once

#ifdef COMPILE_DLL
#define WRAPPER_LIB_EXPORT __declspec(dllexport)
#else
#define WRAPPER_LIB_EXPORT __declspec(dllimport)
#endif

WRAPPER_LIB_EXPORT int getConstantWinWidth();
WRAPPER_LIB_EXPORT int getConstantWinHeight();