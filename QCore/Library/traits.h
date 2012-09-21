#ifndef _QLANGUAGE_LIBRARY_TRAITS_H_
#define _QLANGUAGE_LIBRARY_TRAITS_H_

namespace QLanguage
{
    namespace Library
    {
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
            static_assert(false, "Please use const T& not T*");
            return static_cast<typename __type_traits<T>::has_destruct*>(0);
        }

        template <typename T>
        inline typename __type_traits<const T*>::has_destruct* has_destruct(const T*)
        {
            static_assert(false, "Please use const T& not const T*");
            return static_cast<typename __type_traits<T>::has_destruct*>(0);
        }

        template <typename T>
        inline typename __type_traits<T>::is_POD* is_POD(const T&)
        {
            return static_cast<typename __type_traits<T>::is_POD*>(0);
        }

        template <typename T>
        inline typename __type_traits<T*>::is_POD* is_POD(T*)
        {
            return static_cast<typename __type_traits<T>::is_POD*>(0);
        }

        template <typename T>
        inline typename __type_traits<const T*>::is_POD* is_POD(const T*)
        {
            return static_cast<typename __type_traits<T>::is_POD*>(0);
        }

        template <typename T>
        inline const bool compare_type(T, T)
        {
            return true;
        }

        template <typename T1, typename T2>
        inline const bool compare_type(T1, T2)
        {
            return false;
        }

        template <typename T>
        inline const bool compare_type(T*, T*)
        {
            return true;
        }

        template <typename T1, typename T2>
        inline const bool compare_type(T1*, T2*)
        {
            return false;
        }

        template <typename T>
        inline const bool compare_type(const T*, const T*)
        {
            return true;
        }

        template <typename T1, typename T2>
        inline const bool compare_type(const T1*, const T2*)
        {
            return false;
        }

        template <typename T1, typename T2>
        inline const bool compare_type(const T1*, T2*)
        {
            return false;
        }

        template <typename T1, typename T2>
        inline const bool compare_type(T1*, const T2*)
        {
            return false;
        }
    }
}

#endif
