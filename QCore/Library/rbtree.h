#ifndef _QLANGUAGE_LIBRARY_RBTREE_H_
#define _QLANGUAGE_LIBRARY_RBTREE_H_

#include "function.h"
#include "pair.h"
#include "iterator.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        struct __rbtree_node
        {
        protected:
            typedef __rbtree_node<T> self;
        public:
            typedef T                value_type;
            typedef self*            link_type;
            enum color_type          { red, black };

            __rbtree_node(const T& x) : data(x), parent(0), left(0), right(0), color(red)
            {
            }

            link_type parent;
            link_type left;
            link_type right;
            value_type data;
            color_type color;
        };

        template <typename T>
        struct __rbtree_iterator_base
        {
        protected:
            template <typename Key, typename Value, typename KeyOfValue, typename Less_Compare, typename Equal_Compare>
            friend class rbtree;

            typedef __rbtree_node<T> node_type;
            typedef node_type*       link_type;

            link_type node;
        public:
            __rbtree_iterator_base() : node(NULL)
            {
            }

            __rbtree_iterator_base(link_type x) : node(x)
            {
            }

            void inc()
            {
                if(node->right)
                {
                    node = node->right;
                    while(node->left) node = node->left;
                }
                else
                {
                    link_type y = node->parent;
                    while(y && node == y->right)
                    {
                        node = y;
                        y = y->parent;
                    }
                    //if(!y) throw "It's maximum value";
                    if(node->right != y) node = y;
                }
            }

            void dec()
            {
                if(node->left)
                {
                    node = node->left;
                    while(node->right) node = node->right;
                }
                else
                {
                    link_type y = node->parent;
                    while(y && node == y->left)
                    {
                        node = y;
                        y = y->parent;
                    }
                    //if(!y) throw "It's minimum value";
                    if(node->left != y) node = y;
                }
            }

            bool operator==(const __rbtree_iterator_base<T>& x)const
            {
                return node == x.node;
            }

            bool operator!=(const __rbtree_iterator_base<T>& x)const
            {
                return node != x.node;
            }
        };

        template <typename T>
        struct __rbtree_iterator : public iterator<T>, __rbtree_iterator_base<T>
        {
        protected:
            template <typename C>
            friend struct __rbtree_const_iterator;

            typedef __rbtree_iterator_base<T> parent;
            typedef iterator<T> iterator_parent;
        public:
            __rbtree_iterator() : __rbtree_iterator_base<T>()
            {
            }

            __rbtree_iterator(typename parent::link_type x) : __rbtree_iterator_base<T>(x)
            {
            }

            typename iterator_parent::reference operator*()const
            {
                return parent::node->data;
            }

            typename iterator_parent::self& operator++()
            {
                parent::inc();
                return *this;
            }

            typename iterator_parent::self operator++(int)
            {
                typename parent::self tmp = *this;
                ++*this;
                return tmp;
            }

            typename iterator_parent::self& operator--()
            {
                parent::dec();
                return *this;
            }

            typename iterator_parent::self operator--(int)
            {
                typename parent::self tmp = *this;
                --*this;
                return tmp;
            }
        };

        template <typename T>
        struct __rbtree_const_iterator : public const_iterator<T>, public __rbtree_iterator_base<T>
        {
        protected:
            typedef __rbtree_iterator<T> iterator;
            typedef __rbtree_iterator_base<T> parent;
        public:
            __rbtree_const_iterator() : __rbtree_iterator_base<T>()
            {
            }

            __rbtree_const_iterator(typename parent::link_type x) : __rbtree_iterator_base<T>(x)
            {
            }

            __rbtree_const_iterator(const iterator& i) : __rbtree_iterator_base<T>(i.node)
            {
            }

            typename parent::reference operator*()const
            {
                return parent::node->data;
            }

            typename parent::self& operator++()
            {
                parent::inc();
                return *this;
            }

            typename parent::self operator++(int)
            {
                typename parent::self tmp = *this;
                ++*this;
                return tmp;
            }

            typename parent::self& operator--()
            {
                parent::dec();
                return *this;
            }

            typename parent::self operator--(int)
            {
                typename parent::self tmp = *this;
                --*this;
                return tmp;
            }
        };

        template <typename Key, typename Value, typename KeyOfValue,
            typename Less_Compare = less<Key>,
            typename Equal_Compare = equal_to<Key> >
        class rbtree
        {
        protected:
            typedef Key                  key_type;
            typedef Value                value_type;
            typedef __rbtree_node<Value> node_type;
            typedef node_type*           link_type;
            typedef rbtree<Key, Value, KeyOfValue, Less_Compare, Equal_Compare> self;
            typedef allocator<__rbtree_node<Value> > Value_Alloc;
            #define red   node_type::red
            #define black node_type::black

            link_type     root;
            KeyOfValue    key;
            Less_Compare  less_compare;
            Equal_Compare equal_compare;
        public:
            typedef size_t                         size_type;
            typedef ptrdiff_t                      difference_type;
            typedef __rbtree_iterator<Value>       iterator;
            typedef __rbtree_const_iterator<Value> const_iterator;
        protected:
            size_type node_count;
            link_type header;
        public:
            rbtree() : root(NULL), node_count(0)
            {
                header = Value_Alloc::allocate();
                header->color = red;
                header->parent = root;
                header->left = header;
                header->right = header;
            }

            ~rbtree()
            {
                clear();
            }

            inline iterator insert_equal(const value_type& x)
            {
                return insert_equal(x, root, NULL);
            }

            inline pair<iterator, bool> insert_unique(const value_type& x)
            {
                return insert_unique(x, root, NULL);
            }

            inline iterator erase(const key_type& x)
            {
                iterator i = find(x);
                erase(i.node);
                return i;
            }

            inline iterator find(const key_type& x)
            {
                return find(x, root);
            }

            inline void clear()
            {
                clear(root);
                root = NULL;
                header->parent = root;
                header->left = header;
                header->right = header;
                node_count = 0;
            }

            inline iterator maximum()const
            {
                return maximum(root);
            }

            inline iterator minimum()const
            {
                return minimum(root);
            }

            inline iterator begin()
            {
                return header->left;
            }

            inline const_iterator begin()const
            {
                return header->left;
            }

            inline iterator end()
            {
                return header;
            }

            inline const_iterator end()const
            {
                return header;
            }

            inline self& operator<<(const pair<bool, const value_type&> x)
            {
                if(x.first) insert_equal(x.second, root, NULL);
                else insert_unique(x.second, root, NULL);
                return *this;
            }
        protected:
            inline iterator find(const key_type& x, link_type node)
            {
                if(node)
                {
                    if(less_compare(key(x), key(node->data))) return find(x, left(node));
                    else if(equal_compare(key(x), key(node->data))) return node;
                    else return find(x, right(node));
                }
                return NULL;
            }

            iterator insert_equal(const value_type& x, link_type& node, link_type _parent)
            {
                if(node != NULL)
                {
                    if(less_compare(key(x), key(node->data))) return insert_equal(x, left(node), node);
                    else return insert_equal(x, right(node), node);
                }
                else
                {
                    node = Value_Alloc::allocate();
                    construct(node, x);
                    parent(node) = _parent;
                    insert_rebalance(node, root);
                    ++node_count;
                    if(node == root)
                    {
                        header->parent = node;
                        node->parent = header;
                    }
                    header->left = minimum(root);
                    header->right = maximum(root);
                    return iterator(node);
                }
            }

            pair<iterator, bool> insert_unique(const value_type& x, link_type& node, link_type _parent)
            {
                if(node != NULL)
                {
                    if(less_compare(key(x), key(node->data))) return insert_unique(x, left(node), node);
                    else if(equal_compare(key(x), key(node->data))) return pair<iterator, bool>(node, false);
                    else return insert_unique(x, right(node), node);
                }
                else
                {
                    node = Value_Alloc::allocate();
                    construct(node, x);
                    parent(node) = _parent;
                    insert_rebalance(node, root);
                    ++node_count;
                    if(node == root)
                    {
                        header->parent = node;
                        node->parent = header;
                    }
                    header->left = minimum(root);
                    header->right = maximum(root);
                    return pair<iterator, bool>(node, true);
                }
            }

            void erase(link_type z)
            {
                // y为被删除的节点
                // x为被删除的节点的孩子
                link_type y = z, x = NULL, x_parent = NULL;
                if(!left(y)) x = right(y);
                else
                {
                    if(!right(y)) x = left(y);
                    else
                    {
                        y = right(y);
                        while(left(y)) y = left(y);
                        x = right(y);
                    }
                }

                if(y != z)
                {
                    // 将y替换为被删除的节点
                    parent(left(z)) = y;
                    left(y) = left(z);

                    if(y != right(z)) // 被删除的节点作为根的子树的高度>1
                    {
                        x_parent = parent(y);
                        if(x) parent(x) = x_parent;
                        left(x_parent) = x;

                        // 将y替换为要删除的节点
                        right(y) = right(z);
                        parent(right(z)) = y;
                    }
                    else x_parent = y;

                    if(root == z) root = y;
                    else if(left(parent(z)) == z) left(parent(z)) = y;
                    else right(parent(z)) = y;

                    parent(y) = parent(z);

                    // 交换要删除节点和被删除节点的颜色
                    typename node_type::color_type tmp = y->color;
                    y->color = z->color;
                    z->color = tmp;

                    // 重设y为要删除的点
                    y = z;
                }
                else
                {
                    // 被删除节点只有一个子节点，直接将其子节点替换为被删除节点即可
                    x_parent = parent(y);
                    if(x) parent(x) = parent(y);
                    if(root == z) root = x;
                    else
                    {
                        if(left(parent(z)) == z) left(parent(z)) = x;
                        else right(parent(z)) = x;
                    }
                }

                if(y->color == black) // 要删除的那个点为黑色
                {
                    while(x != root && (!x || x->color == black)) // 迭代直到x不为黑色或根节点为止
                    {
                        if(left(x_parent) == x) // 被删除节点为左孩子
                        {
                            link_type w = right(x_parent); // 其兄弟节点
                            // case1
                            if(w->color == red)
                            {
                                w->color = black;
                                x_parent->color = red;
                                l_rotate(x_parent, root);
                                w = right(x_parent);
                            }

                            // case2
                            if((!left(w) || left(w)->color == black) &&
                                (!right(w) || right(w)->color == black))
                            {
                                w->color = red;
                                x = x_parent;
                                x_parent = parent(x_parent);
                            }
                            else
                            {
                                // case3
                                if(!right(w) || right(w)->color == black)
                                {
                                    if(left(w)) left(w)->color = black;
                                    w->color = red;
                                    r_rotate(w, root);
                                    w = right(x_parent);
                                }

                                // case4
                                w->color = x_parent->color;
                                x_parent->color = black;
                                if(right(w)) right(w)->color = black;
                                l_rotate(x_parent, root);
                                break;
                            }
                        }
                        else // 被删除节点为右孩子
                        {
                            link_type w = left(x_parent);
                            if(w->color == red)
                            {
                                w->color = black;
                                x_parent->color = red;
                                r_rotate(x_parent, root);
                                w = left(x_parent);
                            }

                            if((!right(w) || right(w)->color == black) &&
                                (!left(w) || left(w)->color == black))
                            {
                                w->color = red;
                                x = x_parent;
                                x_parent = parent(x_parent);
                            }
                            else
                            {
                                if(!left(w) || left(w)->color == black)
                                {
                                    if(right(w)) right(w)->color = black;
                                    w->color = red;
                                    l_rotate(w, root);
                                    w = left(x_parent);
                                }

                                w->color = x_parent->color;
                                x_parent->color = black;
                                if(left(w)) left(w)->color = black;
                                r_rotate(x_parent, root);
                                break;
                            }
                        }
                    }
                    if(x) x->color = black;
                }
                destruct(&y->data, has_destruct(y->data));
                Value_Alloc::deallocate(y);
                --node_count;
            }

            void insert_rebalance(link_type node, link_type& root)
            {
                while(node != root && parent(node)->color == red)
                {
                    link_type u = uncle(node), p = parent(node), g = parent(p);
                    if(u != NULL && u->color == red)
                    {
                        // 若父节点和叔父节点都为红色，则
                        // 1.直接修改这2个节点的颜色和祖父节点的颜色
                        // 2.递归向上调整
                        p->color = black;
                        u->color = black;
                        g->color = red;
                        node = g;
                    }
                    else if(p == left(g))
                    {
                        if(node == right(p)) // 内侧旋转
                        {
                            node = p;
                            l_rotate(node, root);
                        }
                        p = parent(node);
                        g = parent(p);
                        p->color = black;
                        g->color = red;
                        r_rotate(g, root); // 外侧旋转
                    }
                    else
                    {
                        if(node == left(p))
                        {
                            node = p;
                            r_rotate(node, root);
                        }
                        p = parent(node);
                        g = parent(p);
                        p->color = black;
                        g->color = red;
                        l_rotate(g, root);
                    }
                }
                root->color = black;
            }

            inline void clear(link_type node)
            {
                if(node != NULL)
                {
                    clear(left(node));
                    clear(right(node));
                    destruct(&node->data, has_destruct(node->data));
                    Value_Alloc::deallocate(node);
                }
            }

            inline static link_type maximum(link_type x)
            {
                while(right(x)) x = right(x);
                return x;
            }

            inline static link_type minimum(link_type x)
            {
                while(left(x)) x = left(x);
                return x;
            }

            inline static link_type& parent(link_type node)
            {
                return node->parent;
            }

            inline static link_type& left(link_type node)
            {
                return node->left;
            }

            inline static link_type& right(link_type node)
            {
                return node->right;
            }

            inline static link_type& grandparent(link_type node)
            {
                return parent(parent(node));
            }

            inline static link_type& uncle(link_type node)
            {
                link_type g = grandparent(node);
                return left(g) == parent(node) ? right(g) : left(g);
            }

            inline static void r_rotate(link_type node, link_type& root)
            {
                link_type tmp = left(node);
                left(node) = right(tmp);
                if(right(tmp) != NULL) parent(right(tmp)) = node;
                parent(tmp) = parent(node);

                if(node == root) root = tmp;
                else if(node == left(parent(node))) left(parent(node)) = tmp;
                else right(parent(node)) = tmp;
                right(tmp) = node;
                parent(node) = tmp;
            }

            inline static void l_rotate(link_type node, link_type& root)
            {
                link_type tmp = right(node);
                right(node) = left(tmp);
                if(left(tmp) != NULL) parent(left(tmp)) = node;
                parent(tmp) = parent(node);

                if(node == root) root = tmp;
                else if(node == left(parent(node))) left(parent(node)) = tmp;
                else right(parent(node)) = tmp;
                left(tmp) = node;
                parent(node) = tmp;
            }
        };
    }
}

#endif
