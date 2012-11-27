#include "hashtable.h"
#include "traits.h"

namespace QLanguage
{
    namespace Library
    {
#define HASHTABLE_ACHIEVE_HEADER(...) \
    template <typename Key, typename Value, typename KeyOfValue, \
    size_t Max_Bucket_Length, \
    bool Resize, \
    typename Hash, \
    typename Compare> \
    __VA_ARGS__ hashtable<HASHTABLE_TEMPLATE_ACHIEVE>

///////////////////////////////////////////////////////////////////////////////////////////////////
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
            void __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>::inc()
        {
            node_type* prev = node;
            node = node->next;
            if (node == prev)
            {
                HASH_KEY_TYPE bucket = hashtable.index(node->data);
                typename __container_traits<typename hashtable_type::buckets_type>::size_type size = hashtable.buckets.size();
                while(++bucket < size && node = reinterpret_cast<typename hashtable_type::node_size_type*>(hashtable[bucket])->next && node == hashtable[bucket]);
            }
        }
        
        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
            void __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>::dec()
        {
            node_type* prev = node;
            node = node->prev;
            if (node == prev)
            {
                HASH_KEY_TYPE bucket = hashtable.index(node->data);
                while(--bucket >= 0 && node = reinterpret_cast<typename hashtable_type::node_size_type*>(hashtable[bucket])->next && node == hashtable[bucket]);
            }
        }
///////////////////////////////////////////////////////////////////////////////////////////////////
        HASHTABLE_ACHIEVE_HEADER()::hashtable()
            : length(0)
        {
            const size_type defaultSize = 11;
            buckets.reserve(defaultSize);
            for(typename __container_traits<buckets_type>::size_type i = 0; i < defaultSize; ++i)
            {
                node_size_type* node = Node_Size_Alloc::allocate();
                construct(node, 0);
                node->prev = node;
                node->next = node;
                buckets.push_back(reinterpret_cast<link_type>(node));
            }
        }
        
        HASHTABLE_ACHIEVE_HEADER()::hashtable(size_type size)
            : length(0)
        {
            buckets.reserve(size);
            for(typename __container_traits<buckets_type>::size_type i = 0; i < size; ++i)
            {
                node_size_type* node = Node_Size_Alloc::allocate();
                construct(node, 0);
                node->prev = node;
                node->next = node;
                buckets.push_back(reinterpret_cast<link_type>(node));
            }
        }

        HASHTABLE_ACHIEVE_HEADER()::~hashtable()
        {
            clear();
            for(typename __container_traits<buckets_type>::size_type i = 0; i < buckets.size(); ++i)
            {
                Node_Size_Alloc::deallocate(reinterpret_cast<node_size_type*>(buckets[i]));
            }
        }

        HASHTABLE_ACHIEVE_HEADER(void)::clear()
        {
            for(typename __container_traits<buckets_type>::iterator i = buckets.begin(); i != buckets.end(); ++i)
            {
                link_type header = *i;
                link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(*i))->next;
                while(current != header)
                {
                    link_type next = current->next;
                    destruct(&current->data, has_destruct(current->data));
                    Alloc::deallocate(current);
                    current = next;
                }
                reinterpret_cast<node_size_type*>(*i)->data = 0;
                reinterpret_cast<node_size_type*>(*i)->next = NULL;
            }
            length = 0;
        }
        
        HASHTABLE_ACHIEVE_HEADER(typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator)::insert_equal(const value_type& x)
        {
            node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(key(x))]);
            link_type tail = reinterpret_cast<link_type>(head->prev);
            link_type node = Alloc::allocate();
            construct(node, x);
            head->prev = reinterpret_cast<node_size_type*>(node);
            node->prev = tail;
            node->next = reinterpret_cast<link_type>(head);
            tail->next = node;
            ++head->data;
            return iterator(node, *this);
        }
        
        HASHTABLE_ACHIEVE_HEADER(pair<typename hashtable<HASHTABLE_TEMPLATE_ACHIEVE>::iterator, bool>)::insert_unique(const value_type& x)
        {
            node_size_type* head = reinterpret_cast<node_size_type*>(buckets[index(key(x))]);
            link_type current = reinterpret_cast<link_type>(head->next);
            while (current != reinterpret_cast<link_type>(head))
            {
                if (compare(key(x), key(current->data)))
                {
                    return pair<iterator, bool>(iterator(reinterpret_cast<link_type>(head), *this), false);
                }
                current = current->next;
            }
            link_type tail = reinterpret_cast<link_type>(head->prev);
            link_type node = Alloc::allocate();
            construct(node, x);
            head->prev = reinterpret_cast<node_size_type*>(node);
            node->prev = tail;
            node->next = reinterpret_cast<link_type>(head);
            tail->next = node;
            ++head->data;
            return pair<iterator, bool>(iterator(node, *this), true);
        }
    }
}
