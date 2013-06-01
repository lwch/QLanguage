#include <stdio.h>
#include <Windows.h>

#if defined(_DEBUG) && defined(_WINDOWS_)
#include "../QCore/vld/vld.h"
#endif

#include "../QCore/Library/construct.h"
#include "../QCore/Library/allocator.h"
#include "Common/LexicographicTreeNode.h"

using namespace QLanguage::Library;
using namespace QLanguage::WordSegmentation;

map<char, LexicographicTreeNode<char>*> treeRoot;

typedef allocator<char> CHAR_ALLOC;

bool createLexicographicTreeNode(const char* lpBuffer)
{
    const char* current = lpBuffer;
    LexicographicTreeNode<char>* currentNode = NULL;
    while(*current)
    {
        LexicographicTreeNode<char>* parentNode = currentNode;
        if(currentNode == NULL)
        {
            currentNode = treeRoot[*current];
        }
        else
        {
            currentNode = currentNode->mChilds[*current];
        }
        if(currentNode != NULL)
        {
            ++current;
            continue;
        }
        LexicographicTreeNode<char>* newNode = LexicographicTreeNode<char>::Alloc::allocate();
        construct(newNode, parentNode, *current);
        if(parentNode == NULL)
        {
            treeRoot[*current] = newNode;
        }
        else
        {
            parentNode->mChilds[*current] = newNode;
        }
        currentNode = newNode;
        ++current;
    }
    currentNode->mbEnd = true;
    return true;
}

bool createLexicographicTree(const char* lpBuffer)
{
    const char *first = lpBuffer, *current = lpBuffer;
    while(*current)
    {
        if(*current == '\r' || *current == '\n')
        {
            const size_t size = current - first;
            char* str = CHAR_ALLOC::allocate(size + 1);
            memcpy(str, first, size);
            str[size] = 0;
            createLexicographicTreeNode(str);
            while(*current && (*current == '\r' || *current == '\n')) ++current;
            first = current;
            CHAR_ALLOC::deallocate(str, size + 1);
            continue;
        }
        ++current;
    }
    return true;
}

void printLexicographicTree(LexicographicTreeNode<char>* pNode)
{
    if(pNode->mbEnd)
    {
        size_t size = 1;
        LexicographicTreeNode<char>* pCurrent = pNode;
        while(pCurrent = pCurrent->mpParent) ++size;
        char* lpBuffer = CHAR_ALLOC::allocate(size + 1);
        pCurrent = pNode;
        lpBuffer[size] = 0;
        lpBuffer[size - 1] = pNode->value;
        --size;
        while(pCurrent = pCurrent->mpParent)
        {
            lpBuffer[size - 1] = pCurrent->value;
            --size;
        }
        printf("%s\n", lpBuffer);
        CHAR_ALLOC::deallocate(lpBuffer, size + 1);
    }

    map<char, LexicographicTreeNode<char>*>::iterator j = pNode->mChilds.begin();

    for(map<char, LexicographicTreeNode<char>*>::iterator i = pNode->mChilds.begin(); i != pNode->mChilds.end(); ++i)
    {
        printLexicographicTree((*i).second);
    }
}

void printLexicographicTree()
{
    for(map<char, LexicographicTreeNode<char>*>::iterator i = treeRoot.begin(); i != treeRoot.end(); ++i)
    {
        LexicographicTreeNode<char>* pNode = (*i).second;
        printLexicographicTree(pNode);
    }
}

int main()
{
    FILE* fp = fopen("Lexicographic", "r");
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        size_t file_size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* lpBuffer = CHAR_ALLOC::allocate(file_size + 1);
        size_t new_size = fread(lpBuffer, sizeof(char), file_size, fp);
        lpBuffer[new_size] = 0;
        fclose(fp);

        createLexicographicTree(lpBuffer);

        printLexicographicTree();

        CHAR_ALLOC::deallocate(lpBuffer, file_size + 1);
    }

    __MemoryPool__1.dump();

    system("pause");
	return 0;
}