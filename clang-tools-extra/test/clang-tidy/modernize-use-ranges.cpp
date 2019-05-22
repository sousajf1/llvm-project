// RUN: %check_clang_tidy %s modernize-use-ranges %t -- -- \
// RUN:    -std=c++2a -I %S/Inputs/modernize-use-ranges

#include "array.h"
#include "algorithm.h"

using SomePolicy = int;

int *Begin(std::array<int, 4> &a);
int *End(std::array<int, 4> &a);

void single_range_all_of_everything() {
  std::array<int, 4> foo, bar;

  std::all_of(foo.begin(), foo.end(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::all_of(foo, [](int x) { return x == 0; });
  std::all_of(std::begin(foo), std::end(foo), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::all_of(foo, [](int x) { return x == 0; });

  std::all_of(std::cbegin(foo), std::cend(foo), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::all_of(foo, [](int x) { return x == 0; });
  std::all_of(foo.cbegin(), foo.cend(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::all_of(foo, [](int x) { return x == 0; });

  std::all_of((foo.cbegin()), foo.cend(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::all_of(foo, [](int x) { return x == 0; });

  // Try out more complicated transformations.
  std::all_of /* Some Comment */ ( /* Some Comment */ (foo.cbegin()), foo.cend(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::all_of ( /* Some Comment */ foo, [](int x) { return x == 0; });

  // Variations that are not diagnosed yet, because either advanced feature
  // or it would be incorrect, because not the full range of the container
  // is used.
  std::all_of(std::rbegin(foo), std::rend(foo), [](int x) { return x == 0; });
  std::all_of(std::crbegin(foo), std::crend(foo), [](int x) { return x == 0; });
  std::all_of(foo.rbegin(), foo.rend(), [](int x) { return x == 0; });
  std::all_of(foo.crbegin(), foo.crend(), [](int x) { return x == 0; });

  std::all_of(foo.begin() + 2, foo.end() - 2, [](int x) { return x == 0; });
  std::all_of(foo.begin(), foo.end() - 2, [](int x) { return x == 0; });
  std::all_of(foo.begin() + 2, foo.end(), [](int x) { return x == 0; });

  std::all_of(foo.cbegin() + 2, foo.cend() - 2, [](int x) { return x == 0; });
  std::all_of(foo.cbegin(), foo.cend() - 2, [](int x) { return x == 0; });
  std::all_of(foo.cbegin() + 2, foo.cend(), [](int x) { return x == 0; });

  std::all_of(std::begin(foo) + 2, std::end(foo) - 2, [](int x) { return x == 0; });
  std::all_of(std::begin(foo), std::end(foo) - 2, [](int x) { return x == 0; });
  std::all_of(std::begin(foo) + 2, std::end(foo), [](int x) { return x == 0; });

  std::all_of(std::cbegin(foo) + 2, std::cend(foo) - 2, [](int x) { return x == 0; });
  std::all_of(std::cbegin(foo), std::cend(foo) - 2, [](int x) { return x == 0; });
  std::all_of(std::cbegin(foo) + 2, std::cend(foo), [](int x) { return x == 0; });

  std::all_of(std::cend(foo) - 1, std::cbegin(foo) - 1, [](int x) { return x == 0; });
  std::all_of(Begin(foo), End(foo), [](int x) { return x == 0; });
  std::all_of(static_cast<int *>(foo.cbegin()), foo.cend(), [](int x) { return x == 0; });
  std::all_of(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x == 0; });

  // container-names missmatch and it's not safe to do the transformation.
  std::all_of(std::begin(foo), std::end(bar), [](int x) { return x == 0; });
  // TODO: use index-expr as container argument
}

void single_range_others() {
  std::array<int, 4> foo, bar;

  std::any_of(foo.begin(), foo.end(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::any_of(foo, [](int x) { return x == 0; });
  std::any_of(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x == 0; });

  std::none_of(foo.begin(), foo.end(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::none_of(foo, [](int x) { return x == 0; });
  std::none_of(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x == 0; });

  std::for_each(foo.begin(), foo.end(), [](int x) { return x == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::for_each(foo, [](int x) { return x == 0; });
  std::for_each(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x == 0; });

  std::count(foo.begin(), foo.end(), 42);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::count(foo, 42);
  std::count(SomePolicy(42), std::begin(foo), std::end(foo), 42);

  std::count_if(foo.begin(), foo.end(), [](int x) { return x % 2 == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::count_if(foo, [](int x) { return x % 2 == 0; });
  std::count_if(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x % 2 == 0; });

  std::find(foo.begin(), foo.end(), 42);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::find(foo, 42);
  std::find(SomePolicy(42), std::begin(foo), std::end(foo), 42);

  std::find_if(foo.begin(), foo.end(), [](int x) { return x % 2 == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::find_if(foo, [](int x) { return x % 2 == 0; });
  std::find_if(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x % 2 == 0; });

  std::find_if_not(foo.begin(), foo.end(), [](int x) { return x % 2 == 0; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::find_if_not(foo, [](int x) { return x % 2 == 0; });
  std::find_if_not(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x % 2 == 0; });

  std::adjacent_find(foo.begin(), foo.end());
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::adjacent_find(foo);
  std::adjacent_find(SomePolicy(42), std::begin(foo), std::end(foo));

  std::adjacent_find(foo.begin(), foo.end(), [](int x, int y) { return x + 2 == y; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::adjacent_find(foo, [](int x, int y) { return x + 2 == y; });
  std::adjacent_find(SomePolicy(42), std::begin(foo), std::end(foo), [](int x, int y) { return x + 2 == y; });

  std::fill(foo.begin(), foo.end(), 42);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::fill(foo, 42);
  std::fill(SomePolicy(42), std::begin(foo), std::end(foo), 42);

  std::generate(foo.begin(), foo.end(), 42);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::generate(foo, 42);
  std::generate(SomePolicy(42), std::begin(foo), std::end(foo), 42);

  std::remove(foo.begin(), foo.end(), 42);
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::remove(foo, 42);
  std::remove(SomePolicy(42), std::begin(foo), std::end(foo), 42);

  std::remove_if(foo.begin(), foo.end(), [](int x) { return x == 4; });
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: algorithm could be rewritten with std::ranges
  // CHECK-FIXES: std::ranges::remove_if(foo, [](int x) { return x == 4; });
  std::remove_if(SomePolicy(42), std::begin(foo), std::end(foo), [](int x) { return x == 4; });
}

void missmatch_algorithm() {
}
