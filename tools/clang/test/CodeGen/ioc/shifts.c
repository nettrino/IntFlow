// RUN: %clang_cc1 -triple x86_64-apple-darwin10 %s -emit-llvm -o - \
// RUN:   -fioc-shifts | FileCheck %s --check-prefix=SHIFT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 %s -emit-llvm -o - \
// RUN:   -fioc-strict-shifts | FileCheck %s --check-prefix=STRICT

unsigned int u1, u2, u3;
  signed int s1, s2, s3;

// SHIFT: define void @testSShl()
// STRICT: define void @testSShl()
void testSShl() {
  // SHIFT: [[T1:%.*]] = load i32* @s2
  // SHIFT: [[T2:%.*]] = load i32* @s3
  // SHIFT: icmp ule i32 [[T2]], 31
  // SHIFT: call void @__ioc_report_shl_bitwidth
  // STRICT: [[T1:%.*]] = load i32* @s2
  // STRICT: [[T2:%.*]] = load i32* @s3
  // STRICT: icmp ule i32 [[T2]], 31
  // STRICT: call void @__ioc_report_shl_bitwidth
  // STRICT: call void @__ioc_report_shl_strict
  s1 = s2 << s3;
}

// SHIFT: define void @testSShr()
// STRICT: define void @testSShr()
void testSShr() {
  // SHIFT:     [[T1:%.*]] = load i32* @s2
  // SHIFT:     [[T2:%.*]] = load i32* @s3
  // SHIFT:     icmp ult i32 [[T2]], 32
  // SHIFT:     call void @__ioc_report_shr_bitwidth
  // SHIFT-NOT: call void @__ioc_report_shr_strict
  // STRICT:     [[T1:%.*]] = load i32* @s2
  // STRICT:     [[T2:%.*]] = load i32* @s3
  // STRICT:     icmp ult i32 [[T2]], 32
  // STRICT:     call void @__ioc_report_shr_bitwidth
  // STRICT-NOT: call void @__ioc_report_shr_strict
  s1 = s2 >> s3;
}

// SHIFT: define void @testUShl()
// STRICT: define void @testUShl()
void testUShl() {
  // SHIFT:     [[T1:%.*]] = load i32* @u2
  // SHIFT:     [[T2:%.*]] = load i32* @u3
  // SHIFT:     icmp ule i32 [[T2]], 31
  // SHIFT:     call void @__ioc_report_shl_bitwidth
  // SHIFT-NOT: call void @__ioc_report_shl_strict
  // STRICT:     [[T1:%.*]] = load i32* @u2
  // STRICT:     [[T2:%.*]] = load i32* @u3
  // STRICT:     icmp ule i32 [[T2]], 31
  // STRICT:     call void @__ioc_report_shl_bitwidth
  // STRICT-NOT: call void @__ioc_report_shl_strict
  u1 = u2 << u3;
}

// SHIFT: define void @testUShr()
// STRICT: define void @testUShr()
void testUShr() {
  // SHIFT:     [[T1:%.*]] = load i32* @u2
  // SHIFT:     [[T2:%.*]] = load i32* @u3
  // SHIFT:     icmp ult i32 [[T2]], 32
  // SHIFT:     call void @__ioc_report_shr_bitwidth
  // SHIFT-NOT: call void @__ioc_report_shr_strict
  // STRICT:     [[T1:%.*]] = load i32* @u2
  // STRICT:     [[T2:%.*]] = load i32* @u3
  // STRICT:     icmp ult i32 [[T2]], 32
  // STRICT:     call void @__ioc_report_shr_bitwidth
  // STRICT-NOT: call void @__ioc_report_shr_strict
  u1 = u2 >> u3;
}
