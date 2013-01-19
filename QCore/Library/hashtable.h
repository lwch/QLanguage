/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:33
	filename: 	\QCore\Library\hashtable.h
	file path:	\QCore\Library
	file base:	hashtable
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_HASHTABLE_H_
#define _QLANGUAGE_LIBRARY_HASHTABLE_H_

#include "definition.h"
#include "hash.h"
#include "vector.h"

#include "pair.h"
#include "function.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

#define HASHTABLE_TEMPLATE_ACHIEVE Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Compare
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class __hashtable_bucket_node
{
protected:
    typedef __hashtable_bucket_node<T> self;
public:
    typedef T     value_type;
    typedef self* link_type;

    value_type data;
    link_type  prev;
    link_type  next;

    __hashtable_bucket_node(const T& x) : data(x)
    {
        prev = next = NULL;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length,
    bool Resize,
    typename Hash,
    typename Compare>
class hashtable;
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length,
    bool Resize,
    typename Hash,
    typename Compare>
class __hashtable_iterator_base : public bidirectional_iterator
{
    template <typename K, typename V, typename KOV,
        size_t MBL,
        bool R,
        typename H,
        typename C>
        friend class hashtable;
protected:
    typedef hashtable<HASHTABLE_TEMPLATE_ACHIEVE> hashtable_type;
    typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE> self;
    typedef __hashtable_bucket_node<Value>        node_type;

    node_type*      node;
    hashtable_type& ht;
public:
    // I have no default construct.
    // Because iterator must belong to hashtable.
    __hashtable_iterator_base(node_type* node, hashtable_type& hashtable) : node(node), ht(hashtable) {}

    inline const bool operator==(const self& x)const
    {
        return node == x.node;
    }

    inline const bool operator!=(const self& x)const
    {
        return node != x.node;
    }

    inline typename __container_traits<hashtable_type>::reference operator*()const
    {
        return node->data;
    }

    inline self& operator++()
    {
        inc();
        return *this;
    }

    inline self operator++(int)
    {
        self tmp = *this;
        ++(*this);
        return tmp;
    }

    inline self& operator--()
    {
        dec();
        return *this;
    }

    inline self operator--(int)
    {
        self tmp = *this;
        --(*this);
        return tmp;
    }
protected:
    void inc()
    {
        node_type* prev = node;
        node = node->next;
        if (node == NULL)
        {
            HASH_KEY_TYPE bucket = ht.index(node->data);
            typename __container_traits<typename hashtable_type::buckets_type>::size_type size = ht.buckets.size();
            while(++bucket < size && node = reinterpret_cast<typename hashtable_type::node_size_type*>(ht[bucket])->next && node == ht[bucket]);
        }
    }

    void dec()
    {
        HASH_KEY_TYPE idx = ht.index(ht.key(node->data));
        node_type* prev = node;
        node = node->prev;
        if (node == ht.buckets[idx])
        {
            while (--idx >= 0 && reinterpret_cast<typename hashtable_type::node_size_type*>(ht.buckets[idx])->data > 0)
            {
                typedef typename hashtable_type::link_type link_type;
                typedef typename hashtable_type::node_size_type node_size_type;
                link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(ht.buckets[idx])->next);
                while (current) current = current->next;
                node = current;
                return;
            }
            node = NULL;
        }
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length,
    bool Resize,
    typename Hash,
    typename Compare,
    typename Size, typename Distance>
class __hashtable_iterator : public __iterator<typename __container_traits<hashtable<HASHTABLE_TEMPLATE_ACHIEVE> >::value_type, Size, Distance>
    , public __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>
{
protected:
    typedef __hashtable_iterator<HASHTABLE_TEMPLATE_ACHIEVE, Size, Distance> self;
    typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>            base;
public:
    __hashtable_iterator(typename base::node_type* node, typename base::hashtable_type& hashtable) : base(node, hashtable) {}

    inline typename __container_traits<typename base::hashtable_type>::reference operator*()const
    {
        return *(*this);
    }

    inline self& operator++()
    {
        return ++(*this);
    }

    inline self operator++(int)
    {
        return (*this)++;
    }

    inline self& operator--()
    {
        return --(*this);
    }

    inline self operator--(int)
    {
        return (*this)--;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length,
    bool Resize,
    typename Hash,
    typename Compare,
    typename Size, typename Distance>
class __hashtable_const_iterator : public __const_iterator<typename __container_traits<hashtable<HASHTABLE_TEMPLATE_ACHIEVE> >::value_type, Size, Distance>
    , public __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>
{
protected:
    typedef __hashtable_const_iterator<HASHTABLE_TEMPLATE_ACHIEVE, Size, Distance> self;
    typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>                  base;
public:
    __hashtable_const_iterator(typename base::node_type* node, typename base::hashtable_type& hashtable) : base(node, hashtable) {}

    inline typename __container_traits<typename base::hashtable_type>::reference operator*()const
    {
        return *(*this);
    }

    inline self& operator++()
    {
        return ++(*this);
    }

    inline self operator++(int)
    {
        return (*this)++;
    }

    inline self& operator--()
    {
        return --(*this);
    }

    inline self operator--(int)
    {
        return (*this)--;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length = 11,
    bool Resize = true,
    typename Hash = hash<Key>,
    typename Compare = equal_to<Key> >
class hashtable
{
protected:
    typedef Key                            key_type;
    typedef __hashtable_bucket_node<Value> node_type;
    typedef node_type* link_type;
    typedef hashtable<Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Compare> self;
    typedef allocator<__hashtable_bucket_node<Value> > Alloc;
    typedef __hashtable_bucket_node<size_t> node_size_type;
    typedef allocator<node_size_type> Node_Size_Alloc;
public:
    typedef Value        value_type;
    typedef Value*       pointer;
    typedef Value&       reference;
    typedef const Value& const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    distance_type;
    typedef __hashtable_iterator<HASHTABLE_TEMPLATE_ACHIEVE, size_type, distance_type> iterator;
    typedef __hashtable_const_iterator<HASHTABLE_TEMPLATE_ACHIEVE, size_type, distance_type> const_iterator;
    typedef __reverse_iterator<const_iterator, value_type, size_type, distance_type> const_reverse_iterator;
    typedef __reverse_iterator<iterator, value_type, size_type, distance_type> reverse_iterator;

    typedef vector<link_type> buckets_type;
protected:
    buckets_type buckets;
    size_type    length;
    KeyOfValue   key;
    Hash         hash;
    Compare      compare;
public:
    hashtable() : length(0)
    {
        const size_type defaultSize = 11;
        buckets.reserve(defaultSize);
        for(typename __container_traits<buckets_type>::size_type i = 0; i < defaultSize; ++i)
        {
            node_size_type* node = Node_Size_Alloc::allocate();
            construct(node, node_size_type(0));
            buckets.push_back(reinterpret_cast<link_type>(node));
        }
    }

    hashtable(size_type size) : length(0)
    {
        buckets.reserve(size);
        for(typename __container_traits<buckets_type>::size_type i = 0; i < size; ++i)
        {
            node_size_type* node = Node_Size_Alloc::allocate();
            construct(node, 0);
            buckets.push_back(reinterpret_cast<link_type>(node));
        }
    }

    ~hashtable()
    {
        clear();
        for(typename __container_traits<buckets_type>::size_type i = 0; i < buckets.size(); ++i)
        {
            Node_Size_Alloc::deallocate(reinterpret_cast<node_size_type*>(buckets[i]));
        }
    }

    inline size_type size()const
    {
        return length;
    }

    inline bool empty()const
    {
        return length == 0;
    }

    inline typename __container_traits<buckets_type>::size_type bucket_size()const
    {
        return buckets.size();
    }

    void clear()
    {
        for(typename __container_traits<buckets_type>::iterator i = buckets.begin(); i != buckets.end(); ++i)
        {
            link_type header = *i;
            link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(*i))->next;
            while(current)
            {
                link_type next = current->next;
                destruct(&current->data, has_destruct(current->data));
                Alloc::deallocate(current);
                current = next;
            }
            reinterpret_cast<node_size_type*>(*i)->data = 0;
            reinterpret_cast<node_size_type*>(*i)->next = NULL;
        }
        length = 0;
    }

    void resize()
    {
        const typename __container_traits<buckets_type>::size_type old_size = buckets.size();
        const typename __container_traits<buckets_type>::size_type new_size = old_size == 0 ? 2 : old_size * 2;
        buckets_type tmp(new_size);

        for (typename __container_traits<buckets_type>::size_type i = 0; i < new_size; ++i)
        {
            node_size_type* pNode = Node_Size_Alloc::allocate();
            construct(pNode, 0);
            tmp[i] = reinterpret_cast<link_type>(pNode);
        }

        for (typename __container_traits<buckets_type>::iterator i = buckets.begin(), m = buckets.end(); i != m; ++i)
        {
            node_size_type* head = reinterpret_cast<node_size_type*>(*i);
            link_type current = reinterpret_cast<link_type>(head->next);
            while (current)
            {
                link_type next = current->next;
                HASH_KEY_TYPE idx = index(current->data, new_size);
                current->next = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(tmp[idx])->next);
                current->prev = tmp[idx];
                if (reinterpret_cast<node_size_type*>(tmp[idx])->next) reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(tmp[idx])->next)->prev = current;
                reinterpret_cast<node_size_type*>(tmp[idx])->next = reinterpret_cast<node_size_type*>(current);
                current = next;
            }
        }
        swap(buckets, tmp);
    }

    inline iterator insert_equal(const value_type& x)
    {
        if (willResize(index(x, buckets.size()))) resize();
        return insert_equal_noresize(x);
    }

    inline void insert_equal(const value_type* first, const value_type* last)
    {
        while (first != last)
        {
            insert_equal(*first);
            ++first;
        }
    }

    inline void insert_equal(const_iterator first, const_iterator last)
    {
        while (first != last)
        {
            insert_equal(*first);
            ++first;
        }
    }

    inline pair<iterator, bool> insert_unique(const value_type& x)
    {
        if (willResize(index(x, buckets.size()))) resize();
        return insert_unique_noresize(x);
    }

    inline void insert_unique(const value_type* first, const value_type* last)
    {
        while (first != last)
        {
            insert_unique(*first);
            ++first;
        }
    }

    inline void insert_unique(const_iterator first, const_iterator last)
    {
        while (first != last)
        {
            insert_unique(*first);
            ++first;
        }
    }

    void erase(iterator position)
    {
        typename iterator::node_type* pNode = position.node;
        key_type k = key(pNode->data);
        if (pNode)
        {
            node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(pNode->data, buckets.size())]);
            link_type current = reinterpret_cast<link_type>(head->next);
            if (current == pNode)
            {
                head->next = reinterpret_cast<node_size_type*>(current->next);
                if (current->next) current->next->prev = reinterpret_cast<link_type>(head);
                destruct(&current->data, has_destruct(&current->data));
                Alloc::deallocate(current);
                --head->data;
                --length;
            }
            else
            {
                link_type next = current->next;
                while (next)
                {
                    if (pNode == next)
                    {
                        current->next = next->next;
                        if (next->next) next->next->prev = current;
                        destruct(&next->data, has_destruct(&next->data));
                        Alloc::deallocate(next);
                        --head->data;
                        --length;
                        break;
                    }
                    current = next;
                    next = current->next;
                }
            }
        }
    }

    iterator find(const key_type& k)
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[idx])->next);
        while (current)
        {
            if (compare(k, key(current->data))) break;
            current = current->next;
        }
        return iterator(current, *this);
    }

    const_iterator find(const key_type& k)const
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[idx])->next);
        while (current)
        {
            if (compare(k, key(current->data))) break;
            current = current->next;
        }
        return const_iterator(current, *this);
    }

    size_type count(const key_type& k)const
    {
        HASH_KEY_TYPE idx = index(k);
        size_type n = 0;
        link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[idx])->next);
        bool bStart = false;

        while (current)
        {
            if (compare(k, key(current->data)))
            {
                ++n;
                bStart = true;
            }
            else if (bStart) break;
            current = current->next;
        }
        return n;
    }

    pair<iterator, iterator> equal_range(const key_type& k)
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[idx])->next);
        bool bStart = false;

        link_type first = NULL;

        while (current)
        {
            if (compare(k, key(current->data)))
            {
                first = current;
                bStart = true;
            }
            else if (bStart) return pair<iterator, iterator>(iterator(first, *this), iterator(current, *this));
            current = current->next;
        }
        if (current) return pair<iterator, iterator>(iterator(current, *this), ++iterator(current, *this));
        else return pair<iterator, iterator>(end(), end());
    }

    pair<const_iterator, const_iterator> equal_range(const key_type& k)const
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[idx])->next);
        bool bStart = false;

        link_type first = NULL;

        while (current)
        {
            if (compare(k, key(current->data)))
            {
                first = current;
                bStart = true;
            }
            else if (bStart) return pair<const_iterator, const_iterator>(const_iterator(first, *this), const_iterator(current, *this));
            current = current->next;
        }
        if (current) return pair<const_iterator, const_iterator>(const_iterator(current, *this), ++const_iterator(current, *this));
        else return pair<const_iterator, const_iterator>(end(), end());
    }

    iterator begin()
    {
        for (typename __container_traits<buckets_type>::size_type i = 0; i < buckets.size(); ++i)
        {
            if (reinterpret_cast<node_size_type*>(buckets[i])->data > 0)
            {
                return iterator(reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[i])->next), *this);
            }
        }
        return end();
    }

    const_iterator begin()const
    {
        for (typename __container_traits<buckets_type>::size_type i = 0; i < buckets.size(); ++i)
        {
            if (reinterpret_cast<node_size_type*>(buckets[i])->data > 0)
            {
                return const_iterator(reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[i])->next), *this);
            }
        }
        return end();
    }

    reverse_iterator rbegin()
    {
        for (typename __container_traits<buckets_type>::size_type i = buckets.size() - 1; i >= 0; --i)
        {
            if (reinterpret_cast<node_size_type*>(buckets[i])->data > 0)
            {
                link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[i])->next);
                while (current->next) current = current->next;
                return reverse_iterator(iterator(current, *this));
            }
        }
        return rend();
    }

    const_reverse_iterator rbegin()const
    {
        for (typename __container_traits<buckets_type>::size_type i = buckets.size() - 1; i >= 0; --i)
        {
            if (reinterpret_cast<node_size_type*>(buckets[i])->data > 0)
            {
                link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[i])->next);
                while (current->next) current = current->next;
                return const_reverse_iterator(const_iterator(current, *this));
            }
        }
        return rend();
    }

    inline iterator end()
    {
        return iterator(0, *this);
    }

    inline const_iterator end()const
    {
        return const_iterator(0, *this);
    }

    inline reverse_iterator rend()
    {
        return reverse_iterator(end());
    }

    inline const_reverse_iterator rend()const
    {
        return const_reverse_iterator(end());
    }
protected:
    inline HASH_KEY_TYPE index(const key_type& k)
    {
        return hash(k) % buckets.size();
    }

    inline HASH_KEY_TYPE index(const value_type& x, typename __container_traits<buckets_type>::size_type sz)
    {
        return hash(key(x)) % sz;
    }

    inline bool willResize(HASH_KEY_TYPE bucket_index)
    {
        node_size_type* head = reinterpret_cast<node_size_type*>(buckets[bucket_index]);
        return head->data + 1 > Max_Bucket_Length;
    }

    iterator insert_equal_noresize(const value_type& x)
    {
        node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(x, buckets.size())]);
        link_type node = Alloc::allocate();
        construct(node, x);
        link_type current = reinterpret_cast<link_type>(head->next);
        key_type k = key(x);
        while (current)
        {
            if (compare(k, key(current->data)))
            {
                node->next = current->next;
                node->prev = current;
                if (current->next) current->next->prev = node;
                current->next = node;
                ++head->data;
                return iterator(node, *this);
            }
            current = current->next;
        }
        node->next = reinterpret_cast<link_type>(head->next);
        node->prev = reinterpret_cast<link_type>(head);
        if (head->next) reinterpret_cast<link_type>(head->next)->prev = node;
        head->next = reinterpret_cast<node_size_type*>(node);
        ++head->data;
        return iterator(node, *this);
    }

    pair<iterator, bool> insert_unique_noresize(const value_type& x)
    {
        node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(x, buckets.size())]);
        link_type current = reinterpret_cast<link_type>(head->next);
        while (current != reinterpret_cast<link_type>(head))
        {
            if (compare(key(x), key(current->data)))
            {
                return pair<iterator, bool>(iterator(current, *this), false);
            }
            current = current->next;
        }
        link_type node = Alloc::allocate();
        construct(node, x);
        node->next = reinterpret_cast<link_type>(head->next);
        node->prev = reinterpret_cast<link_type>(head);
        if (head->next) reinterpret_cast<link_type>(head->next)->prev = node;
        head->next = reinterpret_cast<node_size_type*>(node);
        ++head->data;
        return pair<iterator, bool>(iterator(node, *this), true);
    }
};

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
