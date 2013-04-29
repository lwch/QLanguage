/********************************************************************
	created:	2013/03/10
	created:	10:3:2013   21:05
	filename: 	\QCore\Library\graph\bitmap.cpp
	file path:	\QCore\Library\graph
	file base:	bitmap
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <assert.h>

#include "../string.h"

#include "bitmap.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
    namespace graph
    {
        Bitmap::Bitmap()
            : type(Unknown), bits(NULL), width(0), height(0), sizePreLine(0), pQuadTable(NULL)
        {
        }

        Bitmap::Bitmap(size_t width, size_t height, DepthType type /* = RGB */, _RGBQUAD* quadTable /* = NULL */)
            : type(type), width(width), height(height), pQuadTable(NULL)
        {
            if (type == Unknown) throw error<const char*>("Unknown type", __FILE__, __LINE__);
            sizePreLine = ROUND_UP((size_t)(width / 8.0f * type + 0.5f), 4);
            bits = BitsAlloc::allocate(sizePreLine * height);
            memset(bits, 0, sizePreLine * height);
            if (quadTable)
            {
                copyQuadTable(quadTable);
            }
        }

        Bitmap::~Bitmap()
        {
            if (bits) BitsAlloc::deallocate(bits, sizePreLine * height);
            clearQuadTable();
        }

        uchar* Bitmap::line(size_t y)
        {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
            if (y >= height) throw error<const char*>("out of bound", __FILE__, __LINE__);
#endif
            return bits + sizePreLine * y;
        }

        void Bitmap::setQuadTable(_RGBQUAD* quadTable)
        {
            clearQuadTable();
            copyQuadTable(quadTable);
        }

        void Bitmap::save(const string& path)
        {
            assert(bits);
            FILE* fp = fopen(path.c_str(), "wb");
            if (fp == NULL) throw error<const char*>("error open file", __FILE__, __LINE__);
            _BITMAPFILEHEADER fileHeader;
            _BITMAPINFOHEADER infoHeader(width, height, type, sizePreLine * height);
            fileHeader.size = sizeof(_BITMAPFILEHEADER) + sizeof(_BITMAPINFOHEADER) + sizePreLine * height;
            fileHeader.offset = sizeof(_BITMAPFILEHEADER) + sizeof(_BITMAPINFOHEADER);
            size_t quadCount = 0;
            switch (type)
            {
            case Index1:
                quadCount = 2;
                fileHeader.size += (sizeof(_RGBQUAD) >> type);
                fileHeader.offset += (sizeof(_RGBQUAD) >> type);
                break;
            case Index4:
                quadCount = 16;
                fileHeader.size += (sizeof(_RGBQUAD) >> type);
                fileHeader.offset += (sizeof(_RGBQUAD) >> type);
                break;
            case Index8:
                quadCount = 256;
                fileHeader.size += (sizeof(_RGBQUAD) >> type);
                fileHeader.offset += (sizeof(_RGBQUAD) >> type);
                break;
            case RGB:
                break;
            default:
                break;
            }
            fwrite(&fileHeader, sizeof(_BITMAPFILEHEADER), 1, fp);
            fwrite(&infoHeader, sizeof(_BITMAPINFOHEADER), 1, fp);
            if (quadCount) fwrite(pQuadTable, sizeof(_RGBQUAD), quadCount, fp);
            fwrite(bits, sizeof(uchar), sizePreLine * height, fp);
            fclose(fp);
        }

        void Bitmap::copyQuadTable(_RGBQUAD* quadTable)
        {
            if (type == Index1)
            {
                pQuadTable = QuadAlloc::allocate(2);
                memcpy(pQuadTable, quadTable, sizeof(_RGBQUAD) * 2);
            }
            else if (type == Index4)
            {
                pQuadTable = QuadAlloc::allocate(16);
                memcpy(pQuadTable, quadTable, sizeof(_RGBQUAD) * 16);
            }
            else if (type == Index8)
            {
                pQuadTable = QuadAlloc::allocate(256);
                memcpy(pQuadTable, quadTable, sizeof(_RGBQUAD) * 256);
            }
        }

        void Bitmap::clearQuadTable()
        {
            if (pQuadTable)
            {
                if      (type == Index1) QuadAlloc::deallocate(pQuadTable, sizeof(_RGBQUAD) * 2);
                else if (type == Index4) QuadAlloc::deallocate(pQuadTable, sizeof(_RGBQUAD) * 16);
                else if (type == Index8) QuadAlloc::deallocate(pQuadTable, sizeof(_RGBQUAD) * 256);
            }
        }

        inline const size_t Bitmap::ROUND_UP(size_t bytes, size_t align)const
        {
            return (bytes + align - 1) & ~(align - 1);
        }
    }
NAMESPACE_QLANGUAGE_LIBRARY_END
