#ifndef _QLANGUAGE_LIBRARY_MAP_H_
#define _QLANGUAGE_LIBRARY_MAP_H_

#include "rbtree.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename Key, typename T, typename LessCompare = less<T>, typename EqualCompare = equal_to<T> >
        class map
        {
        protected:
            typedef Key                key_type;
            typedef T                  data_type;
            typedef pair<const Key, T> value_type;
            typedef rbtree<key_type, value_type, select1st<value_type>, LessCompare, EqualCompare> instance_type;

            instance_type instance;
        public:
            typedef typename instance_type::size_type       size_type;
            typedef typename instance_type::difference_type difference_type;
            typedef typename instance_type::iterator        iterator;
            typedef typename instance_type::const_iterator  const_iterator;

            inline pair<iterator, bool> insert(const value_type& x)
            {
                return instance.insert_unique(x);
            }

            inline iterator erase(const key_type& x)
            {
                return instance.erase(x);
            }

            inline iterator find(const key_type& x)
            {
                return instance.find(x);
            }

            inline void clear()
            {
                instance.clear();
            }

            inline iterator maximum()const
            {
                return instance.maximum();
            }

            inline iterator minimum()const
            {
                return instance.minimum();
            }

            inline iterator begin()
            {
                return instance.begin();
            }

            inline const_iterator begin()const
            {
                return instance.begin();
            }

            inline iterator end()
            {
                return instance.end();
            }

            inline const_iterator end()const
            {
                return instance.end();
            }

            inline data_type& operator[](const key_type& key)
            {
                return (*instance.insert_unique(value_type(key, data_type())).first).second;
            }
        };
    }
}

#endif
