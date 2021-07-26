#include "file.h"

#using <fileLib.dll>

using namespace fileLib;

float getRadiusManaged(int frameNum)
{
	FileManager^ c1 = gcnew FileManager;
	return c1->getRadius(0);
}

#pragma managed(push, off)

float getRadius(int frameNum)
{
	return getRadiusManaged(frameNum);
}

#pragma managed(pop)
