#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace std {

//  ------------- 1 Range-algorithms ----------------

template <class InputIt, class UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p) { return true; }
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bool all_of(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, UnaryPredicate p) { return true; }

template <class InputIt, class UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p) { return true; }
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bool any_of(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, UnaryPredicate p) { return true; }

template <class InputIt, class UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p) { return true; }
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
bool none_of(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, UnaryPredicate p) { return true; }

template <class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f) { return f; }
template <class ExecutionPolicy, class ForwardIt, class UnaryFunction>
void for_each(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, UnaryFunction f) { return; }

template <class InputIt, class T>
ptrdiff_t count(InputIt first, InputIt last, const T &value) { return 0; }
template <class ExecutionPolicy, class ForwardIt, class T>
ptrdiff_t count(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, const T &value) { return 0; }

template <class InputIt, class UnaryPredicate>
ptrdiff_t count_if(InputIt first, InputIt last, UnaryPredicate p) { return 0; }
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
ptrdiff_t count_if(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, UnaryPredicate p) { return 0; }

template <class InputIt, class T>
InputIt find(InputIt first, InputIt last, const T &value) { return first; }
template <class ExecutionPolicy, class ForwardIt, class T>
ForwardIt find(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last, const T &value) { return first; }

template <class InputIt, class UnaryPredicate>
InputIt find_if(InputIt first, InputIt last, UnaryPredicate p) { return first; }
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
ForwardIt find_if(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last,
                  UnaryPredicate p) { return first; }

template <class InputIt, class UnaryPredicate>
InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q) { return first; }
template <class ExecutionPolicy, class ForwardIt, class UnaryPredicate>
ForwardIt find_if_not(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last,
                      UnaryPredicate q) { return first; }

template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) { return first; }
template <class ExecutionPolicy, class ForwardIterator>
ForwardIterator adjacent_find(ExecutionPolicy &&policy, ForwardIterator first,
                              ForwardIterator last) { return first; }

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
                              BinaryPredicate pred) { return first; }
template <class ExecutionPolicy, class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ExecutionPolicy &&policy, ForwardIterator first,
                              ForwardIterator last, BinaryPredicate pred) { return first; }

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T &value);
template <class ExecutionPolicy, class ForwardIterator, class T>
void fill(ExecutionPolicy &&policy, ForwardIterator first, ForwardIterator last, const T &value);

template <class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {}
template <class ExecutionPolicy, class ForwardIterator, class Generator>
void generate(ExecutionPolicy &&policy, ForwardIterator first, ForwardIterator last, Generator gen) {}

template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T &value) { return first; }
template <class ExecutionPolicy, class ForwardIterator, class T>
ForwardIterator remove(ExecutionPolicy &&policy, ForwardIterator first, ForwardIterator last, const T &value) { return first; }

template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, 
                          Predicate pred) { return first; }
template <class ExecutionPolicy, class ForwardIterator, class Predicate>
ForwardIterator remove_if(ExecutionPolicy &&policy, ForwardIterator first,
                          ForwardIterator last, Predicate pred) { return first; }

template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) { return first; }
template <class ExecutionPolicy, class ForwardIterator>
ForwardIterator unique(ExecutionPolicy &&policy, ForwardIterator first, ForwardIterator last) { return first; }

//  ------------- 2 Range-algorithms ----------------

#if 0
template <class InputIt1, class InputIt2>
void mismatch(InputIt1 first1, InputIt1 last1,
              InputIt2 first2);
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
void mismatch(ExecutionPolicy &&policy, ForwardIt1 first1, ForwardIt1 last1,
              ForwardIt2 first2);

template <class InputIt1, class InputIt2, class BinaryPredicate>
void mismatch(InputIt1 first1, InputIt1 last1,
              InputIt2 first2, BinaryPredicate p);
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate>
void mismatch(ExecutionPolicy &&policy, ForwardIt1 first1, ForwardIt1 last1,
              ForwardIt2 first2, BinaryPredicate p);

template <class InputIt1, class InputIt2>
void mismatch(InputIt1 first1, InputIt1 last1,
              InputIt2 first2, InputIt2 last2);
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
void mismatch(ExecutionPolicy &&policy, ForwardIt1 first1, ForwardIt1 last1,
              ForwardIt2 first2, ForwardIt2 last2);

template <class InputIt1, class InputIt2, class BinaryPredicate>
void mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2,
              BinaryPredicate p);
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate>
void mismatch(ExecutionPolicy &&policy, ForwardIt1 first1, ForwardIt1 last1,
              ForwardIt2 first2, ForwardIt2 last2, BinaryPredicate p);



template <class ForwardIt1, class ForwardIt2>
ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last) { return first; }
template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2>
ForwardIt1 find_end(ExecutionPolicy &&policy, ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last) { return first; }

template <class ForwardIt1, class ForwardIt2, class BinaryPredicate>
ForwardIt1 find_end(ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p) { return first; }

template <class ExecutionPolicy, class ForwardIt1, class ForwardIt2, class BinaryPredicate>
ForwardIt1 find_end(ExecutionPolicy &&policy, ForwardIt1 first, ForwardIt1 last,
                    ForwardIt2 s_first, ForwardIt2 s_last, BinaryPredicate p) { return first; }
#endif
} // namespace std

#endif /* end of include guard: ALGORITHM_H */
