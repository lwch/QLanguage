#ifndef _QLANGUAGE_LIBRARY_VECTOR_H_
#define _QLANGUAGE_LIBRARY_VECTOR_H_

#include "iterator.h"
#include "allocator.h"
#include "construct.h"
#include "error.h"

namespace QLanguage
{
    namespace Library
    {
        template <typename T, typename Distance>
        class __vector_iterator : public iterator<T>
        {
        protected:
            template <typename C, typename D>
            friend class __vector_const_iterator;

            typedef iterator<T>                    parent;
            typedef __vector_iterator<T, Distance> self;
            typedef T*                             link_type;

            link_type node;
        public:
            __vector_iterator() : node(NULL)
            {
            }

            __vector_iterator(link_type x) : node(x)
            {
            }

            static const __vector_iterator<T, Distance> null()
            {
                static __vector_iterator<T, Distance> tmp;
                return tmp;
            }

            inline typename parent::reference operator*()
            {
                return *node;
            }

            inline self& operator++()
            {
                ++node;
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
                --node;
                return *this;
            }

            inline self operator--(int)
            {
                self tmp = *this;
                --*this;
                return tmp;
            }

            inline self operator+(Distance n)const
            {
                return self(node + n);
            }

            inline self& operator+=(Distance n)
            {
                node += n;
                return *this;
            }

            inline self operator-(Distance n)const
            {
                return self(node - n);
            }

            inline self& operator-=(Distance n)
            {
                node -= n;
                return *this;
            }

            inline const bool operator==(const self& x)const
            {
                return node == x.node;
            }

            inline const bool operator!=(const self& x)const
            {
                return node != x.node;
            }

            inline T& operator[](Distance n)
            {
                return node[n];
            }

            inline operator link_type()const
            {
                return node;
            }
        };

        template <typename T, typename Distance>
        class __vector_const_iterator : public const_iterator<T>
        {
        protected:
            typedef const_iterator<T>                    parent;
            typedef __vector_const_iterator<T, Distance> self;
            typedef const T*                             link_type;

            link_type node;
        public:
            __vector_const_iterator() : node(NULL)
            {
            }

            __vector_const_iterator(link_type x) : node(x)
            {
            }

            __vector_const_iterator(const __vector_iterator<T, Distance>& x) : node(x.node)
            {
            }

            inline typename parent::reference operator*()
            {
                return *node;
            }

            inline self& operator++()
            {
                ++node;
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
                --node;
                return *this;
            }

            inline self operator--(int)
            {
                self tmp = *this;
                --*this;
                return tmp;
            }

            inline self operator+(Distance n)const
            {
                return self(node + n);
            }

            inline self& operator+=(Distance n)
            {
                node += n;
                return *this;
            }

            inline self operator-(Distance n)const
            {
                return self(node - n);
            }

            inline self& operator-=(Distance n)
            {
                node -= n;
                return *this;
            }

            inline const bool operator==(const self& x)const
            {
                return node == x.node;
            }

            inline const bool operator!=(const self& x)const
            {
                return node != x.node;
            }

            inline T& operator[](Distance n)
            {
                return node[n];
            }

            inline operator link_type()const
            {
                return node;
            }
        };

        template <typename T>
        class vector
        {
        public:
            typedef T         value_type;
            typedef T*        pointer;
            typedef T&        reference;
            typedef const T&  const_reference;
            typedef size_t    size_type;
            typedef ptrdiff_t difference_type;
            typedef __vector_const_iterator<value_type, difference_type> const_iterator;
            typedef reverse_iterator<const_iterator, value_type, size_type, difference_type> const_reverse_iterator;
            typedef __vector_iterator<value_type, difference_type> iterator;
            typedef reverse_iterator<iterator, value_type, size_type, difference_type> reverse_iterator;
        protected:
            typedef vector<T>    self;
            typedef allocator<T> Alloc;

            iterator start;
            iterator finish;
            iterator end_of_element;
        public:
            vector()
            {
                start = finish = end_of_element = NULL;
            }

            vector(const size_type& count, const T& x)
            {
                if(count <= 0) throw error<const char*>("out of range");
                start = Alloc::allocate(count);
                uninitialized_fill_n(start, count, x);
                finish = start + count;
                end_of_element = start + count;
            }

            vector(const size_type& count)
            {
                if(count <= 0) throw error<const char*>("out of range");
                start = Alloc::allocate(count);
                uninitialized_fill_n(start, count, T());
                finish = start + count;
                end_of_element = start + count;
            }

            vector(const self& x)
            {
                size_type const other_size = x.size();
                start = Alloc::allocate(other_size);
                finish = uninitialized_copy(x.begin(), x.end(), start);
                end_of_element = start + other_size;
            }

            vector(const_iterator first, const_iterator last)
            {
                size_type const other_size = last - first;
                start = Alloc::allocate(other_size);
                finish = uninitialized_copy(first, last, start);
                end_of_element = start + other_size;
            }

            ~vector()
            {
                destruct(start, end_of_element);
                if (start != iterator::null()) Alloc::deallocate(start, end_of_element - start);
            }

            void reserve(const size_type& count)
            {
                size_type const old_size = size();
                if(count <= capacity())
                {
                    // Do nothing
                }
                else
                {
                    iterator tmp = Alloc::allocate(count);
                    copy(begin(), end(), tmp);
                    Alloc::deallocate(start, old_size);
                    start = tmp;
                    finish = tmp + old_size;
                    end_of_element = start + count;
                }
            }

            inline iterator begin()
            {
                return start;
            }

            inline const_iterator begin()const
            {
                return start;
            }

            inline reverse_iterator rbegin()
            {
                return reverse_iterator(end());
            }

            inline const_reverse_iterator rbegin()const
            {
                return const_reverse_iterator(end());
            }

            inline iterator end()
            {
                return finish;
            }

            inline const_iterator end()const
            {
                return finish;
            }

            inline reverse_iterator rend()
            {
                return reverse_iterator(begin());
            }

            inline const_reverse_iterator rend()const
            {
                return const_reverse_iterator(begin());
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
                return *(end() - 1);
            }

            inline const_reference back()const
            {
                return *(end() - 1);
            }

            inline bool empty()const
            {
                return begin() == end();
            }

            inline const size_type size()const
            {
                return size_type(end() - begin());
            }

            inline const size_type capacity()const
            {
                return size_type(end_of_element - begin());
            }

            void push_back(const T& x)
            {
                if (end_of_element != finish)
                {
                    construct(&*finish, x);
                    ++finish;
                }
                else
                {
                    insert_aux(end(), x);
                }
            }

            inline void pop_back()
            {
                --finish;
                destruct<T>(finish, has_destruct(*finish));
            }

            inline iterator insert(iterator position, const T& x)
            {
                const size_type pos = position - begin();
                if(finish != end_of_element && position == end())
                {
                    construct(&*finish, x);
                    ++finish;
                }
                else insert_aux(position, x);
                return begin() + pos;
            }

            void erase(iterator position)
            {
                destruct(&*position, has_destruct(*position));
                if (position + 1 != end())
                {
                    copy(position + 1, end(), position);
                }
                --finish;
            }

            self& operator=(const self& x)
            {
                if(&x == this) return *this;
                size_type const other_size = x.size();
                if(other_size > capacity())
                {
                    destruct(start, finish);
                    Alloc::deallocate(start, capacity());
                    start = Alloc::allocate(other_size);
                    finish = uninitialized_copy(x.begin(), x.end(), start);
                    end_of_element = start + other_size;
                }
                else
                {
                    finish = uninitialized_copy(x.begin(), x.end(), start);
                }
                return *this;
            }

            inline const_reference operator[](size_type n)const
            {
                return *(begin() + n);
            }

            inline reference operator[](size_type n)
            {
                return *(begin() + n);
            }

            inline const value_type at(size_type n)const
            {
                return operator[](n);
            }
        protected:
            void insert_aux(const iterator position, const T& x)
            {
                if(finish != end_of_element)
                {
                    construct(&*finish, *(finish - 1));
                    T x_copy = x;
                    copy_backward(position, finish - 1, finish);
                    *position = x_copy;
                    ++finish;
                }
                else
                {
                    const size_type old_size = size();
                    const size_type new_size = old_size == 0 ? 2 : old_size * 2;
                    iterator tmp = Alloc::allocate(new_size);
                    uninitialized_copy(begin(), position, tmp);
                    iterator new_position = tmp + (position - begin());
                    construct(&*new_position, x);
                    uninitialized_copy(position, end(), new_position + 1);
                    destruct(begin(), end());
                    Alloc::deallocate(begin(), old_size);
                    end_of_element = tmp + new_size;
                    finish = tmp + old_size + 1;
                    start = tmp;
                }
            }
        };
    }
}

#endif
