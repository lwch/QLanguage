/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:36
	filename: 	\QCore\Library\set.h
	file path:	\QCore\Library
	file base:	set
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

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
        public:
            typedef T        key_type;
            typedef T        value_type;
            typedef T&       reference;
            typedef T*       pointer;
            typedef const T& const_reference;
        protected:
            typedef rbtree<key_type, value_type, identity<value_type>, LessCompare, EqualCompare> instance_type;
            typedef set<T, LessCompare, EqualCompare> self;

            instance_type container;
        public:
            typedef typename instance_type::size_type              size_type;
            typedef typename instance_type::distance_type          distance_type;
            typedef typename instance_type::iterator               iterator;
            typedef typename instance_type::const_iterator         const_iterator;
            typedef typename instance_type::reverse_iterator       reverse_iterator;
            typedef typename instance_type::const_reverse_iterator const_reverse_iterator;

            set() {}

            set(const self& x) : container(x.container) {}

            ~set() {}

            inline pair<iterator, bool> insert(const value_type& x)
            {
                return container.insert_unique(x);
            }

            void insert(const self& x)
            {
                for (const_iterator i = x.begin(), m = x.end(); i != m; ++i)
                {
                    container.insert_unique(*i);
                }
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

            inline const bool operator==(const self& x)const
            {
                if (&x == this) return true;

                return container == x.instance;
            }
        };
    }
}

#endif
