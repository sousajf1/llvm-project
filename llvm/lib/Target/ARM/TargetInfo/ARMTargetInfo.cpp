//===-- ARMTargetInfo.cpp - ARM Target Implementation ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/ARMTargetInfo.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheARMLETarget() {
  static Target TheARMLETarget;
  return TheARMLETarget;
}
Target &llvm::getTheARMBETarget() {
  static Target TheARMBETarget;
  return TheARMBETarget;
}
Target &llvm::getTheThumbLETarget() {
  static Target TheThumbLETarget;
  return TheThumbLETarget;
}
Target &llvm::getTheThumbBETarget() {
  static Target TheThumbBETarget;
  return TheThumbBETarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeARMTargetInfo() {
  RegisterTarget<Triple::arm, /*HasJIT=*/true> const X(getTheARMLETarget(), "arm",
                                                 "ARM", "ARM");
  RegisterTarget<Triple::armeb, /*HasJIT=*/true> const Y(getTheARMBETarget(), "armeb",
                                                   "ARM (big endian)", "ARM");

  RegisterTarget<Triple::thumb, /*HasJIT=*/true> const A(getTheThumbLETarget(),
                                                   "thumb", "Thumb", "ARM");
  RegisterTarget<Triple::thumbeb, /*HasJIT=*/true> const B(
      getTheThumbBETarget(), "thumbeb", "Thumb (big endian)", "ARM");
}
