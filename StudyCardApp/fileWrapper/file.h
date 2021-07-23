#pragma once

#ifdef COMPILE_DLL
#define WRAPPER_LIB_EXPORT __declspec(dllexport)
#else
#define WRAPPER_LIB_EXPORT __declspec(dllimport)
#endif

WRAPPER_LIB_EXPORT float getRadius(int frameNum);

// WRAPPER_LIB_EXPORT float getRadiusManaged(int frameNum);
