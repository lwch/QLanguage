/********************************************************************
	created:	2012/11/29
	created:	29:11:2012   23:41
	filename: 	\QCoreUnitTest\Units\TestTraits.cpp
	file path:	\QCoreUnitTest\Units
	file base:	TestTraits
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/

#include "TestTraits.h"

struct A {};
struct B {};
class  C {};
class  D {};

struct E : public A {};
class  F : public A {};
struct G : public C {};
class  H : public C {};

struct I : virtual public A {};
class  J : virtual public A {};
struct K : virtual public C {};
struct L : virtual public C {};

TEST_CASE(TestTraits)
{
    TEST_ASSERT(!type_compare(A(), A()), "invalid type_compare with A and A!");
    TEST_ASSERT(type_compare(A(), B()), "invalid type_compare with A and B!");
    TEST_ASSERT(!type_compare(C(), C()), "invalid type_compare with C and C!");
    TEST_ASSERT(type_compare(C(), D()), "invalid type_compare with C and D!");

    TEST_ASSERT(!type_compare(static_cast<A*>(0), static_cast<A*>(0)), "invalid type_compare with A* and A*!");
    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<B*>(0)), "invalid type_compare with A* and B*!");
    TEST_ASSERT(!type_compare(static_cast<C*>(0), static_cast<C*>(0)), "invalid type_compare with C* and C*!");
    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<D*>(0)), "invalid type_compare with C* and D*!");

    TEST_ASSERT(!type_compare(static_cast<const A*>(0), static_cast<const A*>(0)), "invalid type_compare with const A* and const A*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<A*>(0)), "invalid type_compare with const A* and A*!");
    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<const A*>(0)), "invalid type_compare with A* and const A*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<B*>(0)), "invalid type_compare with const A* and B*!");
    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<const B*>(0)), "invalid type_compare with A* and const B*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<const B*>(0)), "invalid type_compare with const A* and const B*!");

    TEST_ASSERT(!type_compare(static_cast<const C*>(0), static_cast<const C*>(0)), "invalid type_compare with const C* and const C*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<C*>(0)), "invalid type_compare with const C* and C*!");
    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<const C*>(0)), "invalid type_compare with C* and const C*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<const D*>(0)), "invalid type_compare with const C* and const D*!");

    TEST_ASSERT(type_compare(A(), E()), "invalid type_compare with A and E!");
    TEST_ASSERT(type_compare(A(), F()), "invalid type_compare with A and F!");

    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<E*>(0)), "invalid type_compare with A* and E*!");
    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<F*>(0)), "invalid type_compare with A* and F*!");

    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<const E*>(0)), "invalid type_compare with A* and const E*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<E*>(0)), "invalid type_compare with const A* and E*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<const E*>(0)), "invalid type_compare with const A* and const E*!");

    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<const F*>(0)), "invalid type_compare with A* and const F*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<F*>(0)), "invalid type_compare with const A* and F*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<const F*>(0)), "invalid type_compare with const A* and const F*!");

    TEST_ASSERT(type_compare(C(), G()), "invalid type_compare with C and G!");
    TEST_ASSERT(type_compare(C(), H()), "invalid type_compare with C and H!");

    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<G*>(0)), "invalid type_compare with C* and G*!");
    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<H*>(0)), "invalid type_compare with C* and H*!");

    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<const G*>(0)), "invalid type_compare with C* and const G*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<G*>(0)), "invalid type_compare with const C* and G*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<const G*>(0)), "invalid type_compare with const C* and const G*!");

    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<const H*>(0)), "invalid type_compare with C* and const H*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<H*>(0)), "invalid type_compare with const C* and H*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<const H*>(0)), "invalid type_compare with const C* and const H*!");

    TEST_ASSERT(type_compare(A(), I()), "invalid type_compare with A and I!");
    TEST_ASSERT(type_compare(A(), J()), "invalid type_compare with A and J!");

    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<I*>(0)), "invalid type_compare with A* and I*!");
    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<J*>(0)), "invalid type_compare with A* and J*!");

    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<const I*>(0)), "invalid type_compare with A* and const I*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<I*>(0)), "invalid type_compare with const A* and I*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<const I*>(0)), "invalid type_compare with const A* and const I*!");

    TEST_ASSERT(type_compare(static_cast<A*>(0), static_cast<const J*>(0)), "invalid type_compare with A* and const J*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<J*>(0)), "invalid type_compare with const A* and J*!");
    TEST_ASSERT(type_compare(static_cast<const A*>(0), static_cast<const J*>(0)), "invalid type_compare with const A* and const J*!");

    TEST_ASSERT(type_compare(C(), K()), "invalid type_compare with C and K!");
    TEST_ASSERT(type_compare(C(), L()), "invalid type_compare with C and L!");

    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<K*>(0)), "invalid type_compare with C* and K*!");
    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<L*>(0)), "invalid type_compare with C* and L*!");

    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<const K*>(0)), "invalid type_compare with C* and const K*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<K*>(0)), "invalid type_compare with const C* and K*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<const K*>(0)), "invalid type_compare with const C* and const K*!");

    TEST_ASSERT(type_compare(static_cast<C*>(0), static_cast<const L*>(0)), "invalid type_compare with C* and const L*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<L*>(0)), "invalid type_compare with const C* and L*!");
    TEST_ASSERT(type_compare(static_cast<const C*>(0), static_cast<const L*>(0)), "invalid type_compare with const C* and const L*!");

    TEST_ASSERT(type_compare(has_destruct(A()), static_cast<__false_type*>(0)), "A hasn't destruct!");
    TEST_ASSERT(type_compare(has_destruct(static_cast<A*>(0)), static_cast<__true_type*>(0)), "A* hasn't destruct!");
    TEST_ASSERT(type_compare(has_destruct(static_cast<const A*>(0)), static_cast<__true_type*>(0)), "const A* hasn't destruct!");

    TEST_ASSERT(type_compare(has_destruct(int()), static_cast<__true_type*>(0)), "int hasn't destruct!");
    TEST_ASSERT(type_compare(has_destruct(static_cast<int*>(0)), static_cast<__true_type*>(0)), "int* hasn't destruct!");
    TEST_ASSERT(type_compare(has_destruct(static_cast<const int*>(0)), static_cast<__true_type*>(0)), "const int* hasn't destruct!");

    TEST_ASSERT(type_compare(is_POD(A()), static_cast<__true_type*>(0)), "A hasn't destruct!");
    TEST_ASSERT(type_compare(is_POD(static_cast<A*>(0)), static_cast<__false_type*>(0)), "A* hasn't destruct!");
    TEST_ASSERT(type_compare(is_POD(static_cast<const A*>(0)), static_cast<__false_type*>(0)), "const A* hasn't destruct!");

    TEST_ASSERT(type_compare(is_POD(int()), static_cast<__false_type*>(0)), "int hasn't destruct!");
    TEST_ASSERT(type_compare(is_POD(static_cast<int*>(0)), static_cast<__false_type*>(0)), "int* hasn't destruct!");
    TEST_ASSERT(type_compare(is_POD(static_cast<const int*>(0)), static_cast<__false_type*>(0)), "const int* hasn't destruct!");
}
