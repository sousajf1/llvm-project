// RUN: %check_clang_tidy %s cppcoreguidelines-mixed-int-arithmetic %t

enum UnsignedEnum : unsigned char {
  UEnum1,
  UEnum2
};

enum SignedEnum : signed char {
  SEnum1,
  SEnum2
};

unsigned char returnUnsignedCharacter() { return 42; }
unsigned returnUnsignedNumber() { return 42u; }
long returnBigNumber() { return 42; }
float unrelatedThing() { return 42.f; }
SignedEnum returnSignedEnum() { return SEnum1; }
UnsignedEnum returnUnsignedEnum() { return UEnum1; }

void mixed_binary() {
  unsigned int UInt1 = 42;
  signed int SInt1 = 42;
  UnsignedEnum UE1 = UEnum1;
  SignedEnum SE1 = SEnum1;
  float UnrelatedFloat = 42.f;

  // Test traditional integer types.
  auto R1 = UInt1 + SInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:21: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:13: note: unsigned operand

  int R2 = UInt1 - SInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:12: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:20: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:12: note: unsigned operand

  unsigned int R3 = UInt1 * SInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:21: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:29: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:21: note: unsigned operand

  unsigned int R4 = UInt1 / returnBigNumber();
  // CHECK-MESSAGES: [[@LINE-1]]:21: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:29: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:21: note: unsigned operand

  char R5 = returnUnsignedCharacter() + SInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:41: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:13: note: unsigned operand

  auto R6 = SInt1 - 10u;
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:13: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:21: note: unsigned operand

  auto R7 = UInt1 * 10;
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:21: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:13: note: unsigned operand

  auto R8 = 10u / returnBigNumber();
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:19: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:13: note: unsigned operand

  auto R9 = 10 + returnUnsignedCharacter();
  // CHECK-MESSAGES: [[@LINE-1]]:13: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:13: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:18: note: unsigned operand

  // Test enum types.
  char R10 = returnUnsignedEnum() - SInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:37: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:14: note: unsigned operand

  unsigned char R11 = returnSignedEnum() * UInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:23: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:44: note: unsigned operand

  char R12 = UE1 / SInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:20: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:14: note: unsigned operand

  unsigned char R13 = SE1 + UInt1;
  // CHECK-MESSAGES: [[@LINE-1]]:23: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:23: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:29: note: unsigned operand

  auto R14 = SE1 - 10u;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:14: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:20: note: unsigned operand

  auto R15 = UE1 * 10;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:20: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:14: note: unsigned operand

  auto R16 = returnSignedEnum() / 10u;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:14: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:35: note: unsigned operand

  auto R17 = returnUnsignedEnum() + 10;
  // CHECK-MESSAGES: [[@LINE-1]]:14: warning: mixed signed and unsigned arithmetic; prefer signed integers and use unsigned types only for modulo arithmetic
  // CHECK-MESSAGES: [[@LINE-2]]:37: note: signed operand
  // CHECK-MESSAGES: [[@LINE-3]]:14: note: unsigned operand

  // Check that floating pointer numbers do not interfere improperly.
  // Implicit conversion from float to int are covered in other checks.
  int Ok1 = UInt1 + UnrelatedFloat;
  unsigned int Ok2 = SInt1 - UnrelatedFloat;
  int Ok3 = UInt1 * unrelatedThing();
  unsigned int Ok4 = SInt1 / unrelatedThing();
  auto Ok5 = 10 + UnrelatedFloat;
  auto Ok6 = 10u - UnrelatedFloat;
}

void mixed_assignments() {
}

void pure_unsigned() {
  unsigned int UInt1 = 42u;
  unsigned char UChar1 = 42u;
  UnsignedEnum UE1 = UEnum1;
  float UnrelatedFloat = 42.f;

  auto Ok1 = UInt1 + UChar1;
  auto Ok2 = UChar1 + UInt1;
  auto Ok3 = UInt1 + returnUnsignedCharacter();
  auto Ok4 = UChar1 + returnUnsignedCharacter();
  auto Ok5 = returnUnsignedCharacter() + returnUnsignedCharacter();
  auto Ok6 = UInt1 + UE1;
  auto Ok7 = UInt1 + returnUnsignedEnum();
  auto Ok8 = UE1 + UE1;
  // FIXME: unsigned character converts to 'int' and pollutes the result.
  // http://en.cppreference.com/w/cpp/language/implicit_conversion  Integral conversions
  // if `returnUnsignedCharacter()` would return char, the conversion would result
  // in either `signed int` or `unsigned int` (arch dependent i guess). Both `short`
  // and `char` perform this conversion in arithmetic operations. This would probably
  // need some bigger magic to match in the AST, but should be possible in theory.
  auto Ok9 = 10u * (returnUnsignedNumber() + returnUnsignedEnum());
  auto Ok10 = 10u * (10u + 10ul);
  auto Ok11 = 10u * (10u + returnUnsignedEnum());
  auto Ok12 = returnUnsignedCharacter() * (10u + returnUnsignedEnum());

  // Test that unrelated types do not interfere.
  auto OkUnrelated1 = UInt1 + UnrelatedFloat;
  auto OkUnrelated2 = UChar1 + unrelatedThing();
  auto OkUnrelated3 = UE1 + UnrelatedFloat;
  auto OkUnrelated4 = UE1 + unrelatedThing();

  // Test that correct assignments to not cause warnings.
  UInt1 += 1u;
  UInt1 -= returnUnsignedCharacter();
  UInt1 *= UE1;
  UInt1 /= returnUnsignedEnum();
  UInt1 += (returnUnsignedCharacter() + returnUnsignedEnum());
}

void pure_signed() {
  int SInt1 = 42u;
  signed char SChar1 = 42u;
  SignedEnum SE1 = SEnum1;

  float UnrelatedFloat = 42.f;

  auto Ok1 = SInt1 + SChar1;
  auto Ok2 = SChar1 + SInt1;
  auto Ok3 = SInt1 + returnBigNumber();
  auto Ok4 = SChar1 + returnBigNumber();
  auto Ok5 = returnBigNumber() + returnBigNumber();
  auto Ok6 = SInt1 + SE1;
  auto Ok7 = SInt1 + returnSignedEnum();
  auto Ok8 = SE1 + SE1;
  auto Ok9 = 10 * (returnBigNumber() + returnSignedEnum());

  // Test that unrelated types do not interfere.
  auto OkUnrelated1 = SInt1 + UnrelatedFloat;
  auto OkUnrelated2 = SChar1 + unrelatedThing();
  auto OkUnrelated3 = SE1 + UnrelatedFloat;
  auto OkUnrelated4 = SE1 + unrelatedThing();
}
