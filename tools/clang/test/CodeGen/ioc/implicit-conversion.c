// Check implicit conversions across sign and bitwidth differences.
// Explicit test prefix only verifies instrumentation is not added.
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
extern "C" void testUUSame();
extern "C" void testUSSame();
extern "C" void testSUSame();
extern "C" void testSSSame();
#endif // __cplusplus

// IMPLICIT: define void @testUUExt
// EXPLICIT: define void @testUUExt
void testUUExt() {
  // IMPLICIT:      load
  // IMPLICIT-NEXT: zext
  // IMPLICIT-NEXT: store
  // IMPLICIT-NOT:  call
  //
  // EXPLICIT-NOT:  call
  int_u = char_u;
}

// IMPLICIT: define void @testUSExt
// EXPLICIT: define void @testUSExt
void testUSExt() {
  // IMPLICIT:      [[T1:%.*]] = load i8
  // IMPLICIT-NEXT: [[T2:%.*]] = icmp slt i8 [[T1]], 0
  // IMPLICIT-NOT:  icmp
  // IMPLICIT:      call
  //
  // EXPLICIT-NOT:  call
  int_u = char_s;
}

// IMPLICIT: define void @testSUExt
// EXPLICIT: define void @testSUExt
void testSUExt() {
  // IMPLICIT:      load
  // IMPLICIT-NEXT: zext
  // IMPLICIT-NEXT: store
  // IMPLICIT-NOT:  call
  //
  // EXPLICIT-NOT:  call
  int_s = char_u;
}

// IMPLICIT: define void @testSSExt
// EXPLICIT: define void @testSSExt
void testSSExt() {
  // IMPLICIT:      load
  // IMPLICIT-NEXT: sext
  // IMPLICIT-NEXT: store
  // IMPLICIT-NOT:  call
  //
  // EXPLICIT-NOT:  call
  int_s = char_s;
}

// IMPLICIT: define void @testUUTrunc
// EXPLICIT: define void @testUUTrunc
void testUUTrunc() {
  // IMPLICIT:      [[T1:%.*]] = load i32
  // IMPLICIT-NEXT: [[T2:%.*]] = icmp ugt i32 [[T1]], 255
  // IMPLICIT-NOT:  icmp
  // IMPLICIT:      call
  //
  // EXPLICIT-NOT:  call
  char_u = int_u;
}

// IMPLICIT: define void @testUSTrunc
// EXPLICIT: define void @testUSTrunc
void testUSTrunc() {
  // IMPLICIT:     [[T1:%.*]] = load i32
  // IMPLICIT:     [[T2:%.*]] = icmp slt i32 [[T1]], 0
  // IMPLICIT:     [[T3:%.*]] = icmp sgt i32 [[T1]], 255
  // IMPLICIT-NOT: icmp
  // IMPLICIT:     call
  //
  // EXPLICIT-NOT: call
  char_u = int_s;
}

// IMPLICIT: define void @testSUTrunc
// EXPLICIT: define void @testSUTrunc
void testSUTrunc() {
  // IMPLICIT:      [[T1:%.*]] = load i32
  // IMPLICIT-NEXT: [[T2:%.*]] = icmp ugt i32 [[T1]], 127
  // IMPLICIT-NOT:  icmp
  // IMPLICIT:      call
  //
  // EXPLICIT-NOT:  call
  char_s = int_u;
}

// IMPLICIT: define void @testSSTrunc
// EXPLICIT: define void @testSSTrunc
void testSSTrunc() {
  // IMPLICIT:     [[T1:%.*]] = load i32
  // IMPLICIT:     [[T2:%.*]] = icmp slt i32 [[T1]], -128
  // IMPLICIT:     [[T3:%.*]] = icmp sgt i32 [[T1]], 127
  // IMPLICIT-NOT: icmp
  // IMPLICIT:     call
  //
  // EXPLICIT-NOT: call
  char_s = int_s;
}

// IMPLICIT: define void @testUUSame
// EXPLICIT: define void @testUUSame
void testUUSame() {
  // IMPLICIT-NOT:  call
  //
  // EXPLICIT-NOT:  call
  int_u = int_u;
}

// IMPLICIT: define void @testUSSame
// EXPLICIT: define void @testUSSame
void testUSSame() {
  // IMPLICIT:     [[T1:%.*]] = load i32
  // IMPLICIT:     [[T2:%.*]] = icmp slt i32 [[T1]], 0
  // IMPLICIT-NOT: icmp
  // IMPLICIT:     call
  //
  // EXPLICIT-NOT: call
  int_u = int_s;
}

// IMPLICIT: define void @testSUSame
// EXPLICIT: define void @testSUSame
void testSUSame() {
  // IMPLICIT:      [[T1:%.*]] = load i32
  // IMPLICIT-NEXT: [[T2:%.*]] = icmp ugt i32 [[T1]], 2147483647
  // IMPLICIT-NOT:  icmp
  // IMPLICIT:      call
  //
  // EXPLICIT-NOT:  call
  int_s = int_u;
}

// IMPLICIT: define void @testSSSame
// EXPLICIT: define void @testSSSame
void testSSSame() {
  // IMPLICIT-NOT: call
  //
  // EXPLICIT-NOT: call
  int_s = int_s;
}
