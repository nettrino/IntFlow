// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -fioc-signed %s -emit-llvm -o - | FileCheck %s
// Verify -fioc-signed behavior for +,-,*, and ++ for various datatypes.
//
// Post/preincrement on types smaller than int don't get instrumented,
// as they cannot overflow due to promotion.

long li, lj, lk;
int ii, ij, ik;
short si, sj, sk;
char ci, cj, ck;

extern void opaquelong(long);
extern void opaqueint(int);
extern void opaqueshort(short);
extern void opaquechar(char);

// CHECK: define void @testlongadd()
void testlongadd() {

  // CHECK:      [[T1:%.*]] = load i64* @lj
  // CHECK-NEXT: [[T2:%.*]] = load i64* @lk
  // CHECK-NEXT: [[T3:%.*]] = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 [[T1]], i64 [[T2]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i64, i1 } [[T3]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i64, i1 } [[T3]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
  li = lj + lk;
}

// CHECK: define void @testlongsub()
void testlongsub() {

  // CHECK:      [[T1:%.*]] = load i64* @lj
  // CHECK-NEXT: [[T2:%.*]] = load i64* @lk
  // CHECK-NEXT: [[T3:%.*]] = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 [[T1]], i64 [[T2]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i64, i1 } [[T3]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i64, i1 } [[T3]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_sub_overflow
  li = lj - lk;
}

// CHECK: define void @testlongmul()
void testlongmul() {

  // CHECK:      [[T1:%.*]] = load i64* @lj
  // CHECK-NEXT: [[T2:%.*]] = load i64* @lk
  // CHECK-NEXT: [[T3:%.*]] = call { i64, i1 } @llvm.smul.with.overflow.i64(i64 [[T1]], i64 [[T2]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i64, i1 } [[T3]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i64, i1 } [[T3]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_mul_overflow
  li = lj * lk;
}

// CHECK: define void @testlongpostinc()
void testlongpostinc() {
  opaquelong(li++);

  // CHECK:      [[T1:%.*]] = load i64* @li
  // CHECK-NEXT: [[T2:%.*]] = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 [[T1]], i64 1)
  // CHECK-NEXT: [[T3:%.*]] = extractvalue { i64, i1 } [[T2]], 0
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i64, i1 } [[T2]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
}

// CHECK: define void @testlongpreinc()
void testlongpreinc() {
  opaquelong(++li);

  // CHECK:      [[T1:%.*]] = load i64* @li
  // CHECK-NEXT: [[T2:%.*]] = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 [[T1]], i64 1)
  // CHECK-NEXT: [[T3:%.*]] = extractvalue { i64, i1 } [[T2]], 0
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i64, i1 } [[T2]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
}

// CHECK: define void @testintadd()
void testintadd() {

  // CHECK:      [[T1:%.*]] = load i32* @ij
  // CHECK-NEXT: [[T2:%.*]] = load i32* @ik
  // CHECK-NEXT: [[T3:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T1]], i32 [[T2]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T3]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T3]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
  ii = ij + ik;
}

// CHECK: define void @testintsub()
void testintsub() {

  // CHECK:      [[T1:%.*]] = load i32* @ij
  // CHECK-NEXT: [[T2:%.*]] = load i32* @ik
  // CHECK-NEXT: [[T3:%.*]] = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 [[T1]], i32 [[T2]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T3]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T3]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_sub_overflow
  ii = ij - ik;
}

// CHECK: define void @testintmul()
void testintmul() {

  // CHECK:      [[T1:%.*]] = load i32* @ij
  // CHECK-NEXT: [[T2:%.*]] = load i32* @ik
  // CHECK-NEXT: [[T3:%.*]] = call { i32, i1 } @llvm.smul.with.overflow.i32(i32 [[T1]], i32 [[T2]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T3]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T3]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_mul_overflow
  ii = ij * ik;
}

// CHECK: define void @testintpostinc()
void testintpostinc() {
  opaqueint(ii++);

  // CHECK:      [[T1:%.*]] = load i32* @ii
  // CHECK-NEXT: [[T2:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T1]], i32 1)
  // CHECK-NEXT: [[T3:%.*]] = extractvalue { i32, i1 } [[T2]], 0
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T2]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
}

// CHECK: define void @testintpreinc()
void testintpreinc() {
  opaqueint(++ii);

  // CHECK:      [[T1:%.*]] = load i32* @ii
  // CHECK-NEXT: [[T2:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T1]], i32 1)
  // CHECK-NEXT: [[T3:%.*]] = extractvalue { i32, i1 } [[T2]], 0
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T2]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
}

// CHECK: define void @testshortadd()
void testshortadd() {

  // CHECK:      load i16* @sj
  // CHECK:      load i16* @sk
  // CHECK:      [[T1:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T2:%.*]], i32 [[T3:%.*]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T1]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T1]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
  si = sj + sk;
}

// CHECK: define void @testshortsub()
void testshortsub() {

  // CHECK:      load i16* @sj
  // CHECK:      load i16* @sk
  // CHECK:      [[T1:%.*]] = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 [[T2:%.*]], i32 [[T3:%.*]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T1]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T1]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_sub_overflow
  si = sj - sk;
}

// CHECK: define void @testshortmul()
void testshortmul() {

  // CHECK:      load i16* @sj
  // CHECK:      load i16* @sk
  // CHECK:      [[T1:%.*]] = call { i32, i1 } @llvm.smul.with.overflow.i32(i32 [[T2:%.*]], i32 [[T3:%.*]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T1]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T1]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_mul_overflow
  si = sj * sk;
}

// CHECK: define void @testshortpostinc()
void testshortpostinc() {
  opaqueshort(si++);

  // CHECK:      [[T1:%.*]] = load i16* @si
  // CHECK-NEXT: [[T2:%.*]] = add i16 [[T1]], 1
  // CHECK-NOT:  llvm.sadd
  // CHECK-NOT:  llvm.uadd
  // CHECK:      ret void
}

// CHECK: define void @testshortpreinc()
void testshortpreinc() {
  opaqueshort(++si);

  // CHECK:      [[T1:%.*]] = load i16* @si
  // CHECK-NEXT: [[T2:%.*]] = add i16 [[T1]], 1
  // CHECK-NOT:  llvm.sadd
  // CHECK-NOT:  llvm.uadd
  // CHECK:      ret void
}

// CHECK: define void @testcharadd()
void testcharadd() {

  // CHECK:      load i8* @cj
  // CHECK:      load i8* @ck
  // CHECK:      [[T1:%.*]] = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 [[T2:%.*]], i32 [[T3:%.*]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T1]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T1]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_add_overflow
  ci = cj + ck;
}

// CHECK: define void @testcharsub()
void testcharsub() {

  // CHECK:      load i8* @cj
  // CHECK:      load i8* @ck
  // CHECK:      [[T1:%.*]] = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 [[T2:%.*]], i32 [[T3:%.*]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T1]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T1]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_sub_overflow
  ci = cj - ck;
}

// CHECK: define void @testcharmul()
void testcharmul() {

  // CHECK:      load i8* @cj
  // CHECK:      load i8* @ck
  // CHECK:      [[T1:%.*]] = call { i32, i1 } @llvm.smul.with.overflow.i32(i32 [[T2:%.*]], i32 [[T3:%.*]])
  // CHECK-NEXT: [[T4:%.*]] = extractvalue { i32, i1 } [[T1]], 0
  // CHECK-NEXT: [[T5:%.*]] = extractvalue { i32, i1 } [[T1]], 1
  // CHECK:      {{^.*}}:
  // CHECK:      call void @__ioc_report_mul_overflow
  ci = cj * ck;
}

// CHECK: define void @testcharpostinc()
void testcharpostinc() {
  opaquechar(ci++);

  // CHECK:      [[T1:%.*]] = load i8* @ci
  // CHECK-NEXT: [[T2:%.*]] = add i8 [[T1]], 1
  // CHECK-NOT:  llvm.sadd
  // CHECK-NOT:  llvm.uadd
  // CHECK:      ret void
}

// CHECK: define void @testcharpreinc()
void testcharpreinc() {
  opaquechar(++ci);

  // CHECK:      [[T1:%.*]] = load i8* @ci
  // CHECK-NEXT: [[T2:%.*]] = add i8 [[T1]], 1
  // CHECK-NOT:  llvm.sadd
  // CHECK-NOT:  llvm.uadd
  // CHECK:      ret void
}
