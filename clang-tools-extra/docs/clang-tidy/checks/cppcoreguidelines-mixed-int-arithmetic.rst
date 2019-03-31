.. title:: clang-tidy - cppcoreguidelines-mixed-int-arithmetic

cppcoreguidelines-mixed-int-arithmetic
======================================

This check enforces `ES. 100 <http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es100-dont-mix-signed-and-unsigned-arithmetic>`_
and `ES. 102 <http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es102-use-signed-types-for-arithmetic>`_
of the `CppCoreGuidelines <http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c-core-guidelines>`_
addressing the use of unsigned types in integer arithmetic.

Because of the subtile difference between ``signed`` and ``unsigned`` integer
types in C++ it is recommended to use ``signed`` types in general for arithmetic
and to not mix ``signed`` and ``unsigned`` integers in arithmetic expressions.

The behaviour of ``signed`` integer type is undefined when an overflow occurs.
On the contrary ``unsigned`` types will wrap around leading to potentially
unexpected results of integer computations.

.. code-block:: c++

    // Taken from the CppCoreGuidelines
    template<typename T, typename T2>
    T subtract(T x, T2 y)
    {
        return x - y;
    }

    void test()
    {
        int s = 5;
        unsigned int us = 5;
        cout << subtract(s, 7) << '\n';       // -2
        cout << subtract(us, 7u) << '\n';     // 4294967294
        cout << subtract(s, 7u) << '\n';      // -2
        cout << subtract(us, 7) << '\n';      // 4294967294
        cout << subtract(s, us + 2) << '\n';  // -2
        cout << subtract(us, s + 2) << '\n';  // 4294967294
    }
