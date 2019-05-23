// RUN: %check_clang_tidy %s modernize-use-noexcept %t -- \
// RUN:   -config="{CheckOptions: [{key: modernize-use-noexcept.AddMissingNoexcept, value: 1}]}" \
// RUN:   -- -std=c++11 -fexceptions

extern void conceptuallyUnknown();
void undefined();
void undefinedNoexcept() noexcept;

void empty() {}
// CHECK-MESSAGES: :[[@LINE-1]]:1: warning: this function can not throw an exception, consider adding 'noexcept'

int tryCatchBody() try {
  int Array[] = {1, 2, 3, 4};
  throw int(42);
  return Array[0] + Array[1] + Array[2] + Array[3];
} catch (...) {
  return 42;
}
// CHECK-MESSAGES: :[[@LINE-7]]:1: warning: this function can not throw an exception, consider adding 'noexcept'

int functionCanThrow() {
  throw int(42);
}

struct ConstructorWithNoexcept {
  ConstructorWithNoexcept() = default;
  int nonThrowingMember() { return 42; }
  // CHECK-MESSAGES: :[[@LINE-1]]:3: warning: this function can not throw an exception, consider adding 'noexcept'
  int nonThrowingButShown() noexcept { return 42; }
  void undefinedMethod();
  int Member;
};

struct VirtualMethods {
  virtual int doesNotThrowButOverriderMaybe() const { return 42; }
};

template <typename T>
void nonThrowingTemplateUnknown();

template <typename T>
T nonThrowingTemplateKnown() {
  // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: this function can not throw an exception, consider adding 'noexcept'
  return T{};
}
void instantiate() {
  // CHECK-MESSAGES: :[[@LINE-1]]:1: warning: this function can not throw an exception, consider adding 'noexcept'
  (void)nonThrowingTemplateKnown<int>();
  (void)nonThrowingTemplateKnown<float>();
  (void)nonThrowingTemplateKnown<double>();
}

void lambdas() {
  auto L1 = []() { return 42; };
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: this function can not throw an exception, consider adding 'noexcept'
  auto L2 = []() { throw 42; };

  auto L3 = []() { undefined(); };
  auto L4 = []() { undefinedNoexcept(); };
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: this function can not throw an exception, consider adding 'noexcept'
  auto L5 = []() {};
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: this function can not throw an exception, consider adding 'noexcept'

  auto L6 = []() noexcept { return 42; };
  auto L7 = []() noexcept { undefinedNoexcept(); };

  auto L8 = []() { conceptuallyUnknown(); undefinedNoexcept(); };
}
