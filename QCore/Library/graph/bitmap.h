/********************************************************************
	created:	2013/03/10
	created:	10:3:2013   21:03
	filename: 	\QCore\Library\graph\bitmap.h
	file path:	\QCore\Library\graph
	file base:	bitmap
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_LIBRARY_GRAPH_BITMAP_H_
#define _QLANGUAGE_LIBRARY_GRAPH_BITMAP_H_

#include "bmp/structs.h"

#include "../allocator.h"
#include "../definition.h"
#include "../typedef.h"
#include "../string.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    using namespace QLanguage::Library;
    namespace graph
    {
        class Bitmap
        {
            typedef allocator<uchar>    BitsAlloc;
            typedef allocator<_RGBQUAD> QuadAlloc;
        public:
            enum DepthType
            {
                Unknown,    // 未知
                Index1 = 1, // 双色
                Index4 = 4, // 16色
                Index8 = 8, // 256色
                RGB = 24,   // RGB
            };
        public:
            Bitmap();
            Bitmap(size_t width, size_t height, DepthType type = RGB, _RGBQUAD* quadTable = NULL);
            ~Bitmap();

            void setQuadTable(_RGBQUAD* quadTable);

            uchar* line(size_t y);

            void save(const string& path);
        protected:
            void copyQuadTable(_RGBQUAD* quadTable);
            void clearQuadTable();
            const size_t ROUND_UP(size_t bytes, size_t align)const;
        protected:
            DepthType type;
            uchar*    bits;
            size_t    width;
            size_t    height;
            size_t    sizePreLine;
            _RGBQUAD* pQuadTable;
        };
    }
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
