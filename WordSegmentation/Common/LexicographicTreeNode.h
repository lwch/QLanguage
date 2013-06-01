#ifndef _WORDSEGMENTATION_NODE_H_
#define _WORDSEGMENTATION_NODE_H_

#include "../../QCore/Library/allocator.h"
#include "../../QCore/Library/map.h"

namespace QLanguage
{
    using namespace Library;

    namespace WordSegmentation
    {
        template <typename T>
        struct LexicographicTreeNode
        {
        public:
            typedef allocator<LexicographicTreeNode<T>> Alloc;
        public:
            LexicographicTreeNode(LexicographicTreeNode<T>* parent, const T& v) : mpParent(parent), value(v), mbEnd(false)
            {
            }

            virtual ~LexicographicTreeNode()
            {
            }
        public:
            T value;
            bool mbEnd;
            LexicographicTreeNode<T>* mpParent;
            map<T, LexicographicTreeNode<T>*> mChilds;
        };
    }
}

#endif