#ifndef _QLANGUAGE_LIBRARY_SET_H_
#define _QLANGUAGE_LIBRARY_SET_H_

#include "rbtree.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T, typename LessCompare = less<T>, typename EqualCompare = equal_to<T> >
        class set
        {
        protected:
            typedef T key_type;
            typedef T value_type;
            typedef rbtree<key_type, value_type, identity<value_type> > instance_type;

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
        };
    }
}

#endif
