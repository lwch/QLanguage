/********************************************************************
	created:	2013/03/10
	created:	10:3:2013   20:26
	filename: 	\QCore\Library\graph\bmp\filestruct.h
	file path:	\QCore\Library\graph\bmp
	file base:	filestruct
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_GRAPH_BMP_FILESTRUCT_H_
#define _QLANGUAGE_LIBRARY_GRAPH_BMP_FILESTRUCT_H_

#include "../../definition.h"
#include "../../typedef.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    using namespace QLanguage::Library;
    namespace graph
    {
#pragma pack(1)
        struct _BITMAPFILEHEADER
        {
            ushort type;
            ulong  size;
            ushort reserved1;
            ushort reserved2;
            ulong  offset;

            _BITMAPFILEHEADER()
            {
                type = 0x4D42; // BM
                size = 0;
                reserved1 = reserved2 = 0;
                offset = 0;
            }
        };

        struct _BITMAPINFOHEADER
        {
            ulong  size;
            long   width;
            long   height;
            ushort planes;
            ushort bitCount;
            ulong  compression;
            ulong  sizeImage;
            long   xPelsPerMeter;
            long   yPelsPerMeter;
            ulong  clrUsed;
            ulong  clrImportant;

            _BITMAPINFOHEADER(long width, long height, ushort bitCount, ulong sizeImage)
                : width(width), height(height), bitCount(bitCount), sizeImage(sizeImage)
            {
                size = sizeof(_BITMAPINFOHEADER);
                planes = 1;
                compression = 0;
                xPelsPerMeter = yPelsPerMeter = 0;
                clrUsed = clrImportant = 0;
            }
        };

#ifdef red
#undef red
#endif

        struct _RGBQUAD
        {
            uchar blue;
            uchar green;
            uchar red;
            uchar reserved;

            _RGBQUAD() : blue(0), green(0), red(0), reserved(0) {}
            _RGBQUAD(uchar blue, uchar green, uchar red) : blue(blue), green(green), red(red), reserved(0) {}
        };
#pragma pack()
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
