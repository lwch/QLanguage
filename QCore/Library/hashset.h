/********************************************************************
	created:	2013/02/13
	created:	13:2:2013   17:27
	filename: 	\QCore\Library\hashset.h
	file path:	\QCore\Library
	file base:	hashset
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_HASHSET_H_
#define _QLANGUAGE_LIBRARY_HASHSET_H_

#include "hashtable.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T,
                  typename Hash = hash<T>,
                  size_t Max_Bucket_Length = 11,
                  bool Resize = true,
                  typename Less = less<T>,
                  typename Greater = greater<T>,
                  typename Compare = equal_to<T> >
        class hashset
        {
        public:
            typedef T        key_type;
            typedef T        value_type;
            typedef T*       pointer;
            typedef T&       reference;
            typedef const T& const_reference;
        protected:
            typedef hashtable<key_type, value_type, identity<key_type>, identity<value_type>, Max_Bucket_Length, Resize, Hash, Less, Greater, Compare> container_type;
            typedef hashset<T, Hash, Max_Bucket_Length, Resize, Compare> self;

            container_type container;
        public:
            typedef typename container_type::size_type              size_type;
            typedef typename container_type::distance_type          distance_type;
            typedef typename container_type::iterator               iterator;
            typedef typename container_type::const_iterator         const_iterator;
            typedef typename container_type::reverse_iterator       reverse_iterator;
            typedef typename container_type::const_reverse_iterator const_reverse_iterator;

            hashset() {}

            hashset(const self& x) : container(x.container) {}

            ~hashset() {}

            inline pair<iterator, bool> insert(const value_type& x)
            {
                return container.insert_unique(x);
            }

            inline void erase(iterator position)
            {
                container.erase(position);
            }

            inline void erase(const key_type& x)
            {
                container.erase(x);
            }

            inline void erase(iterator first, iterator last)
            {
                container.erase(first, last);
            }

            inline iterator find(const key_type& x)
            {
                return container.find(x);
            }

            inline void clear()
            {
                container.clear();
            }

            inline size_type size()const
            {
                return container.size();
            }

            inline bool empty()const
            {
                return container.empty();
            }

            inline const_iterator maximum()const
            {
                return container.maximum();
            }

            inline const_iterator minimum()const
            {
                return container.minimum();
            }

            inline iterator begin()
            {
                return container.begin();
            }

            inline const_iterator begin()const
            {
                return container.begin();
            }

            inline reverse_iterator rbegin()
            {
                return container.rbegin();
            }

            inline const_reverse_iterator rbegin()const
            {
                return container.rbegin();
            }

            inline iterator end()
            {
                return container.end();
            }

            inline const_iterator end()const
            {
                return container.end();
            }

            inline reverse_iterator rend()
            {
                return container.rend();
            }

            inline const_reverse_iterator rend()const
            {
                return container.rend();
            }

            inline pair<iterator, iterator> equal_range(const key_type& x)
            {
                return container.equal_range(x);
            }

            inline pair<const_iterator, const_iterator> equal_range(const key_type& x)const
            {
                return container.equal_range(x);
            }

            inline self& operator=(const self& x)
            {
                if (&x != this) container = x.container;
                return *this;
            }
        };
    }
}

#endif
