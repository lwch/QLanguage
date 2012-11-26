#ifndef _QLANGUAGE_LIBRARY_TRAITS_H_
#define _QLANGUAGE_LIBRARY_TRAITS_H_

namespace QLanguage
{
    namespace Library
    {
        template <typename T>
        struct __container_traits
        {
            typedef typename T::value_type       value_type;
            typedef typename T::pointer          pointer;
            typedef typename T::reference        reference;
            typedef typename T::const_reference  const_reference;
            typedef typename T::size_type        size_type;
            typedef typename T::distance_type    distance_type;
            typedef typename T::const_iterator   const_iterator;
            typedef typename T::const_reverse_iterator const_reverse_iterator;
            typedef typename T::iterator         iterator;
            typedef typename T::reverse_iterator reverse_iterator;
        };

        struct __true_type
        {
        };

        struct __false_type
        {
        };

        template <typename T>
        struct __type_traits
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __true_type  has_destruct;
            typedef __false_type is_POD;
        };

        template <>
        struct __type_traits<char>
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <>
        struct __type_traits<wchar_t>
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <>
        struct __type_traits<int>
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <>
        struct __type_traits<long>
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <>
        struct __type_traits<float>
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <>
        struct __type_traits<double>
        {
            typedef __true_type  has_default_construct;
            typedef __true_type  has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <typename T>
        struct __type_traits<T*>
        {
            typedef __false_type has_default_construct;
            typedef __false_type has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <typename T>
        struct __type_traits<const T*>
        {
            typedef __false_type has_default_construct;
            typedef __false_type has_copy_construct;
            typedef __true_type  has_assign_operator;
            typedef __false_type has_destruct;
            typedef __true_type  is_POD;
        };

        template <typename T>
        inline typename __type_traits<T>::has_destruct* has_destruct(const T&)
        {
            return static_cast<typename __type_traits<T>::has_destruct*>(0);
        }

        template <typename T>
        inline typename __type_traits<T*>::has_destruct* has_destruct(T*)
        {
            return static_cast<typename __type_traits<T*>::has_destruct*>(0);
        }

        template <typename T>
        inline typename __type_traits<const T*>::has_destruct* has_destruct(const T*)
        {
            return static_cast<typename __type_traits<const T*>::has_destruct*>(0);
        }

        template <typename T>
        inline typename __type_traits<T>::is_POD* is_POD(const T&)
        {
            return static_cast<typename __type_traits<T>::is_POD*>(0);
        }

        template <typename T>
        inline typename __type_traits<T*>::is_POD* is_POD(T*)
        {
            return static_cast<typename __type_traits<T*>::is_POD*>(0);
        }

        template <typename T>
        inline typename __type_traits<const T*>::is_POD* is_POD(const T*)
        {
            return static_cast<typename __type_traits<const T*>::is_POD*>(0);
        }

        template <typename T>
        inline const bool type_compare(T, T)
        {
            return true;
        }

        template <typename T1, typename T2>
        inline const bool type_compare(T1, T2)
        {
            return false;
        }

        template <typename T>
        inline const bool type_compare(T*, T*)
        {
            return true;
        }

        template <typename T1, typename T2>
        inline const bool type_compare(T1*, T2*)
        {
            return false;
        }

        template <typename T>
        inline const bool type_compare(const T*, const T*)
        {
            return true;
        }

        template <typename T1, typename T2>
        inline const bool type_compare(const T1*, const T2*)
        {
            return false;
        }

        template <typename T1, typename T2>
        inline const bool type_compare(const T1*, T2*)
        {
            return false;
        }

        template <typename T1, typename T2>
        inline const bool type_compare(T1*, const T2*)
        {
            return false;
        }
    }
}

#endif
