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
                // yΪ��ɾ���Ľڵ�
                // xΪ��ɾ���Ľڵ�ĺ���
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
                    // ��y�滻Ϊ��ɾ���Ľڵ�
                    left(y) = left(z);

                    if(y != right(z)) // ��ɾ���Ľڵ���Ϊ���������ĸ߶�>1
                    {
                        left(y_parent) = x;

                        // ��y�滻ΪҪɾ���Ľڵ�
                        right(y) = right(z);
                    }

                    if(root == z) root = y;
                    else if(left(z_parent) == z) left(z_parent) = y;
                    else right(z_parent) = y;

                    // ����yΪҪɾ���ĵ�
                    y = z;
                }
                else
                {
                    // ��ɾ���ڵ�ֻ��һ���ӽڵ㣬ֱ�ӽ����ӽڵ��滻Ϊ��ɾ���ڵ㼴��
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
