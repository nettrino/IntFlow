// Check explicit truncation/lossy cast instrumentation.
// Implicit test prefix only verifies instrumentation is not added.
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - %s \
// RUN:   -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - -xc++ %s \
// RUN:   -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - %s \
// RUN:   -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - -xc++ %s \
// RUN:   -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT

unsigned int  int_u;
  signed int  int_s;
unsigned char char_u;
  signed char char_s;

// Don't mangle function names when building as C++
#ifdef __cplusplus
extern "C" void testUUExt();
extern "C" void testUSExt();
extern "C" void testSUExt();
extern "C" void testSSExt();
extern "C" void testUUTrunc();
extern "C" void testUSTrunc();
extern "C" void testSUTrunc();
extern "C" void testSSTrunc();
#endif // __cplusplus

// EXPLICIT: define void @testUUExt
// IMPLICIT: define void @testUUExt
void testUUExt() {
  // EXPLICIT:      load
  // EXPLICIT-NEXT: zext
  // EXPLICIT-NEXT: store
  // EXPLICIT-NOT:  call
  //
  // IMPLICIT-NOT:  call
  int_u = (unsigned int)char_u;
}

// EXPLICIT: define void @testUSExt
// IMPLICIT: define void @testUSExt
void testUSExt() {
  // EXPLICIT:      [[T1:%.*]] = load i8
  // EXPLICIT-NEXT: [[T2:%.*]] = icmp slt i8 [[T1]], 0
  // EXPLICIT-NOT:  icmp
  // EXPLICIT:      call
  //
  // IMPLICIT-NOT:  call
  int_u = (unsigned int)char_s;
}

// EXPLICIT: define void @testSUExt
// IMPLICIT: define void @testSUExt
void testSUExt() {
  // EXPLICIT:      load
  // EXPLICIT-NEXT: zext
  // EXPLICIT-NEXT: store
  // EXPLICIT-NOT:  call
  //
  // IMPLICIT-NOT:  call
  int_s = (signed int)char_u;
}

// EXPLICIT: define void @testSSExt
// IMPLICIT: define void @testSSExt
void testSSExt() {
  // EXPLICIT:      load
  // EXPLICIT-NEXT: sext
  // EXPLICIT-NEXT: store
  // EXPLICIT-NOT:  call
  //
  // IMPLICIT-NOT:  call
  int_s = (signed int)char_s;
}

// EXPLICIT: define void @testUUTrunc
// IMPLICIT: define void @testUUTrunc
void testUUTrunc() {
  // EXPLICIT:      [[T1:%.*]] = load i32
  // EXPLICIT-NEXT: [[T2:%.*]] = icmp ugt i32 [[T1]], 255
  // EXPLICIT-NOT:  icmp
  // EXPLICIT:      call
  //
  // IMPLICIT-NOT:  call
  char_u = (unsigned char)int_u;
}

// EXPLICIT: define void @testUSTrunc
// IMPLICIT: define void @testUSTrunc
void testUSTrunc() {
  // EXPLICIT:     [[T1:%.*]] = load i32
  // EXPLICIT:     [[T2:%.*]] = icmp slt i32 [[T1]], 0
  // EXPLICIT:     [[T3:%.*]] = icmp sgt i32 [[T1]], 255
  // EXPLICIT-NOT: icmp
  // EXPLICIT:     call
  //
  // IMPLICIT-NOT: call
  char_u = (unsigned char)int_s;
}

// EXPLICIT: define void @testSUTrunc
// IMPLICIT: define void @testSUTrunc
void testSUTrunc() {
  // EXPLICIT:      [[T1:%.*]] = load i32
  // EXPLICIT-NEXT: [[T2:%.*]] = icmp ugt i32 [[T1]], 127
  // EXPLICIT-NOT:  icmp
  // EXPLICIT:      call
  //
  // IMPLICIT-NOT:  call
  char_s = (signed char)int_u;
}

// EXPLICIT: define void @testSSTrunc
// IMPLICIT: define void @testSSTrunc
void testSSTrunc() {
  // EXPLICIT:     [[T1:%.*]] = load i32
  // EXPLICIT:     [[T2:%.*]] = icmp slt i32 [[T1]], -128
  // EXPLICIT:     [[T3:%.*]] = icmp sgt i32 [[T1]], 127
  // EXPLICIT-NOT: icmp
  // EXPLICIT:     call
  //
  // IMPLICIT-NOT: call
  char_s = (signed char)int_s;
}
