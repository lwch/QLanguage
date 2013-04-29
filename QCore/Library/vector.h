/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:37
	filename: 	\QCore\Library\vector.h
	file path:	\QCore\Library
	file base:	vector
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/

#ifndef _QLANGUAGE_LIBRARY_VECTOR_H_
#define _QLANGUAGE_LIBRARY_VECTOR_H_

#include "definition.h"

#include "iterator.h"
#include "allocator.h"
#include "construct.h"

NAMESPACE_QLANGUAGE_LIBRARY_START
template <typename T, typename Size, typename Distance>
class __vector_iterator : public __iterator<T, Size, Distance>
{
protected:
    template <typename C, typename S, typename D>
    friend class __vector_const_iterator;

    typedef __iterator<T, Size, Distance>        parent;
    typedef __vector_iterator<T, Size, Distance> self;
    typedef T*                                   link_type;

    link_type node;
public:
    __vector_iterator() : node(NULL)
    {
    }

    __vector_iterator(link_type x) : node(x)
    {
    }

    static const self null()
    {
        static self tmp;
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

    inline typename parent::value_type& operator[](Distance n)
    {
        return node[n];
    }

    inline operator link_type()const
    {
        return node;
    }

    inline link_type operator->()const
    {
        return node;
    }
};

template <typename T, typename Size, typename Distance>
class __vector_const_iterator : public __const_iterator<T, Size, Distance>
{
protected:
    typedef __const_iterator<T, Size, Distance>        parent;
    typedef __vector_const_iterator<T, Size, Distance> self;
    typedef const T*                                   link_type;

    link_type node;
public:
    __vector_const_iterator() : node(NULL)
    {
    }

    __vector_const_iterator(link_type x) : node(x)
    {
    }

    __vector_const_iterator(const __vector_iterator<T, Size, Distance>& x) : node(x.node)
    {
    }

    static const self null()
    {
        static self tmp;
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

    inline typename parent::value_type& operator[](Distance n)
    {
        return node[n];
    }

    inline operator link_type()const
    {
        return node;
    }

    inline link_type operator->()const
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
    typedef ptrdiff_t distance_type;
    typedef __vector_const_iterator<value_type, size_type, distance_type> const_iterator;
    typedef __reverse_iterator<const_iterator, value_type, size_type, distance_type> const_reverse_iterator;
    typedef __vector_iterator<value_type, size_type, distance_type> iterator;
    typedef __reverse_iterator<iterator, value_type, size_type, distance_type> reverse_iterator;
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
        initialize(count, x);
    }

    vector(const size_type& count)
    {
        initialize(count);
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
        destruct(start, finish);
        if (start != iterator::null()) Alloc::deallocate(start, end_of_element - start);
    }

    void initialize(size_type count, const value_type& x = value_type())
    {
        destruct(start, finish);
        if (start != iterator::null()) Alloc::deallocate(start, end_of_element - start);
        if(count <= 0) THROW_OUT_OF_RANGE;
        start = Alloc::allocate(count);
        uninitialized_fill_n(start, count, x);
        finish = start + count;
        end_of_element = start + count;
    }

    void reserve(size_type count)
    {
        const size_type old_capacity = capacity();
        if(count <= capacity())
        {
            // Do nothing
        }
        else
        {
            const size_type old_size = size();
            iterator tmp = Alloc::allocate(count);
            copy(begin(), end(), tmp);
            destruct(start, finish);
            Alloc::deallocate(start, old_capacity);
            start = tmp;
            finish = tmp + old_size;
            end_of_element = tmp + count;
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

    void setsize(size_t size)
    {
        if (size <= capacity()) finish = start + size;
        else throw error<const char*>("size is too big", __FILE__, __LINE__);
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

    bool push_back_unique(const T& x)
    {
        for (const_iterator i = begin(), m = end(); i != m; ++i)
        {
            if (*i == x) return false;
        }
        if (end_of_element != finish)
        {
            construct(&*finish, x);
            ++finish;
        }
        else
        {
            insert_aux(end(), x);
        }
        return true;
    }

    template <typename Compare>
    bool push_back_unique(const T& x, Compare comp)
    {
        if (comp(x))
        {
            for (const_iterator i = begin(), m = end(); i != m; ++i)
            {
                if (*i == x) return false;
            }
            if (end_of_element != finish)
            {
                construct(&*finish, x);
                ++finish;
            }
            else
            {
                insert_aux(end(), x);
            }
            return true;
        }
        return false;
    }

    inline void pop_back()
    {
        --finish;
        destruct<T>(finish, has_destruct(*finish));
    }

    inline void pop_front()
    {
        erase(begin());
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

    void erase(iterator first, iterator last)
    {
        iterator i = copy(last, end(), first);
        destruct(i, finish);
        finish = finish - (last - first);
    }

    void unique()
    {
        const_iterator first = begin();
        const_iterator last  = end();
        if (first == last) return;
        while (first != last)
        {
            for (const_iterator i = ++first; i != last;)
            {
                if (*i == *first)
                {
                    const_iterator j = i;
                    ++i;
                    erase(j);
                }
                else ++i;
            }
            ++first;
        }
    }

    const bool operator==(const self& x)const
    {
        if (size() != x.size()) return false;

        for (const_iterator first1 = begin(), first2 = x.begin(), last = end(); first1 != last; ++first1, ++first2)
        {
            if (*first1 != *first2) return false;
        }
        return true;
    }

    const bool operator!=(const self& x)const
    {
        if (size() != x.size()) return true;

        for (const_iterator first1 = begin(), first2 = x.begin(), last = end(); first1 != last; ++first1, ++first2)
        {
            if (*first1 != *first2) return true;
        }
        return false;
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
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        if (n >= size()) throw error<const char*>("operator[] out of range", __FILE__, __LINE__);
#endif
        return *(begin() + n);
    }

    inline reference operator[](size_type n)
    {
#if defined(_DEBUG) && DEBUG_LEVEL == 3
        if (n >= size()) throw error<const char*>("operator[] out of range", __FILE__, __LINE__);
#endif
        return *(begin() + n);
    }

    inline const value_type at(size_type n)const
    {
        return operator[](n);
    }

    inline void clear()
    {
        erase(begin(), end());
    }

    inline void add(const self& x)
    {
        reserve(size() + x.size());
        pointer p = finish;
        for (const_iterator i = x.begin(), m = x.end(); i != m; ++i, ++p) construct(p);
        finish = copy(x.begin(), x.end(), end());
    }

    inline void add_unique(const self& x)
    {
      for (const_iterator i = x.begin(), m = x.end(); i != m; ++i)
      {
        push_back_unique(*i);
      }
    }

    template <typename Compare>
    inline void add_unique(const self& x, Compare comp)
    {
        for (const_iterator i = x.begin(), m = x.end(); i != m; ++i)
        {
            push_back_unique(*i, comp);
        }
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
NAMESPACE_QLANGUAGE_LIBRARY_END

#endif
