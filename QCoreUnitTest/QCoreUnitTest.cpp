#include <stdio.h>

#ifdef _WINDOWS
#include <windows.h>
#endif

#if defined(_DEBUG) && defined(_WINDOWS)
#include "../QCore/vld/vld.h"
#endif

#include "Units/TestVector.h"

int main()
{
    return 0;
}
