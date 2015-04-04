// Check explicit vs implicit cast handling in C++
// This is tricky because of the way implicit cast nodes are generated
// to do the work for explicit casts, which become noop's.
// Go through the various sources of integer casts here and check them.

// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - %s \
// RUN:   -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - %s \
// RUN:   -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT

extern "C" int checkImplicit(long l);
extern "C" int checkCStyle(long l);
extern "C" int checkFunction(long l);
extern "C" int checkStatic(long l);

// IMPLICIT: define i32 @checkImplicit
// EXPLICIT: define i32 @checkImplicit
int checkImplicit(long l) {
  // IMPLICIT: call void @__ioc_report_conversion
  // EXPLICIT-NOT: call
  return l;
}

// IMPLICIT: define i32 @checkCStyle
// EXPLICIT: define i32 @checkCStyle
int checkCStyle(long l) {
  // IMPLICIT-NOT: call
  // EXPLICIT: call void @__ioc_report_conversion
  return (int)l;
}

// IMPLICIT: define i32 @checkFunction
// EXPLICIT: define i32 @checkFunction
int checkFunction(long l) {
  // IMPLICIT-NOT: call
  // EXPLICIT: call void @__ioc_report_conversion
  return int(l);
}

// IMPLICIT: define i32 @checkStatic
// EXPLICIT: define i32 @checkStatic
int checkStatic(long l) {
  // IMPLICIT-NOT: call
  // EXPLICIT: call void @__ioc_report_conversion
  return static_cast<int>(l);
}
