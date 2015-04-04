// Check implicit conversions in post/preincrement operations.
// RUN: %clang_cc1 -emit-llvm -o - -xc %s -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -emit-llvm -o - -xc++ %s -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -emit-llvm -o - -xc %s -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT
// RUN: %clang_cc1 -emit-llvm -o - -xc++ %s -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT


unsigned int i;
unsigned char c;

#ifdef __cplusplus
extern "C" void testIntInc();
extern "C" void testIntDec();
extern "C" void testCharInc();
extern "C" void testCharDec();
#endif

// IMPLICIT: define void @testIntInc
// EXPLICIT: define void @testIntInc
void testIntInc() {
  // IMPLICIT-NOT: call void @__ioc_report
  // EXPLICIT-NOT: call void @__ioc_report
  ++i;
  i++;
}


// IMPLICIT: define void @testIntDec
// EXPLICIT: define void @testIntDec
void testIntDec() {
  // IMPLICIT-NOT: call void @__ioc_report
  // EXPLICIT_NOT: call void @__ioc_report
  --i;
  i--;
}

// IMPLICIT: define void @testCharInc
// EXPLICIT: define void @testCharInc
void testCharInc() {
  // IMPLICIT: call void @__ioc_report
  // IMPLICIT: call void @__ioc_report
  // EXPLICIT-NOT: call void @__ioc_report
  ++c;
  c++;
}

// IMPLICIT: define void @testCharDec
// EXPLICIT: define void @testCharDec
void testCharDec() {
  // IMPLICIT: call void @__ioc_report
  // IMPLICIT: call void @__ioc_report
  // EXPLICIT-NOT: call void @__ioc_report
  --c;
  c--;
}
