#ifndef _QLANGUAGE_LIBRARY_HASHTABLE_H_
#define _QLANGUAGE_LIBRARY_HASHTABLE_H_

#include "hash.h"
#include "vector.h"

namespace QLanguage
{
    namespace Library
    {
#define HASHTABLE_TEMPLATE_ACHIEVE Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Compare
        template <typename T>
        class __hashtable_bucket_node
        {
        protected:
            typedef __hashtable_bucket_node<T> self;
        public:
            typedef T     value_type;
            typedef self* link_type;

            value_type data;
            link_type  next;

            __hashtable_bucket_node(const T& x);
        };

        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
        class hashtable;

        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
        class __hashtable_iterator_base : public bidirectional_iterator
        {
        protected:
            typedef hashtable<HASHTABLE_TEMPLATE_ACHIEVE> hashtable_type;
            typedef __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE> self;
            typedef __hashtable_bucket_node<Value>        node_type;

            node_type*      current;
            hashtable_type& hashtable;
        public:
            // I have no default construct.
            // Because iterator must belong to hashtable.
            __hashtable_iterator_base(node_type* node, hashtable_type& hashtable);
        };

        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length = 11,
            bool Resize = true,
            typename Hash = hash<Key>,
            typename Compare = equal_to<Key> >
        class hashtable
        {
        public:
            typedef size_t size_type;
        protected:
            typedef Key    key_type;
            typedef Value  value_type;
            typedef __hashtable_bucket_node<Value> node_type;
            typedef node_type* link_type;
            typedef hashtable<Key, Value, KeyOfValue, Max_Bucket_Length, Resize, Hash, Compare> self;
            typedef allocator<__hashtable_bucket_node<Value> > Alloc;

            vector<link_type> buckets;
            size_type         length;
            KeyOfValue        key;
            Hash              hash;
            Compare           compare;
        public:
            hashtable(size_type size);
            ~hashtable();

            inline size_type size()const
            {
                return length;
            }

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
