// RUN: %check_clang_tidy %s cppcoreguidelines-const-correctness %t -- \
// RUN:   -config="{CheckOptions: [\
// RUN:   {key: 'cppcoreguidelines-const-correctness.TransformValues', value: 1}, \
// RUN:   {key: 'cppcoreguidelines-const-correctness.WarnPointersAsValues', value: 0}, \
// RUN:   {key: 'cppcoreguidelines-const-correctness.TransformPointersAsValues', value: 0}, \
// RUN:   ]}" -- -fno-delayed-template-parsing -fms-extensions

struct S {};

void f(__unaligned S *);

void scope() {
  // FIXME: This is a bug in the analysis, that is confused by '__unaligned'.
  // https://bugs.llvm.org/show_bug.cgi?id=51756
  S s;
  // CHECK-MESSAGES:[[@LINE-1]]:3: warning: variable 's' of type 'S' can be declared 'const'
  f(&s);
}
