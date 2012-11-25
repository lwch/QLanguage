#include "hashtable.h"

#define HASHTABLE_ACHIEVE_HEADER(...) \
    template <typename Key, typename Value, typename KeyOfValue, \
    size_t Max_Bucket_Length, \
    bool Resize, \
    typename Hash, \
    typename Compare> \
##__VA_ARGS__ hashtable<HASHTABLE_TEMPLATE_ACHIEVE>

namespace QLanguage
{
    namespace Library
    {
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

        HASHTABLE_ACHIEVE_HEADER()::hashtable(size_type size)
            : length(0)
        {
            buckets.reserve(size);
            typedef __hashtable_bucket_node<size_type> type;
            typedef allocator<type> Alloc;
            for(size_type i = 0; i < size; ++i)
            {
                type* node = Alloc::allocate();
                construct(node, 0);
                buckets.push_back(reinterpret_cast<link_type>(node));
            }
        }

        HASHTABLE_ACHIEVE_HEADER()::~hashtable()
        {
            typedef __hashtable_bucket_node<size_type> type;
            typedef allocator<type> Alloc;
            for(size_type i = 0; i < buckets.size(); ++i)
            {
                Alloc::deallocate(reinterpret_cast<type>(buckets[i]));
            }
        }

        HASHTABLE_ACHIEVE_HEADER(void)::clear()
        {
            typedef __hashtable_bucket_node<size_type> type;
            for(vector<link_type>::iterator i = buckets.begin(); i != buckets.end(); ++i)
            {
                link_type current = reinterpret_cast<link_type>(reinterpret_cast<type*>(*i))->next;
                while(current)
                {
                    link_type next = current->next;
                    destruct(&current->data);
                    Alloc::deallocate(current);
                    current = next;
                }
                reinterpret_cast<type*>(*i)->next = NULL;
            }
            length = 0;
        }
    }
}
