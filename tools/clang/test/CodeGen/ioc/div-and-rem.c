// Test div and rem checks added with -fioc-signed
//
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 %s -emit-llvm -o - \
// RUN: -fioc-signed| FileCheck %s

unsigned u, v, w;
int i, j, k;

// CHECK: define void @testdiv
void testdiv() {
  // CHECK:      [[T1:%.*]] = load i32* @j
  // CHECK-NEXT: [[T2:%.*]] = load i32* @k
  // CHECK:      icmp ne i32 [[T2]], 0
  // CHECK:      icmp ne i32 [[T1]], -2147483648
  // CHECK:      icmp ne i32 [[T2]], -1
  // CHECK:      sdiv
  i = j / k;

  // CHECK:      [[T1:%.*]] = load i32* @v
  // CHECK-NEXT: [[T2:%.*]] = load i32* @w
  // CHECK:      icmp ne i32 [[T2]], 0
  // CHECK-NOT:  icmp ne {{.*}}, -2147483648
  // CHECK-NOT:  icmp ne {{.*}}, -1
  // CHECK:      udiv
  u = v / w;
}

// CHECK: define void @testrem
void testrem() {
  // CHECK:      [[T1:%.*]] = load i32* @j
  // CHECK-NEXT: [[T2:%.*]] = load i32* @k
  // CHECK:      icmp ne i32 [[T2]], 0
  // CHECK:      icmp ne i32 [[T1]], -2147483648
  // CHECK:      icmp ne i32 [[T2]], -1
  // CHECK:      srem
  i = j % k;

  // CHECK:      [[T1:%.*]] = load i32* @v
  // CHECK-NEXT: [[T2:%.*]] = load i32* @w
  // CHECK:      icmp ne i32 [[T2]], 0
  // CHECK-NOT:  icmp ne {{.*}}, -2147483648
  // CHECK-NOT:  icmp ne {{.*}}, -1
  // CHECK:      urem
  u = v % w;
}
