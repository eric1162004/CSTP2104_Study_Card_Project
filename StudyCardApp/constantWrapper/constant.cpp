#include "constant.h"

#using <constantLib.dll>

using namespace constantLib;

int getConstantWinWidthManaged()
{
	Constant^ c1 = gcnew Constant;
	return c1->getConstantWinWidth();
}

int getConstantWinHeightManaged()
{
	Constant^ c1 = gcnew Constant;
	return c1->getConstantWinHeight();
}

#pragma managed(push, off)

int getConstantWinWidth()
{
	return getConstantWinWidthManaged();
}

int getConstantWinHeight()
{
	return getConstantWinHeightManaged();
}

#pragma managed(pop)
