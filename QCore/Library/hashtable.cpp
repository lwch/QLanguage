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

        template <typename T>
        __hashtable_bucket_node<T>::__hashtable_bucket_node(const T& x)
            : data(x)
            , next(NULL)
        {
        }

        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
            __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>::__hashtable_iterator_base(node_type* node, hashtable_type& hashtable)
            : current(node)
            , hashtable(hashtable)
        {
        }

        template <typename Key, typename Value, typename KeyOfValue,
            size_t Max_Bucket_Length,
            bool Resize,
            typename Hash,
            typename Compare>
            void __hashtable_iterator_base<HASHTABLE_TEMPLATE_ACHIEVE>::inc()
        {
            current = current->next;
            if (current == NULL)
            {
                HASH_KEY_TYPE bucket = hashtable.index(current->data);
                typename __container_traits<typename hashtable_type::buckets_type>::size_type size = hashtable.buckets.size();
                while(++bucket < size && current = reinterpret_cast<typename hashtable_type::node_size_type*>(hashtable[bucket])->next && current == NULL);
            }
        }

        HASHTABLE_ACHIEVE_HEADER()::hashtable()
        {
        }

        HASHTABLE_ACHIEVE_HEADER()::hashtable(size_type size)
            : length(0)
        {
            buckets.reserve(size);
            for(typename __container_traits<buckets_type>::size_type i = 0; i < size; ++i)
            {
                node_size_type* node = Node_Size_Alloc::allocate();
                construct(node, 0);
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
                link_type current = reinterpret_cast<link_type>(reinterpret_cast<node_size_type*>(*i))->next;
                while(current)
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
    }
}
