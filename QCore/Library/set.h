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
            typedef rbtree<key_type, value_type, identity<value_type>, LessCompare, EqualCompare> instance_type;

            instance_type instance;
        public:
            typedef typename instance_type::size_type              size_type;
            typedef typename instance_type::difference_type        difference_type;
            typedef typename instance_type::iterator               iterator;
            typedef typename instance_type::const_iterator         const_iterator;
            typedef typename instance_type::reverse_iterator       reverse_iterator;
            typedef typename instance_type::const_reverse_iterator const_reverse_iterator;

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

            inline const_iterator maximum()const
            {
                return instance.maximum();
            }

            inline const_iterator minimum()const
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
        };
    }
}

#endif
