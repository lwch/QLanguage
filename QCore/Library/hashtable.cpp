/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:33
	filename: 	\QCore\Library\hashtable.cpp
	file path:	\QCore\Library
	file base:	hashtable
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_HASHTABLE_CPP_
#define _QLANGUAGE_LIBRARY_HASHTABLE_CPP_

#include "hashtable.h"
#include "traits.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

#define HASHTABLE_ACHIEVE_HEADER(...) \
    template <typename Key, typename Value, typename KeyOfValue, \
    size_t Max_Bucket_Length, \
    bool Resize, \
    typename Hash, \
    typename Compare> \
    __VA_ARGS__ hashtable<HASHTABLE_TEMPLATE_ACHIEVE>

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length,
    bool Resize,
    typename Hash,
    typename Compare>
    void __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>::inc()
{
    node_type* prev = node;
    node = node->next;
    if (node == NULL)
    {
        HASH_KEY_TYPE bucket = hashtable.index(node->data);
        typename __container_traits<typename hashtable_type::buckets_type>::size_type size = hashtable.buckets.size();
        while(++bucket < size && node = reinterpret_cast<typename hashtable_type::node_size_type*>(hashtable[bucket])->next && node == hashtable[bucket]);
    }
}

template <typename Key, typename Value, typename KeyOfValue,
    size_t Max_Bucket_Length,
    bool Resize,
    typename Hash,
    typename Compare>
    void __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>::dec()
{
    HASH_KEY_TYPE idx = hashtable.index(hashtable.key(node->data));
    node_type* prev = node;
    node = node->prev;
    if (node == hashtable.buckets[idx])
    {
        while (--idx >= 0 && reinterpret_cast<typename hashtable_type::node_size_type*>(hashtable.buckets[idx])->data > 0)
        {
            typedef typename hashtable_type::link_type link_type;
            typedef typename hashtable_type::node_size_type node_size_type;
            link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(hashtable.buckets[idx])->next);
            while (current) current = current->next;
            node = current;
            return;
        }
        node = NULL;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
HASHTABLE_ACHIEVE_HEADER()::hashtable()
    : length(0)
{
    const size_type defaultSize = 11;
    buckets.reserve(defaultSize);
    for(typename __container_traits<buckets_type>::size_type i = 0; i < defaultSize; ++i)
    {
        node_size_type* node = Node_Size_Alloc::allocate();
        construct(node, 0);
        buckets.push_back(reinterpret_cast<link_type>(node));
    }
}

HASHTABLE_ACHIEVE_HEADER()::hashtable(size_type size)
    : length(0)
{
    buckets.reserve(size);
    for(typename __container_traits<buckets_type>::size_type i = 0; i < size; ++i)
    {
        node_size_type* node = Node_Size_Alloc::allocate();
        construct(node, 0);
        buckets.push_back(reinterpret_cast<link_type>(node));
    }
}

HASHTABLE_ACHIEVE_HEADER()::~hashtable()
{
    clear();
    for(typename __container_traits<buckets_type>::size_type i = 0; i < buckets.size(); ++i)
    {
        Node_Size_Alloc::deallocate(reinterpret_cast<node_size_type*>(buckets[i]));
    }
}

HASHTABLE_ACHIEVE_HEADER(void)::clear()
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

HASHTABLE_ACHIEVE_HEADER(inline typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator)::insert_equal(const value_type& x)
{
    if (willResize(index(x, buckets.size()))) resize();
    return insert_equal_noresize(x);
}

HASHTABLE_ACHIEVE_HEADER(inline void)::insert_equal(const value_type* first, const value_type* last)
{
    while (first != last)
    {
        insert_equal(*first);
        ++first;
    }
}

HASHTABLE_ACHIEVE_HEADER(inline void)::insert_equal(const_iterator first, const_iterator last)
{
    while (first != last)
    {
        insert_equal(*first);
        ++first;
    }
}

HASHTABLE_ACHIEVE_HEADER(inline pair<typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator, bool>)::insert_unique(const value_type& x)
{
    if (willResize(index(x, buckets.size()))) resize();
    return insert_unique_noresize(x);
}

HASHTABLE_ACHIEVE_HEADER(inline void)::insert_unique(const value_type* first, const value_type* last)
{
    while (first != last)
    {
        insert_unique(*first);
        ++first;
    }
}

HASHTABLE_ACHIEVE_HEADER(inline void)::insert_unique(const_iterator first, const_iterator last)
{
    while (first != last)
    {
        insert_unique(*first);
        ++first;
    }
}

HASHTABLE_ACHIEVE_HEADER(void)::erase(iterator position)
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator)::find(const key_type& k)
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_iterator)::find(const key_type& k)const
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::size_type)::count(const key_type& k)const
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

HASHTABLE_ACHIEVE_HEADER(pair<typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator, typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator>)::equal_range(const key_type& k)
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

HASHTABLE_ACHIEVE_HEADER(pair<typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_iterator, typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_iterator>)::equal_range(const key_type& k)const
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

HASHTABLE_ACHIEVE_HEADER(void)::resize()
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

HASHTABLE_ACHIEVE_HEADER(inline bool)::willResize(HASH_KEY_TYPE bucket_index)
{
    node_size_type* head = reinterpret_cast<node_size_type*>(buckets[bucket_index]);
    return head->data + 1 > Max_Bucket_Length;
}

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator)::insert_equal_noresize(const value_type& x)
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

HASHTABLE_ACHIEVE_HEADER(pair<typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator, bool>)::insert_unique_noresize(const value_type& x)
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator)::begin()
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_iterator)::begin()const
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::reverse_iterator)::rbegin()
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

HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_reverse_iterator)::rbegin()const
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

HASHTABLE_ACHIEVE_HEADER(inline typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator)::end()
{
    return iterator(0, *this);
}

HASHTABLE_ACHIEVE_HEADER(inline typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_iterator)::end()const
{
    return const_iterator(0, *this);
}

HASHTABLE_ACHIEVE_HEADER(inline typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::reverse_iterator)::rend()
{
    return reverse_iterator(end());
}

HASHTABLE_ACHIEVE_HEADER(inline typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::const_reverse_iterator)::rend()const
{
    return const_reverse_iterator(end());
}

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
