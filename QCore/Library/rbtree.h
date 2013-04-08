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

    __rbtree_node(const T& x) : data(x), color(red)
    {
        parent = left = right = NULL;
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

    inline T* operator->()const
    {
        return &node->data;
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
    typedef __reverse_iterator<const_iterator, value_type, size_type, distance_type> const_reverse_iterator;
    typedef __reverse_iterator<iterator, value_type, size_type, distance_type> reverse_iterator;
protected:
    size_type node_count;
    link_type header;
public:
    rbtree() : node_count(0)
    {
        header = Value_Alloc::allocate();
        header->color  = red;
        header->parent = NULL;
        header->left   = header;
        header->right  = header;
    }

    rbtree(const self& x)
    {
        header = Value_Alloc::allocate();
        header->color  = red;
        header->parent = NULL;
        header->left   = header;
        header->right  = header;
        copyFrom(x);
    }

    ~rbtree()
    {
        clear();
		Value_Alloc::deallocate(header);
    }

    inline iterator insert_equal(const value_type& x)
    {
        return insert_equal(x, header->parent, NULL);
    }

    inline pair<iterator, bool> insert_unique(const value_type& x)
    {
        return insert_unique(x, header->parent, NULL);
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
        return find(x, header->parent);
    }

    inline const_iterator find(const key_type& x)const
    {
        return find(x, header->parent);
    }

    inline void clear()
    {
        clear(header->parent);
        header->parent = NULL;
        header->left   = header;
        header->left  = header;
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
        const_iterator i = maximum(header->parent);
        return i == NULL ? header : i;
    }

    inline const_iterator minimum()const
    {
        const_iterator i = minimum(header->parent);
        return i == NULL ? header : i;
    }

    inline iterator begin()
    {
        return header->left;
    }

    inline const_iterator begin()const
    {
        return header->left;
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
        link_type z = y->parent;

        while(z)
        {
            if(!less_compare(key(z->data), x)) y = z, z = z->left;
            else z = z->right;
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
            else z = z->right;
        }
        return y;
    }

    const_iterator upper_bound(const key_type& x)const
    {
        link_type y = header;
        link_type z = y->parent;

        while(z)
        {
            if(less_compare(x, key(z->data))) y = z, z = z->left;
            else z = z->right;
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

    self& operator=(const self& x)
    {
        if (&x != this)
        {
            clear();
            copyFrom(x);
        }
        return *this;
    }

    inline const bool operator==(const self& x)const
    {
        if (node_count != x.node_count) return false;
        return equalNode(header->parent, x.header->parent);
    }

    template <typename T>
    inline const bool operator==(const rbtree<Key, T, select1st<T>, Less_Compare, Equal_Compare>& x)const
    {
        if (node_count != x.size()) return false;
        return equalNode(header->parent, x.root(), x._key());
    }

    inline const link_type root()const
    {
        return header->parent;
    }

    inline const KeyOfValue& _key()const
    {
        return key;
    }

    void copyFrom(const self& x)
    {
        if (x.header->parent)
        {
            link_type node = Value_Alloc::allocate();

            construct(node, x.header->parent->data);
            node->color  = x.header->parent->color;
            node->parent = header;

            copyFrom(node->left, x.header->parent->left, node);
            copyFrom(node->right, x.header->parent->right, node);

            header->parent = node;
            header->left   = minimum(node);
            header->right  = maximum(node);
        }
        node_count    = x.node_count;
        equal_compare = x.equal_compare;
        less_compare  = x.less_compare;
        key           = x.key;
    }
protected:
    inline iterator find(const key_type& x, link_type node)
    {
        if(node)
        {
            if(less_compare(x, key(node->data))) return find(x, node->left);
            else if(equal_compare(x, key(node->data))) return node;
            else return find(x, node->right);
        }
        return NULL;
    }

    iterator insert_equal(const value_type& x, link_type& node, link_type _parent)
    {
        if(node != NULL)
        {
            if(less_compare(key(x), key(node->data))) return insert_equal(x, node->left, node);
            else return insert_equal(x, node->right, node);
        }
        else
        {
            node = Value_Alloc::allocate();
            construct(node, x);
            if(_parent == NULL) // root node
            {
                node->parent = header;
                header->parent = node;
                header->left = node;
                header->right = node;
            }
            else if(_parent == header->left && node == _parent->left)
            {
                header->left = node;
            }
            else if(_parent == header->right && node == _parent->right)
            {
                header->right = node;
            }
            if(_parent) node->parent = _parent;
            insert_rebalance(node, header->parent);
            ++node_count;
            return iterator(node);
        }
    }

    pair<iterator, bool> insert_unique(const value_type& x, link_type& node, link_type _parent)
    {
        if(node != NULL)
        {
            if(less_compare(key(x), key(node->data))) return insert_unique(x, node->left, node);
            else if(equal_compare(key(x), key(node->data))) return pair<iterator, bool>(node, false);
            else return insert_unique(x, node->right, node);
        }
        else
        {
            link_type pNode = node = Value_Alloc::allocate();
            construct(node, x);
            if(_parent == NULL) // root node
            {
                node->parent   = header;
                header->parent = node;
                header->left   = node;
                header->right  = node;
            }
            else if(_parent == header->left && node == _parent->left)
            {
                header->left = node;
            }
            else if(_parent == header->right && node == _parent->right)
            {
                header->right = node;
            }
            if(_parent) node->parent = _parent;
            insert_rebalance(pNode, header->parent);
            ++node_count;
            return pair<iterator, bool>(pNode, true);
        }
    }

    void erase(link_type z)
    {
        // y为被删除的节点
        // x为被删除的节点的孩子
        link_type y = z, x = NULL, x_parent = NULL;
        if(y->left == NULL) x = y->right;
        else if(y->right == NULL) x = y->left;
        else
        {
            y = y->right;
            while(y->left) y = y->left;
            x = y->right;
        }

        if(y != z)
        {
            // 将y替换为被删除的节点
            z->left->parent = y;
            y->left = z->left;

            if(y != z->right) // 被删除的节点作为根的子树的高度>1
            {
                x_parent = y->parent;
                if(x) x->parent = y->parent;
                y->parent->left = x;

                // 将y替换为要删除的节点
                y->right = z->right;
                z->right->parent = y;
            }
            else x_parent = y;

            if (z == header->parent) header->parent = y;
            else if(z->parent->left == z) z->parent->left = y;
            else z->parent->right = y;

            y->parent = z->parent;

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
            x_parent = y->parent;
            if(x) x->parent = y->parent;

            if (z == header->parent) header->parent = x;
            else if(z->parent->left == z) z->parent->left = x;
            else z->parent->right = x;
            if (header->left == z)
            {
                if(z->right == NULL) header->left = z->parent;
                else header->left = minimum(x);
            }
            if (header->right == z)
            {
                if(z->left == NULL) header->right = z->parent;
                else header->right = maximum(x);
            }
        }

        if(y->color == black) // 要删除的那个点为黑色
        {
            while(x != header->parent && (x == NULL || x->color == black)) // 迭代直到x不为黑色或根节点为止
            {
                if(x_parent->left == x) // 被删除节点为左孩子
                {
                    link_type w = x_parent->right; // 其兄弟节点
                    // case1
                    if(w->color == red)
                    {
                        w->color = black;
                        x_parent->color = red;
                        l_rotate(x_parent, header->parent);
                        w = x_parent->right;
                    }

                    if (w == NULL) break;

                    // case2
                    if((w->left == NULL || w->left->color == black) &&
                       (w->right == NULL || w->right->color == black))
                    {
                        w->color = red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        // case3
                        if(w->right == NULL || w->right->color == black)
                        {
                            if(w->left) w->left->color = black;
                            w->color = red;
                            r_rotate(w, header->parent);
                            w = x_parent->right;
                        }

                        // case4
                        w->color = x_parent->color;
                        x_parent->color = black;
                        if(w->right) w->right->color = black;
                        l_rotate(x_parent, header->parent);
                        break;
                    }
                }
                else // 被删除节点为右孩子
                {
                    link_type w = x_parent->left;
                    if(w->color == red)
                    {
                        w->color = black;
                        x_parent->color = red;
                        r_rotate(x_parent, header->parent);
                        w = x_parent->left;
                    }

                    if (w == NULL) break;

                    if((w->right == NULL || w->right->color == black) &&
                       (w->left == NULL || w->left->color == black))
                    {
                        w->color = red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else
                    {
                        if(w->left == NULL || w->left->color == black)
                        {
                            if(w->right) w->right->color = black;
                            w->color = red;
                            l_rotate(w, header->parent);
                            w = x_parent->left;
                        }

                        w->color = x_parent->color;
                        x_parent->color = black;
                        if(w->left) w->left->color = black;
                        r_rotate(x_parent, header->parent);
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
        while(node != root && node->parent->color == red)
        {
            link_type p = node->parent, g = p->parent, u = NULL;
            if (p == g->left)
            {
                u = g->right;
                if (u && u->color == red)
                {
                    p->color = black;
                    u->color = black;
                    g->color = red;
                    node = g;
                }
                else
                {
                    if (node == p->right)
                    {
                        node = p;
                        l_rotate(node, root);
                    }
                    node->parent->color = black;
                    node->parent->parent->color = red;
                    r_rotate(node->parent->parent, root);
                }
            }
            else
            {
                u = g->left;
                if (u && u->color == red)
                {
                    p->color = black;
                    u->color = black;
                    g->color = red;
                    node = g;
                }
                else
                {
                    if (node == p->left)
                    {
                        node = node->parent;
                        r_rotate(node, root);
                    }
                    node->parent->color = black;
                    node->parent->parent->color = red;
                    l_rotate(node->parent->parent, root);
                }
            }
        }
        root->color = black;
    }

    inline void clear(link_type node)
    {
        if(node != NULL)
        {
            clear(node->left);
            clear(node->right);
            destruct(&node->data, has_destruct(node->data));
            Value_Alloc::deallocate(node);
        }
    }

    inline static link_type maximum(link_type x)
    {
        if(!x) return NULL;

        while(x->right) x = x->right;
        return x;
    }

    inline static link_type minimum(link_type x)
    {
        if(!x) return NULL;

        while(x->left) x = x->left;
        return x;
    }

    void copyFrom(link_type& node, const link_type from, link_type parent)
    {
        if (from)
        {
            node = Value_Alloc::allocate();

            construct(node, from->data);
            node->parent = parent;
            node->color = from->color;

            copyFrom(node->left, from->left, node);
            copyFrom(node->right, from->right, node);
        }
    }

    const bool equalNode(link_type x, link_type y)const
    {
        if (x == NULL)
        {
            if (y != NULL) return false;
            return true;
        }
        else if (y == NULL)
        {
            if (x != NULL) return false;
            return true;
        }
        if (x->data != y->data) return false;
        if (!equalNode(x->left, y->left))     return false;
        if (!equalNode(x->right, y ->right))  return false;
        return true;
    }

    template <typename T, typename Key>
    const bool equalNode(link_type x, __rbtree_node<T>* y, const Key& ykey)const
    {
        if (x == NULL)
        {
            if (y != NULL) return false;
            else return true;
        }
        else if (y == NULL)
        {
            if (x != NULL) return false;
            return true;
        }
        if (!equal_compare(key(x->data), ykey(y->data))) return false;
        if (!equalNode(x->left, y->left, ykey)) return false;
        if (!equalNode(x->right, y->right, ykey)) return false;
        return true;
    }

    static void r_rotate(link_type node, link_type& root)
    {
        link_type tmp = node->left;
        node->left = tmp->right;
        if(tmp->right != NULL) tmp->right->parent = node;
        tmp->parent = node->parent;

        if(node == root) root = tmp;
        else if(node == node->parent->left) node->parent->left = tmp;
        else node->parent->right = tmp;
        tmp->right = node;
        node->parent = tmp;
    }

    static void l_rotate(link_type node, link_type& root)
    {
        link_type tmp = node->right;
        node->right = tmp->left;
        if(tmp->left != NULL) tmp->left->parent = node;
        tmp->parent = node->parent;

        if(node == root) root = tmp;
        else if(node == node->parent->left) node->parent->left = tmp;
        else node->parent->right = tmp;
        tmp->left = node;
        node->parent = tmp;
    }
#undef red
#undef black
};

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
