#include "file.h"

#using <fileLib.dll>

using namespace fileLib;

int getWindowWidthManaged()
{
	FileManager^ c1 = gcnew FileManager;
	return c1->getWindowWidth();
}

#pragma managed(push, off)

int getWindowWidth()
{
	return getWindowWidthManaged();
}

#pragma managed(pop)
