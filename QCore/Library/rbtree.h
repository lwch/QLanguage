/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:36
	filename: 	\QCore\Library\rbtree.h
	file path:	\QCore\Library
	file base:	rbtree
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_RBTREE_H_
#define _QLANGUAGE_LIBRARY_RBTREE_H_

#include "definition.h"
#include "function.h"
#include "pair.h"
#include "iterator.h"
#include "construct.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

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
struct __rbtree_iterator_base : public bidirectional_iterator
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
            while(node == y->right)
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

    const bool operator==(const __rbtree_iterator_base<T>& x)const
    {
        return node == x.node;
    }

    const bool operator!=(const __rbtree_iterator_base<T>& x)const
    {
        return node != x.node;
    }
};

template <typename T, typename Size, typename Distance>
struct __rbtree_iterator : public __iterator<T, Size, Distance>, __rbtree_iterator_base<T>
{
protected:
    template <typename C, typename S, typename D>
    friend struct __rbtree_const_iterator;

    typedef __rbtree_iterator_base<T>            parent;
    typedef __iterator<T, Size, Distance>        iterator_parent;
    typedef __rbtree_iterator<T, Size, Distance> self;
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

    self& operator++()
    {
        parent::inc();
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--()
    {
        parent::dec();
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};

template <typename T, typename Size, typename Distance>
struct __rbtree_const_iterator : public __const_iterator<T, Size, Distance>, public __rbtree_iterator_base<T>
{
protected:
    typedef __rbtree_iterator<T, Size, Distance>       iterator;
    typedef __rbtree_iterator_base<T>                  parent;
    typedef __const_iterator<T, Size, Distance>        iterator_type;
    typedef __rbtree_const_iterator<T, Size, Distance> self;
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

    typename iterator_type::reference operator*()const
    {
        return parent::node->data;
    }

    self& operator++()
    {
        parent::inc();
        return *this;
    }

    self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    self& operator--()
    {
        parent::dec();
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
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
    typedef __rbtree_node<Value> node_type;
    typedef node_type*           link_type;
    typedef rbtree<Key, Value, KeyOfValue, Less_Compare, Equal_Compare> self;
    typedef allocator<__rbtree_node<Value> > Value_Alloc;
#define red   node_type::red
#define black node_type::black

    KeyOfValue    key;
    Less_Compare  less_compare;
    Equal_Compare equal_compare;
public:
    typedef Value                          value_type;
    typedef size_t                         size_type;
    typedef ptrdiff_t                      distance_type;
    typedef __rbtree_iterator<value_type, size_type, distance_type> iterator;
    typedef __rbtree_const_iterator<value_type, size_type, distance_type> const_iterator;
    typedef reverse_iterator<const_iterator, value_type, size_type, distance_type> const_reverse_iterator;
    typedef reverse_iterator<iterator, value_type, size_type, distance_type> reverse_iterator;
protected:
    size_type node_count;
    link_type header;
public:
    rbtree() : node_count(0)
    {
        header = Value_Alloc::allocate();
        header->color  = red;
        parent(header) = NULL;
        left(header)   = header;
        right(header)  = header;
    }

    ~rbtree()
    {
        clear();
		Value_Alloc::deallocate(header);
    }

    inline iterator insert_equal(const value_type& x)
    {
        return insert_equal(x, parent(header), NULL);
    }

    inline pair<iterator, bool> insert_unique(const value_type& x)
    {
        return insert_unique(x, parent(header), NULL);
    }

    inline void erase(iterator position)
    {
        erase(position.node);
    }

    inline void erase(const key_type& x)
    {
        iterator i = find(x);
        erase(i.node);
    }

    inline void erase(iterator first, iterator last)
    {
        if(first == begin() && last == end())
        {
            clear();
            return;
        }
        while(first != last) erase(first++);
    }

    inline iterator find(const key_type& x)
    {
        return find(x, parent(header));
    }

    inline const_iterator find(const key_type& x)const
    {
        return find(x, parent(header));
    }

    inline void clear()
    {
        clear(parent(header));
        parent(header) = NULL;
        left(header)   = header;
        right(header)  = header;
        node_count = 0;
    }

    inline size_type size()const
    {
        return node_count;
    }

    inline bool empty()const
    {
        return node_count == 0;
    }

    inline const_iterator maximum()const
    {
        const_iterator i = maximum(parent(header));
        return i == NULL ? header : i;
    }

    inline const_iterator minimum()const
    {
        const_iterator i = minimum(parent(header));
        return i == NULL ? header : i;
    }

    inline iterator begin()
    {
        return left(header);
    }

    inline const_iterator begin()const
    {
        return left(header);
    }

    inline reverse_iterator rbegin()
    {
        return header;
    }

    inline const_reverse_iterator rbegin()const
    {
        return header;
    }

    inline iterator end()
    {
        return header;
    }

    inline const_iterator end()const
    {
        return header;
    }

    inline reverse_iterator rend()
    {
        return left(header);
    }

    inline const_reverse_iterator rend()const
    {
        return left(header);
    }

    iterator lower_bound(const key_type& x)
    {
        link_type y = header;
        link_type z = parent(y);

        while(z)
        {
            if(!less_compare(key(z->data), x)) y = z, z = left(z);
            else z = right(z);
        }
        return y;
    }

    const_iterator lower_bound(const key_type& x)const
    {
        link_type y = header;
        link_type z = parent(y);

        while(z)
        {
            if(!less_compare(key(z->data), x)) y = z, z = left(z);
            else z = right(z);
        }
        return y;
    }

    iterator upper_bound(const key_type& x)
    {
        link_type y = header;
        link_type z = parent(y);

        while(z)
        {
            if(less_compare(x, key(z->data))) y = z, z = left(z);
            else z = right(z);
        }
        return y;
    }

    const_iterator upper_bound(const key_type& x)const
    {
        link_type y = header;
        link_type z = parent(y);

        while(z)
        {
            if(less_compare(x, key(z->data))) y = z, z = left(z);
            else z = right(z);
        }
        return y;
    }

    pair<iterator, iterator> equal_range(const key_type& x)
    {
        return pair<iterator, iterator>(lower_bound(x), upper_bound(x));
    }

    pair<const_iterator, const_iterator> equal_range(const key_type& x)const
    {
        return pair<const_iterator, const_iterator>(lower_bound(x), upper_bound(x));
    }

    size_type count(const key_type& x)const
    {
        pair<const_iterator, const_iterator> p = equal_range(x);
        size_type n = 0;
        distance(p.first, p.second, n);
        return n;
    }
protected:
    inline iterator find(const key_type& x, link_type node)
    {
        if(node)
        {
            if(less_compare(x, key(node->data))) return find(x, left(node));
            else if(equal_compare(x, key(node->data))) return node;
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
            if(_parent == NULL) // root node
            {
                parent(node) = header;
                parent(header) = node;
                left(header) = node;
                right(header) = node;
            }
            else if(_parent == left(header) && node == left(_parent))
            {
                left(header) = node;
            }
            else if(_parent == right(header) && node == right(_parent))
            {
                right(header) = node;
            }
            if(_parent) parent(node) = _parent;
            insert_rebalance(node, parent(header));
            ++node_count;
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
            if(_parent == NULL) // root node
            {
                parent(node) = header;
                parent(header) = node;
                left(header) = node;
                right(header) = node;
            }
            else if(_parent == left(header) && node == left(_parent))
            {
                left(header) = node;
            }
            else if(_parent == right(header) && node == right(_parent))
            {
                right(header) = node;
            }
            if(_parent) parent(node) = _parent;
            insert_rebalance(node, parent(header));
            ++node_count;
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
                if(x) parent(x) = parent(y);
                left(parent(y)) = x;

                // 将y替换为要删除的节点
                right(y) = right(z);
                parent(right(z)) = y;
            }
            else x_parent = y;

            if(left(parent(z)) == z) left(parent(z)) = y;
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
            if(left(parent(z)) == z) left(parent(z)) = x;
            else right(parent(z)) = x;
            if (left(header) == z)
            {
                if(!right(z)) left(header) = parent(z);
                else left(header) = minimum(x);
            }
            if (right(header) == z)
            {
                if(!left(z)) right(header) = parent(z);
                else right(header) = maximum(x);
            }
        }

        if(y->color == black) // 要删除的那个点为黑色
        {
            while(x != parent(header) && (!x || x->color == black)) // 迭代直到x不为黑色或根节点为止
            {
                if(left(x_parent) == x) // 被删除节点为左孩子
                {
                    link_type w = right(x_parent); // 其兄弟节点
                    // case1
                    if(w->color == red)
                    {
                        w->color = black;
                        x_parent->color = red;
                        l_rotate(x_parent, parent(header));
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
                            r_rotate(w, parent(header));
                            w = right(x_parent);
                        }

                        // case4
                        w->color = x_parent->color;
                        x_parent->color = black;
                        if(right(w)) right(w)->color = black;
                        l_rotate(x_parent, parent(header));
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
                        r_rotate(x_parent, parent(header));
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
                            l_rotate(w, parent(header));
                            w = left(x_parent);
                        }

                        w->color = x_parent->color;
                        x_parent->color = black;
                        if(left(w)) left(w)->color = black;
                        r_rotate(x_parent, parent(header));
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
        if(!x) return NULL;

        while(right(x)) x = right(x);
        return x;
    }

    inline static link_type minimum(link_type x)
    {
        if(!x) return NULL;

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

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
