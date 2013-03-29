/********************************************************************
	created:	2013/02/14
	created:	14:2:2013   15:49
	filename: 	\QCore\Library\hashmap.h
	file path:	\QCore\Library
	file base:	hashmap
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_HASHMAP_H_
#define _QLANGUAGE_LIBRARY_HASHMAP_H_

#include "definition.h"
#include "hashtable.h"

NAMESPACE_QLANGUAGE_LIBRARY_START

template <typename Key,
          typename T,
          typename Hash = hash<Key>,
          size_t Max_Bucket_Length = 11,
          bool Resize = true,
          typename Compare = equal_to<Key> >
class hashmap
{
protected:
    typedef Key          key_type;
    typedef T            data_type;
    typedef pair<Key, T> value_type;
    typedef hashtable<key_type, value_type, select1st<value_type>, Max_Bucket_Length, Resize, Hash, Compare> instance_type;
    typedef hashmap<Key, T, Hash, Max_Bucket_Length, Resize, Compare> self;

    instance_type instance;
public:
    typedef typename instance_type::size_type              size_type;
    typedef typename instance_type::distance_type          distance_type;
    typedef typename instance_type::iterator               iterator;
    typedef typename instance_type::const_iterator         const_iterator;
    typedef typename instance_type::reverse_iterator       reverse_iterator;
    typedef typename instance_type::const_reverse_iterator const_reverse_iterator;

    hashmap() {}

    hashmap(const self& x) : instance(x.instance) {}

    ~hashmap() {}

    inline pair<iterator, bool> insert(const value_type& x)
    {
        return instance.insert_unique(x);
    }

    inline void erase(iterator position)
    {
        instance.erase(position);
    }

    inline void erase(const key_type& x)
    {
        instance.erase(x);
    }

    inline void erase(iterator first, iterator last)
    {
        instance.erase(first, last);
    }

    inline iterator find(const key_type& x)
    {
        return instance.find(x);
    }

    inline void clear()
    {
        instance.clear();
    }

    inline size_type size()const
    {
        return instance.size();
    }

    inline bool empty()const
    {
        return instance.empty();
    }

    inline iterator begin()
    {
        return instance.begin();
    }

    inline const_iterator begin()const
    {
        return instance.begin();
    }

    inline reverse_iterator rbegin()
    {
        return instance.rbegin();
    }

    inline const_reverse_iterator rbegin()const
    {
        return instance.rbegin();
    }

    inline iterator end()
    {
        return instance.end();
    }

    inline const_iterator end()const
    {
        return instance.end();
    }

    inline reverse_iterator rend()
    {
        return instance.rend();
    }

    inline const_reverse_iterator rend()const
    {
        return instance.rend();
    }

    inline data_type& operator[](const key_type& key)
    {
        return (*instance.insert_unique(value_type(key, data_type())).first).second;
    }

    inline const data_type& operator[](const key_type& key)const
    {
        return instance.value(key).second;
    }

    inline self& operator=(const self& x)
    {
        if (&x != this) instance = x.instance;
        return *this;
    }
};

NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
