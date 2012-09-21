#ifndef _QLANGUAGE_LIBRARY_LIST_H_
#define _QLANGUAGE_LIBRARY_LIST_H_

#include "iterator.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        struct __list_iterator;

        template <typename T>
        struct __list_const_iterator;

        template <typename T>
        struct __list_node
        {
            __list_node<T>* prev;
            __list_node<T>* next;
            T data;

            __list_node() : prev(NULL), next(NULL)
            {
            }

            __list_node(const T& x) : prev(NULL), next(NULL), data(x)
            {
            }
        };

        template <typename T>
        struct __list_iterator
        {
            typedef __list_iterator<T> iterator;
            typedef T                  value_type;
            typedef ptrdiff_t          difference_type;
            typedef T*                 pointer;
            typedef T&                 reference;
            typedef const T*           const_pointer;
            typedef const T&           const_reference;
            typedef __list_node<T>*    link_type;
            typedef void*              void_pointer;

            link_type node;

            __list_iterator(link_type x) : node(x)
            {
            }

            __list_iterator(const __list_const_iterator<T>& x) : node(x.node)
            {
            }

            __list_iterator() : node(NULL)
            {
            }

            inline iterator& operator++()
            {
                node = ((link_type)node)->next;
                return *this;
            }

            inline iterator operator++(int)
            {
                iterator tmp = *this;
                ++*this;
                return tmp;
            }

            inline iterator& operator--()
            {
                node = ((link_type)node)->prev;
                return *this;
            }

            inline iterator operator--(int)
            {
                iterator tmp = *this;
                --*this;
                return tmp;
            }

            inline reference operator*()const
            {
                return node->data;
            }

            inline bool operator==(const iterator& x)const
            {
                return node == x.node;
            }

            inline bool operator!=(const iterator& x)const
            {
                return node != x.node;
            }
        };

        template <typename T>
        struct __list_const_iterator
        {
            typedef __list_iterator<T>       iterator;
            typedef __list_const_iterator<T> const_iterator;
            typedef T                        value_type;
            typedef ptrdiff_t                difference_type;
            typedef T*                       pointer;
            typedef T&                       reference;
            typedef const T*                 const_pointer;
            typedef const T&                 const_reference;
            typedef __list_node<T>*          link_type;

            link_type node;

            __list_const_iterator(link_type x) : node(x)
            {
            }

            __list_const_iterator() : node(NULL)
            {
            }

            __list_const_iterator(const iterator& x) : node(x.node)
            {
            }

            inline const_iterator& operator++()
            {
                node = ((link_type)node)->next;
                return *this;
            }

            inline const_iterator operator++(int)
            {
                const_iterator tmp = *this;
                ++*this;
                return tmp;
            }

            inline const_iterator& operator--()
            {
                node = ((link_type)node)->prev;
                return *this;
            }

            inline const_iterator operator--(int)
            {
                const_iterator tmp = *this;
                --*this;
                return tmp;
            }

            inline const_reference operator*()const
            {
                return node->data;
            }

            inline bool operator==(const const_iterator& x)const
            {
                return node == x.node;
            }

            inline bool operator!=(const const_iterator& x)const
            {
                return node != x.node;
            }
        };

        template <typename T>
        class list
        {
        public:
            typedef T                        value_type;
            typedef T*                       pointer;
            typedef __list_iterator<T>       iterator;
            typedef __list_const_iterator<T> const_iterator;
            typedef T&                       reference;
            typedef const T&                 const_reference;
            typedef size_t                   size_type;
            typedef ptrdiff_t                difference_type;
            typedef reverse_iterator<const_iterator, value_type, size_type, difference_type> const_reverse_iterator;
            typedef reverse_iterator<iterator, value_type, size_type, difference_type> reverse_iterator;
        protected:
            typedef __list_node<T>*           link_type;
            typedef list<T>                   self;
            typedef allocator<__list_node<T> > Node_Alloc;

            link_type node;
            size_type length;
        public:
            list() : length(0)
            {
                node = Node_Alloc::allocate();
                node->next = node;
                node->prev = node;
            }

            list(size_type count) : length(0)
            {
                node = Node_Alloc::allocate();
                node->next = node;
                node->prev = node;
                insert(begin(), count, T());
            }

            list(size_type count, const T& x) : length(0)
            {
                node = Node_Alloc::allocate();
                node->next = node;
                node->prev = node;
                insert(begin(), count, x);
            }

            list(const T* first, const T* last) : length(0)
            {
                node = Node_Alloc::allocate();
                node->next = node;
                node->prev = node;
                insert(begin(), first, last);
            }

            list(const iterator& first, const iterator& last) : length(0)
            {
                node = Node_Alloc::allocate();
                node->next = node;
                node->prev = node;
                insert(begin(), first, last);
            }

            list(const self& x) : length(0)
            {
                node = Node_Alloc::allocate();
                node->next = node;
                node->prev = node;
                insert(begin(), x.begin(), x.end());
            }

            ~list()
            {
                clear();
                Node_Alloc::deallocate(node);
            }

            inline iterator begin()
            {
                return node->next;
            }

            inline const_iterator begin()const
            {
                return node->next;
            }

            inline reverse_iterator rbegin()
            {
                return end();
            }

            inline const_reverse_iterator rbegin()const
            {
                return end();
            }

            inline iterator end()
            {
                return node;
            }

            inline const_iterator end()const
            {
                return node;
            }

            inline reverse_iterator rend()
            {
                return begin();
            }

            inline const_reverse_iterator rend()const
            {
                return begin();
            }

            inline bool empty()const
            {
                return length == 0;
            }

            inline size_type size()const
            {
                return length;
            }

            inline reference front()
            {
                return *begin();
            }

            inline const_reference front()const
            {
                return *begin();
            }

            inline reference back()
            {
                return *end();
            }

            inline const_reference back()const
            {
                return *end();
            }

            void clear()
            {
                link_type current = node->next;
                while(current != node)
                {
                    link_type tmp = current;
                    current = current->next;
                    destruct(&tmp->data, has_destruct(tmp->data));
                    Node_Alloc::deallocate(tmp);
                }
                node->next = node;
                node->prev = node;
                length = 0;
            }

            iterator insert(const iterator& position, const T& x)
            {
                if(!inRange(position)) throw "out of range";
                link_type tmp = Node_Alloc::allocate();
                construct(tmp, x);
                tmp->prev = position.node->prev;
                tmp->next = position.node;
                position.node->prev->next = tmp;
                position.node->prev = tmp;
                ++length;
                return tmp;
            }

            void insert(const iterator& position, size_type count, const T& x)
            {
                while(count--) insert(position, x);
            }

            void insert(const iterator& position, const T* first, const T* last)
            {
                while(first != last) insert(position, *first++);
            }

            void insert(const iterator& position, const iterator& first, const iterator& last)
            {
                iterator i = first;
                while(i != last) insert(position, *i++);
            }

            inline void push_front(const T& x)
            {
                insert(begin(), x);
            }

            inline void push_back(const T& x)
            {
                insert(end(), x);
            }

            void erase(const iterator& position)
            {
                if(!inRange(position)) throw "out of range";
                position.node->prev->next = position.node->next;
                position.node->next->prev = position.node->prev;
                destruct(&position.node->data, has_destruct(position.node->data));
                Node_Alloc::deallocate(position.node);
                --length;
            }

            void erase(const iterator& first, const iterator& last)
            {
                iterator i = first;
                while(i != last) erase(i++);
            }

            inline void pop_front()
            {
                erase(begin());
            }

            inline void pop_back()
            {
                erase(--end());
            }

            self& operator=(const self& x)
            {
                if(this == &x) return *this;

                iterator first1 = begin();
                iterator last1 = end();
                const_iterator first2 = x.begin();
                const_iterator last2 = x.end();
                while (first1 != last1 && first2 != last2) *first1++ = *first2++;
                if (first2 == last2) erase(first1, last1);
                else insert(last1, first2, last2);
                return *this;
            }

            reference operator[](size_type n)
            {
                if(n < 0 || n >= length) throw "out of range";
                link_type current = NULL;
                if(n < length / 2)
                {
                    current = node->next;
                    for(size_type i = 0; i < n; i++, current = current->next);
                }
                else
                {
                    n = length - n - 1;
                    current = node->prev;
                    for(size_type i = 0; i < n; i++, current = current->prev);
                }
                return current->data;
            }

            inline value_type at(size_type n)
            {
                return operator[](n);
            }
        protected:
            bool inRange(const iterator& position)
            {
                for(iterator i = begin(); i != end(); ++i)
                {
                    if(*i == *position) return true;
                }
                if(position == end()) return true;
                return false;
            }
        };
    }
}

#endif
