#ifndef _QLANGUAGE_OPCODES_H_
#define _QLANGUAGE_OPCODES_H_

namespace QLanguage
{
    namespace VM
    {
        enum OpCode
        {
            Mov,        // R[a] = k || R[a] = R[b]
            Less,       // R[a] < k || R[a] < R[b]
            More,       // R[a] > k || R[a] > R[b]
            LessEqual,  // R[a] <= k || R[a] <= R[b]
            MoreEqual,  // R[a] >= k || R[a] >= R[b]
            Equal,      // R[a] == k || R[a] == R[b]
            Add,        // R[a] = R[b] + k || R[a] = R[b] + R[c]
            Sub,        // R[a] = R[b] - k || R[a] = k - R[c] || R[a] = R[b] - R[c]
            Mul,        // R[a] = R[b] * k || R[a] = R[b] * R[c]
            Div,        // R[a] = R[b] / k || R[a] = k / R[c] || R[a] = R[b] / R[c]
            Jmp,        // jmp a
            Call,       // call a
            Ret,        // ret [a]
            Count       // count of opcodes
        };
    }
}

#endif
