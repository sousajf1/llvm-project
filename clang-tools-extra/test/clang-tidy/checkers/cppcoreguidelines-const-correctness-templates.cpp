// RUN: %check_clang_tidy %s cppcoreguidelines-const-correctness %t -- \
// RUN:   -config="{CheckOptions: [\
// RUN:   {key: 'cppcoreguidelines-const-correctness.TransformValues', value: 1}, \
// RUN:   {key: 'cppcoreguidelines-const-correctness.TransformReferences', value: 1}, \
// RUN:   {key: 'cppcoreguidelines-const-correctness.WarnPointersAsValues', value: 0}, \
// RUN:   {key: 'cppcoreguidelines-const-correctness.TransformPointersAsValues', value: 0}, \
// RUN:   ]}" -- -fno-delayed-template-parsing

template <typename T>
void type_dependent_variables() {
  T value = 42;
  auto &ref = value;
  T &templateRef = value;

  int value_int = 42;
  // CHECK-MESSAGES:[[@LINE-1]]:3: warning: variable 'value_int' of type 'int' can be declared 'const'
}
void instantiate_template_cases() {
  type_dependent_variables<int>();
  type_dependent_variables<float>();
}
