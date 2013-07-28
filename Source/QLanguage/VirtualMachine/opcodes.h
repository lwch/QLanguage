/********************************************************************
	created:	2013/07/08
	created:	8:7:2013   21:00
	filename: 	\QLanguage\VirtualMachine\opcodes.h
	file path:	\QLanguage\VirtualMachine
	file base:	opcodes
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _QLANGUAGE_OPCODES_H_
#define _QLANGUAGE_OPCODES_H_

#include "../../QCore/Library/typedef.h"

namespace QLanguage
{
    using namespace Library;

    namespace VM
    {
#pragma pack(1)
        // 操作码
        enum OpCode
        {
            Mov       = 0,  // R[a] = k || R[a] = R[b]
            Less      = 1,  // R[a] < k || R[a] < R[b] || k < k
            More      = 2,  // R[a] > k || R[a] > R[b] || k > k
            LessEqual = 3,  // R[a] <= k || R[a] <= R[b] || k <= k
            MoreEqual = 4,  // R[a] >= k || R[a] >= R[b] || k >= k
            Equal     = 5,  // R[a] == k || R[a] == R[b] || k == k
            Add       = 6,  // R[a] = R[b] + k || R[a] = R[b] + R[c] || R[a] = k + k
            Sub       = 7,  // R[a] = R[b] - k || R[a] = k - R[c] || R[a] = R[b] - R[c] || R[a] = k - k
            Mul       = 8,  // R[a] = R[b] * k || R[a] = R[b] * R[c] || R[a] = k * k
            Div       = 9,  // R[a] = R[b] / k || R[a] = k / R[c] || R[a] = R[b] / R[c] || R[a] = k / k
            Mod       = 10, // R[a] = R[b] % k || R[a] = k % R[c] || R[a] = R[b] % R[c] || R[a] = k % k
            Inc       = 11, // R[a] = R[a] + 1 || R[a] = k + 1
            Dec       = 12, // R[a] = R[a] - 1 || R[a] = k - 1
            Pos       = 13, // R[a] = +R[a] || R[a] = +k
            Neg       = 14, // R[a] = -R[a] || R[a] = -k
            Jmp       = 15, // jmp k
            Call      = 16, // call k1, k2
            Ret       = 17  // ret ([a] || [k])
        };

#define MAKE_OT(a, b, c) ((a << 2) | (b << 1) | c)

        // 每条指令有8字节
        struct Instruction
        {
            uchar op : 5; // 操作码5位，最多32种
            uchar ot : 3; // 3个操作数的类型3位，1为常数，0为寄存器
            union
            {
                struct
                {
                    uchar  ob1; // 0为全局的寄存器或常数，1为当前块的，2为上一级的......
                    uchar  ob2; // 0为全局的寄存器或常数，1为当前块的，2为上一级的......
                    uchar  od ; // 目的寄存器 1字节
                    ushort os1; // 源寄存器1或常数 2字节
                    ushort os2; // 源寄存器2或常数 2字节
                }Normal;
                struct
                {
                    int    addr;   // 跳转或函数位置
                    uchar  ext;    // 未使用或参数个数
                    ushort unused; // 未使用
                }JmpCall;
            };
        };
#pragma pack()
    }
}

#endif
