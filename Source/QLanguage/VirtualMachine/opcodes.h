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
            Mov       = 0,  // R[a] = k  || R[a] = R[b]
            Less      = 1,  // R[a] < k  || k < R[b]  || R[a] < R[b]
            More      = 2,  // R[a] > k  || k > R[b]  || R[a] > R[b]
            LessEqual = 3,  // R[a] <= k || k <= R[b] || R[a] <= R[b]
            MoreEqual = 4,  // R[a] >= k || k >= R[b] || R[a] >= R[b]
            Equal     = 5,  // R[a] == k || k == R[b] || R[a] == R[b]
            Add       = 6,  // R[a] = R[b] + k  || R[a] = k + R[c]  || R[a] = R[b] + R[c]
            Sub       = 7,  // R[a] = R[b] - k  || R[a] = k - R[c]  || R[a] = R[b] - R[c]
            Mul       = 8,  // R[a] = R[b] * k  || R[a] = k * R[c]  || R[a] = R[b] * R[c]
            Div       = 9,  // R[a] = R[b] / k  || R[a] = k / R[c]  || R[a] = R[b] / R[c]
            Mod       = 10, // R[a] = R[b] % k  || R[a] = k % R[c]  || R[a] = R[b] % R[c]
            LogicAnd  = 11, // R[a] = R[b] && k || R[a] = k && R[c] || R[a] = R[b] && R[c]
            LogicOr   = 12, // R[a] = R[b] || k || R[a] = k || R[c] || R[a] = R[b] || R[c]
            Not       = 13, // R[a] = !R[b]
            BitAnd    = 14, // R[a] = R[b] & k  || R[a] = k & R[c]  || R[a] = R[b] & R[c]
            BitOr     = 15, // R[a] = R[b] | k  || R[a] = k | R[c]  || R[a] = R[b] | R[c]
            BitXor    = 16, // R[a] = R[b] ^ k  || R[a] = k ^ R[c]  || R[a] = R[b] ^ R[c]
            Inc       = 17, // R[a] = R[a] + 1
            Dec       = 18, // R[a] = R[a] - 1
            Pos       = 19, // R[a] = +R[a]
            Neg       = 20, // R[a] = -R[a]
            Jmp       = 21, // jmp k
            Call      = 22, // call k1, k2
            Ret       = 23  // ret ([a] || [k])
        };

#define MAKE_OT(s1, s2, d) ((s1 << 2) | (s2 << 1) | d)
#define OT_DST_TYPE(ot) (ot & 1)
#define OT_SRC1_TYPE(ot) ((ot >> 2) & 1)
#define OT_SRC2_TYPE(ot) ((ot >> 1) & 1)

        // 每条指令有10字节
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
                    ushort os1; // 源寄存器1或常数 2字节
                    ushort os2; // 源寄存器2或常数 2字节
                    uchar  obd; // 0为全局的寄存器或常数，1为当前块的，2为上一级的......
                    ushort od ; // 目的寄存器 1字节
                }Normal;
                struct
                {
                    int    addr;   // 跳转或函数位置
                    uchar  ext;    // 未使用或参数个数
                    uint   unused; // 未使用
                }JmpCall;
            };
        };
#pragma pack()
    }
}

#endif
