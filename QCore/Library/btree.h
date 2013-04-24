/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:31
	filename: 	\QCore\Library\btree.h
	file path:	\QCore\Library
	file base:	btree
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_BTREE_H_
#define _QLANGUAGE_LIBRARY_BTREE_H_

#include "function.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        struct __btree_node
        {
        protected:
            typedef __btree_node<T> self;
        public:
            typedef T               value_type;
            typedef self*           link_type;

            __btree_node(const T& x) : data(x), left(NULL), right(NULL)
            {
            }

            value_type data;
            link_type  left;
            link_type  right;
        };

        template <typename T, typename Compare = less<T>, typename Alloc = allocator<__btree_node<T> > >
        class btree
        {
        protected:
            typedef __btree_node<T>          node_type;
            typedef node_type*               link_type;
            typedef btree<T, Compare, Alloc> self;

            link_type root;
            Compare   key_compare;
        public:
            btree() : root(NULL)
            {
            }

            ~btree()
            {
                clear();
            }

            inline void insert(const T& x)
            {
                insert(x, root, NULL);
            }

            inline void erase(const T& x)
            {
                link_type parent = NULL;
                link_type node = find(x, parent);
                erase(node, parent);
            }

            inline link_type find(const T& x)
            {
                link_type parent = NULL;
                return find(x, root, parent);
            }

            inline link_type find(const T& x, link_type& parent)
            {
                return find(x, root, parent);
            }

            inline void clear()
            {
                clear(root);
                root = NULL;
            }
        protected:
            inline link_type find(const T& x, link_type node, link_type& parent)
            {
                if(node)
                {
                    if(key_compare(x, node->data))
					{
                        parent = node;
						return find(x, left(node), parent);
                    }
                    else if(node->data == x) return node;
                    else
                    {
                        parent = node;
                        return find(x, right(node), parent);
                    }
                }
                return NULL;
            }

            void insert(const T& x, link_type& node, link_type parent)
            {
                if(node != NULL)
                {
                    if(key_compare(x, node->data)) insert(x, left(node), node);
                    else insert(x, right(node), node);
                }
                else
                {
                    node = Alloc::allocate(1);
                    construct(node, x);
                }
            }

            void erase(link_type z, link_type _parent)
            {
                // y为被删除的节点
                // x为被删除的节点的孩子
                link_type y = z, x = NULL, z_parent = _parent, y_parent = _parent;
                if(!left(y)) x = right(y);
                else
                {
                    if(!right(y)) x = left(y);
                    else
                    {
						y_parent = y;
                        y = right(y);
                        while(left(y))
						{
							y_parent = y;
							y = left(y);
						}
                        x = right(y);
                    }
                }

                if(y != z)
                {
                    // 将y替换为被删除的节点
                    left(y) = left(z);

                    if(y != right(z)) // 被删除的节点作为根的子树的高度>1
                    {
                        left(y_parent) = x;

                        // 将y替换为要删除的节点
                        right(y) = right(z);
                    }

                    if(root == z) root = y;
                    else if(left(z_parent) == z) left(z_parent) = y;
                    else right(z_parent) = y;

                    // 重设y为要删除的点
                    y = z;
                }
                else
                {
                    // 被删除节点只有一个子节点，直接将其子节点替换为被删除节点即可
                    if(root == z) root = x;
                    else
                    {
                        if(left(z_parent) == z) left(z_parent) = x;
                        else right(z_parent) = x;
                    }
                }
                destruct(&y->data);
                Alloc::deallocate(y, sizeof(y));
            }

            inline void clear(link_type node)
            {
                if(node != NULL)
                {
                    clear(left(node));
                    clear(right(node));
                    destruct(&node->data);
                    Alloc::deallocate(node, sizeof(node));
                }
            }

            inline static link_type& left(link_type node)
            {
                return node->left;
            }

            inline static link_type& right(link_type node)
            {
                return node->right;
            }
        };
    }
}

#endif
