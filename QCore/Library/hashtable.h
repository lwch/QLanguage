#ifndef _QLANGUAGE_LIBRARY_HASHTABLE_H_
#define _QLANGUAGE_LIBRARY_HASHTABLE_H_

#include "hash.h"
#include "vector.h"

#include "pair.h"
#include "function.h"

namespace QLanguage
{
    namespace Library
    {
#define HASHTABLE_TEMPLATE_ACHIEVE Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Compare
///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        class __hashtable_bucket_node
        {
        protected:
            typedef __hashtable_bucket_node<T> self;
        public:
            typedef T     value_type;
            typedef self* link_type;

            value_type data;
            link_type  prev;
            link_type  next;

            __hashtable_bucket_node(const T& x) : data(x), prev(NULL), next(NULL) {}
        };
///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
        class hashtable;
///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
        class __hashtable_iterator_base : public bidirectional_iterator
        {
            template <typename K, typename V, typename KOV,
                size_t MBL,
                bool R,
                typename H,
                typename C>
            friend class hashtable;
        protected:
            typedef hashtable<HASHTABLE_TEMPLATE_ACHIEVE> hashtable_type;
            typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE> self;
            typedef __hashtable_bucket_node<Value>        node_type;

            node_type*      node;
            hashtable_type& hashtable;
        public:
            // I have no default construct.
            // Because iterator must belong to hashtable.
            __hashtable_iterator_base(node_type* node, hashtable_type& hashtable) : node(node), hashtable(hashtable) {}
            
            inline const bool operator==(const self& x)const
            {
                return node == x.node;
            }
            
            inline const bool operator!=(const self& x)const
            {
                return node != x.node;
            }
            
            inline typename __container_traits<hashtable_type>::reference operator*()const
            {
                return node->data;
            }
            
            inline self& operator++()
            {
                inc();
                return *this;
            }
            
            inline self operator++(int)
            {
                self tmp = *this;
                ++(*this);
                return tmp;
            }
            
            inline self& operator--()
            {
                dec();
                return *this;
            }
            
            inline self operator--(int)
            {
                self tmp = *this;
                --(*this);
                return tmp;
            }
        protected:
            void inc();
            void dec();
        };
///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare,
            typename Size, typename Distance>
        class __hashtable_iterator : public __iterator<typename __container_traits<hashtable<HASHTABLE_TEMPLATE_ACHIEVE> >::value_type, Size, Distance>
                                   , public __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>
        {
        protected:
            typedef __hashtable_iterator<HASHTABLE_TEMPLATE_ACHIEVE, Size, Distance> self;
            typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>            base;
        public:
            __hashtable_iterator(typename base::node_type* node, typename base::hashtable_type& hashtable) : base(node, hashtable) {}
            
            inline typename __container_traits<hashtable<HASHTABLE_TEMPLATE_ACHIEVE> >::reference operator*()const
            {
                return *(*this);
            }
            
            inline self& operator++()
            {
                return ++(*this);
            }

            inline self operator++(int)
            {
                return (*this)++;
            }

            inline self& operator--()
            {
                return --(*this);
            }

            inline self operator--(int)
            {
                return (*this)--;
            }
        };
///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare,
            typename Size, typename Distance>
        class __hashtable_const_iterator : public __const_iterator<typename __container_traits<hashtable<HASHTABLE_TEMPLATE_ACHIEVE> >::value_type, Size, Distance>
                                         , public __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>
        {
        protected:
            typedef __hashtable_const_iterator<HASHTABLE_TEMPLATE_ACHIEVE, Size, Distance> self;
            typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>                  base;
        public:
            __hashtable_const_iterator(typename base::node_type* node, typename base::hashtable_type& hashtable) : base(node, hashtable) {}
            
            inline typename __container_traits<hashtable<HASHTABLE_TEMPLATE_ACHIEVE> >::reference operator*()const
            {
                return *(*this);
            }
            
            inline self& operator++()
            {
                return ++(*this);
            }

            inline self operator++(int)
            {
                return (*this)++;
            }

            inline self& operator--()
            {
                return --(*this);
            }

            inline self operator--(int)
            {
                return (*this)--;
            }
        };
///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length = 11,
            bool Resize = true,
            typename Hash = hash<Key>,
            typename Compare = equal_to<Key> >
        class hashtable
        {
        protected:
            typedef Key                            key_type;
            typedef __hashtable_bucket_node<Value> node_type;
            typedef node_type* link_type;
            typedef hashtable<Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Compare> self;
            typedef allocator<__hashtable_bucket_node<Value> > Alloc;
            typedef __hashtable_bucket_node<size_t> node_size_type;
            typedef allocator<node_size_type> Node_Size_Alloc;
            typedef vector<link_type> buckets_type;
        public:
            typedef Value        value_type;
            typedef Value*       pointer;
            typedef Value&       reference;
            typedef const Value& const_reference;
            typedef size_t       size_type;
            typedef ptrdiff_t    distance_type;
            typedef __hashtable_iterator<HASHTABLE_TEMPLATE_ACHIEVE, size_type, distance_type> iterator;
            typedef __hashtable_const_iterator<HASHTABLE_TEMPLATE_ACHIEVE, size_type, distance_type> const_iterator;
            typedef reverse_iterator<const_iterator, value_type, size_type, distance_type> const_reverse_iterator;
            typedef reverse_iterator<iterator, value_type, size_type, distance_type> reverse_iterator;
        protected:
            buckets_type buckets;
            size_type    length;
            KeyOfValue   key;
            Hash         hash;
            Compare      compare;
        public:
            hashtable();
            hashtable(size_type size);
            ~hashtable();

            inline size_type size()const
            {
                return length;
            }
            
            iterator insert_equal(const value_type& x);
            pair<iterator, bool> insert_unique(const value_type& x);

            void clear();
        protected:
            inline HASH_KEY_TYPE index(const value_type& x)
            {
                return hash(key(x)) % buckets.size();
            }
        };
    }
}

#endif
