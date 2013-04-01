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
template <typename T, typename HashTable_Type>
class __hashtable_iterator_base : public bidirectional_iterator
{
    template <typename K, typename V, typename KOV,
        size_t MBL,
        bool R,
        typename H,
        typename L,
        typename G,
        typename C>
    friend class hashtable;
protected:
    typedef __hashtable_iterator_base<T, HashTable_Type> self;
    typedef __hashtable_bucket_node<T>                   node_type;

    node_type*      node;
    HashTable_Type& ht;
public:
    // I have no default construct.
    // Because iterator must belong to hashtable.
    __hashtable_iterator_base(node_type* node, HashTable_Type& hashtable) : node(node), ht(hashtable) {}

    inline const bool operator==(const self& x)const
    {
        return node == x.node;
    }

    inline const bool operator!=(const self& x)const
    {
        return node != x.node;
    }

    inline typename __container_traits<HashTable_Type>::reference operator*()const
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

    inline T* operator->()
    {
        return &node->data;
    }
protected:
    void inc()
    {
        node_type* prev = node;
        node = node->next;
        if (node == NULL)
        {
            HASH_KEY_TYPE bucket = ht.index(ht.key(prev->data));
            typename __container_traits<typename HashTable_Type::buckets_type>::size_type size = ht.buckets.size();
            while(++bucket < size && (node = ht.buckets[bucket]) == NULL);
        }
    }

    void dec()
    {
        HASH_KEY_TYPE idx = ht.index(ht.key(node->data));
        node_type* prev = node;
        node = node->prev;
        if (node == NULL)
        {
            while (--idx >= 0 && reinterpret_cast<typename HashTable_Type::node_size_type*>(ht.buckets[idx])->data > 0)
            {
                typedef typename HashTable_Type::link_type link_type;
                typedef typename HashTable_Type::node_size_type node_size_type;
                link_type current = ht.buckets[idx];
                while (current) current = current->next;
                node = current;
                return;
            }
            node = NULL;
        }
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename HashTable_Type, typename Size, typename Distance>
class __hashtable_iterator : public __iterator<typename __container_traits<HashTable_Type>::value_type, Size, Distance>
    , public __hashtable_iterator_base<T, HashTable_Type>
{
protected:
    template <typename T1, typename H, typename S, typename D>
    friend class __hashtable_const_iterator;

    typedef __hashtable_iterator<T, HashTable_Type, Size, Distance> self;
    typedef __hashtable_iterator_base<T, HashTable_Type>            base;
public:
    __hashtable_iterator(typename base::node_type* node, HashTable_Type& hashtable) : base(node, hashtable) {}

    inline self& operator++()
    {
        base::inc();
        return *this;
    }

    inline self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    inline self& operator--()
    {
        base::dec();
        return *this;
    }

    inline self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T, typename HashTable_Type, typename Size, typename Distance>
class __hashtable_const_iterator : public __const_iterator<typename __container_traits<HashTable_Type>::value_type, Size, Distance>
    , public __hashtable_iterator_base<T, HashTable_Type>
{
protected:
    typedef __hashtable_iterator<T, HashTable_Type, Size, Distance>       iterator;
    typedef __hashtable_const_iterator<T, HashTable_Type, Size, Distance> self;
    typedef __hashtable_iterator_base<T, HashTable_Type>                  base;
public:
    __hashtable_const_iterator(typename base::node_type* node, HashTable_Type& hashtable) : base(node, hashtable) {}

    __hashtable_const_iterator(const iterator& i) : base(i.node, i.ht) {}

    inline self& operator++()
    {
        base::inc();
        return *this;
    }

    inline self operator++(int)
    {
        self tmp = *this;
        ++*this;
        return tmp;
    }

    inline self& operator--()
    {
        base::dec();
        return *this;
    }

    inline self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length = 11,
    bool Resize = true,
    typename Hash = hash<Key>,
    typename Less = less<Key>,
    typename Greater = greater<Key>,
    typename Compare = equal_to<Key> >
class hashtable
{
    typedef hashtable<Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Less, Greater, Compare> self;
    typedef Key                             key_type;
    typedef __hashtable_bucket_node<Value>  node_type;
    typedef node_type*                      link_type;
    typedef __hashtable_bucket_node<size_t> node_size_type;

    typedef allocator<node_type>            Alloc;
    typedef allocator<node_size_type>       Node_Size_Alloc;

    typedef vector<link_type>               buckets_type;
    typedef vector<size_t>                  buckets_length_type;

    template <typename T, typename H>
    friend class __hashtable_iterator_base;
public:
    typedef Value        value_type;
    typedef Value*       pointer;
    typedef Value&       reference;
    typedef const Value& const_reference;
    typedef size_t       size_type;
    typedef ptrdiff_t    distance_type;
    typedef __hashtable_iterator<value_type, self, size_type, distance_type> iterator;
    typedef __hashtable_const_iterator<value_type, self, size_type, distance_type> const_iterator;
    typedef __reverse_iterator<const_iterator, value_type, size_type, distance_type> const_reverse_iterator;
    typedef __reverse_iterator<iterator, value_type, size_type, distance_type> reverse_iterator;
protected:
    size_type           length;
    KeyOfValue          key;
    Hash                hash;
    Less                less;
    Greater             greater;
    Compare             compare;
    buckets_type        buckets;
    buckets_length_type buckets_length;
public:
    hashtable() : length(0)
    {
        static const size_type defaultSize = 11;
        buckets.reserve(defaultSize);
        buckets_length.reserve(defaultSize);

        for (typename __container_traits<buckets_type>::size_type i = 0; i < defaultSize; ++i)
        {
            buckets.push_back(NULL);
            buckets_length.push_back(0);
        }
    }

    // TODO
//     hashtable(size_type size) : length(0)
//     {
//         buckets.reserve(size);
//         for(typename __container_traits<buckets_type>::size_type i = 0; i < size; ++i)
//         {
//             node_size_type* node = Node_Size_Alloc::allocate();
//             construct(node, 0);
//             buckets.push_back(reinterpret_cast<link_type>(node));
//         }
//     }

    hashtable(const self& x)
    {
        copyFrom(x);
    }

    ~hashtable()
    {
        clear();
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
        for (typename __container_traits<buckets_type>::size_type i = 0, m = buckets.size(); i < m; ++i)
        {
            link_type current = buckets[i];
            while (current)
            {
                link_type next = current->next;
                destruct(&current->data, has_destruct(current->data));
                destruct(&current, has_destruct(*current));
                Alloc::deallocate(current);
                current = next;
            }
            buckets[i] = NULL;
            buckets_length[i] = 0;
        }
        length = 0;
    }

    void resize()
    {
        const typename __container_traits<buckets_type>::size_type old_size = buckets.size();
        const typename __container_traits<buckets_type>::size_type new_size = old_size == 0 ? 2 : old_size * 2;
        buckets_type tmp(new_size);
        buckets_length_type tmpL(new_size);

        for (typename __container_traits<buckets_type>::size_type i = 0, m = buckets.size(); i < m; ++i)
        {
            link_type current = buckets[i];
            while (current)
            {
                link_type next = current->next;
                HASH_KEY_TYPE idx = index(current->data, new_size);
                current->next = tmp[idx];
                current->prev = NULL;
                if (tmp[idx]) tmp[idx]->prev = current;
                tmp[idx] = current;
                current = next;
                ++tmpL[idx];
            }
        }
        swap(buckets, tmp);
        swap(buckets_length, tmpL);
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

    const value_type& value(const key_type& k)const
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = buckets[idx];
        while (current)
        {
            if (compare(k, key(current->data)))
            {
                return current->data;
            }
            current = current->next;
        }
        throw error<string>("value not exists", __FILE__, __LINE__);
    }

    void erase(iterator position)
    {
        link_type p = position.node;
        if (p)
        {
            HASH_KEY_TYPE idx = index(key(p->data));
            if (p->prev) p->prev->next = p->next;
            else buckets[idx] = p->next;

            if (p->next) p->next->prev = p->prev;
            destruct(&p->data, has_destruct(p->data));
            destruct(p, has_destruct(*p));
            Alloc::deallocate(p);
            --buckets_length[idx];
            --length;
        }
    }

    void erase(const key_type& k)
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = buckets[idx];
        while (current)
        {
            if (compare(k, key(current->data)))
            {
                if (current->prev) current->prev->next = current->next;
                else buckets[idx] = current->next;

                if (current->next) current->next->prev = current->prev;
                destruct(&current->data, has_destruct(current->data));
                destruct(current, has_destruct(*current));
                Alloc::deallocate(current);
                --buckets_length[idx];
                --length;
                break;
            }
        }
    }

    void erase(iterator first, iterator last)
    {
        if (first == begin() && last == end())
        {
            clear();
            return;
        }
        while (first != last) erase(first++);
    }

    iterator find(const key_type& k)
    {
        HASH_KEY_TYPE idx = index(k);
        link_type current = buckets[idx];
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
        link_type current = buckets[idx];
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
        link_type current = buckets[idx];
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
        link_type current = buckets[idx];
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
        link_type current = buckets[idx];
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
            if (buckets[i])
            {
                return iterator(buckets[i], *this);
            }
        }
        return end();
    }

    const_iterator begin()const
    {
        for (typename __container_traits<buckets_type>::size_type i = 0; i < buckets.size(); ++i)
        {
            if (buckets[i])
            {
                return const_iterator(buckets[i], (self&)*this);
            }
        }
        return end();
    }

//     reverse_iterator rbegin()
//     {
//         for (typename __container_traits<buckets_type>::size_type i = buckets.size() - 1; i >= 0; --i)
//         {
//             if (reinterpret_cast<node_size_type*>(buckets[i])->data > 0)
//             {
//                 link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[i])->next);
//                 while (current->next) current = current->next;
//                 return reverse_iterator(iterator(current, *this));
//             }
//         }
//         return rend();
//     }
// 
//     const_reverse_iterator rbegin()const
//     {
//         for (typename __container_traits<buckets_type>::size_type i = buckets.size() - 1; i >= 0; --i)
//         {
//             if (reinterpret_cast<node_size_type*>(buckets[i])->data > 0)
//             {
//                 link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(buckets[i])->next);
//                 while (current->next) current = current->next;
//                 return const_reverse_iterator(const_iterator(current, *this));
//             }
//         }
//         return rend();
//     }

    inline iterator end()
    {
        return iterator(0, *this);
    }

    inline const_iterator end()const
    {
        return const_iterator(0, (self&)*this);
    }

    inline const_iterator maximum()const
    {
        link_type j = NULL;
        for (typename buckets_type::const_iterator i = buckets.begin(), m = buckets.end(); i != m; ++i)
        {
            link_type current = *i;
            while (current)
            {
                if (j == NULL) j = current;
                else if (greater(current->data, j->data)) j = current;
                current = current->next;
            }
        }
        return const_iterator(j, const_cast<self&>(*this));
    }

    inline const_iterator minimum()const
    {
        link_type j = NULL;
        for (typename buckets_type::const_iterator i = buckets.begin(), m = buckets.end(); i != m; ++i)
        {
            link_type current = *i;
            while (current)
            {
                if (j == NULL) j = current;
                else if (less(current->data, j->data)) j = current;
                current = current->next;
            }
        }
        return const_iterator(j, const_cast<self&>(*this));
    }

//     inline reverse_iterator rend()
//     {
//         return reverse_iterator(end());
//     }
// 
//     inline const_reverse_iterator rend()const
//     {
//         return const_reverse_iterator(end());
//     }

    self& operator=(const self& x)
    {
        if (&x != this)
        {
            copyFrom(x);
        }
        return *this;
    }
protected:
    inline const HASH_KEY_TYPE index(const key_type& k)const
    {
        return hash(k) % buckets.size();
    }

    inline HASH_KEY_TYPE index(const value_type& x, typename __container_traits<buckets_type>::size_type sz)
    {
        return hash(key(x)) % sz;
    }

    inline bool willResize(HASH_KEY_TYPE bucket_index)
    {
        return buckets_length[bucket_index] + 1 > Max_Bucket_Length;
    }

    iterator insert_equal_noresize(const value_type& x)
    {
        //node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(x, buckets.size())]);
        HASH_KEY_TYPE idx = index(x, buckets.size());
        link_type node = Alloc::allocate();
        construct(node, x);
        link_type current = buckets[idx];
        key_type k = key(x);
        while (current)
        {
            if (compare(k, key(current->data)))
            {
                node->next = current->next;
                node->prev = current;
                if (current->next) current->next->prev = node;
                current->next = node;
                ++buckets_length[idx];
                ++length;
                return iterator(node, *this);
            }
            current = current->next;
        }
        node->next = buckets[idx];
        if (buckets[idx]) buckets[idx]->prev = node;
        buckets[idx] = node;
        ++buckets_length[idx];
        ++length;
        return iterator(node, *this);
    }

    pair<iterator, bool> insert_unique_noresize(const value_type& x)
    {
        //node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(x, buckets.size())]);
        HASH_KEY_TYPE idx = index(x, buckets.size());
        link_type current = buckets[idx];
        while (current)
        {
            if (compare(key(x), key(current->data)))
            {
                return pair<iterator, bool>(iterator(current, *this), false);
            }
            current = current->next;
        }
        link_type node = Alloc::allocate();
        construct(node, x);
        node->next = buckets[idx];
        if (buckets[idx]) buckets[idx]->prev = node;
        buckets[idx] = node;
        ++buckets_length[idx];
        ++length;
        return pair<iterator, bool>(iterator(node, *this), true);
    }

    void copyFrom(const self& x)
    {
        clear();

        buckets.clear();
        buckets_length.clear();

        buckets.reserve(x.buckets.size());
        buckets_length.reserve(x.buckets_length.size());

        for (typename buckets_type::size_type i = 0, m = x.buckets.size(); i < m; ++i)
        {
            buckets.push_back(NULL);
            buckets_length.push_back(0);
        }

        for (typename buckets_type::size_type i = 0, m = buckets.size(); i < m; ++i)
        {
            link_type pNode = x.buckets[i];
            while (pNode)
            {
                link_type pNewNode = Alloc::allocate();
                construct(pNewNode, pNode->data);

                pNewNode->next = buckets[i];
                if (buckets[i]) buckets[i]->prev = pNewNode;
                buckets[i] = pNewNode;

                pNode = pNode->next;
            }

        }
        length         = x.length;
        key            = x.key;
        hash           = x.hash;
        less           = x.less;
        greater        = x.greater;
        compare        = x.compare;
        buckets_length = x.buckets_length;
    }
};

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
