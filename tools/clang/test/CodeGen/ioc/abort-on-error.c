// Verify that -fioc-abort-on-error works as expected.
//
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 %s -emit-llvm -o - \
// RUN: -fioc-signed | FileCheck %s --check-prefix=NORMAL
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 %s -emit-llvm -o - \
// RUN: -fioc-signed -fioc-abort-on-error | FileCheck %s --check-prefix=ABORT

int i, j;

// NORMAL: define i32 @test()
// ABORT: define i32 @test()
int test() {
  // NORMAL: [[T1:%.*]] = load i32* @i
  // NORMAL: [[T2:%.*]] = load i32* @j
  // NORMAL: [[T3:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T1]], i32 [[T2]])
  // NORMAL-NOT: @llvm.trap
  // NORMAL-NOT: unreachable
  // ABORT: [[T1:%.*]] = load i32* @i
  // ABORT: [[T2:%.*]] = load i32* @j
  // ABORT: [[T3:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T1]], i32 [[T2]])
  // ABORT: @llvm.trap
  // ABORT: unreachable
  return i + j;
}
