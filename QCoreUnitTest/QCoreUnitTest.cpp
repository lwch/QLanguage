#include <stdio.h>

#ifdef _WINDOWS
#include <windows.h>
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#include "../QCore/vld/vld.h"
#endif

#include "Units/TestBase.h"

using namespace QLanguage;
using namespace QLanguage::Library;

int retCode = 0;

int main()
{
    return retCode;
}
