#include "wrapper.h"
#using <fileLib.dll>

using namespace fileLib;

float getRadiusManaged(int frameNum)
{
	File^ c1 = gcnew File();
	return c1->getFileNum(0);
}

#pragma managed(push, off)

float getFileNum(int frameNum)
{
	return getRadiusManaged(frameNum);
}

#pragma managed(pop)
