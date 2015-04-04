// Check post/pre inc/dec on shorts, exercising the conversion-checking logic.
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - %s \
// RUN:   -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - -xc++ %s \
// RUN:   -fioc-implicit-conversion | FileCheck %s --check-prefix=IMPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - %s \
// RUN:   -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT
// RUN: %clang_cc1 -triple x86_64-apple-darwin10 -emit-llvm -o - -xc++ %s \
// RUN:   -fioc-explicit-conversion | FileCheck %s --check-prefix=EXPLICIT

short ss;
unsigned short us;
extern void opaqueshort(short);
extern void opaqueushort(unsigned short);

// Don't mangle function names when building as C++
#ifdef __cplusplus
extern "C" void testsignedshortpostinc();
extern "C" void testsignedshortpreinc();
extern "C" void testsignedshortpostdec();
extern "C" void testsignedshortpredec();
extern "C" void testunsignedshortpostinc();
extern "C" void testunsignedshortpreinc();
extern "C" void testunsignedshortpostdec();
extern "C" void testunsignedshortpredec();
#endif // __cplusplus

// IMPLICIT: define void @testsignedshortpostinc()
// EXPLICIT: define void @testsignedshortpostinc()
void testsignedshortpostinc() {
  opaqueshort(ss++);
  // IMPLICIT:      [[T1:%.*]] = load i16* @ss
  // IMPLICIT-NEXT: llvm.sadd.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testsignedshortpreinc()
// EXPLICIT: define void @testsignedshortpreinc()
void testsignedshortpreinc() {
  opaqueshort(++ss);

  // IMPLICIT:      [[T1:%.*]] = load i16* @ss
  // IMPLICIT-NEXT: llvm.sadd.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testsignedshortpostdec()
// EXPLICIT: define void @testsignedshortpostdec()
void testsignedshortpostdec() {
  opaqueshort(ss--);
  // IMPLICIT:      [[T1:%.*]] = load i16* @ss
  // IMPLICIT-NEXT: llvm.ssub.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testsignedshortpredec()
// EXPLICIT: define void @testsignedshortpredec()
void testsignedshortpredec() {
  opaqueshort(--ss);

  // IMPLICIT:      [[T1:%.*]] = load i16* @ss
  // IMPLICIT-NEXT: llvm.ssub.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testunsignedshortpostinc()
// EXPLICIT: define void @testunsignedshortpostinc()
void testunsignedshortpostinc() {
  opaqueshort(us++);
  // IMPLICIT:      [[T1:%.*]] = load i16* @us
  // IMPLICIT-NEXT: llvm.uadd.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testunsignedshortpreinc()
// EXPLICIT: define void @testunsignedshortpreinc()
void testunsignedshortpreinc() {
  opaqueshort(++us);

  // IMPLICIT:      [[T1:%.*]] = load i16* @us
  // IMPLICIT-NEXT: llvm.uadd.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testunsignedshortpostdec()
// EXPLICIT: define void @testunsignedshortpostdec()
void testunsignedshortpostdec() {
  opaqueushort(us--);
  // IMPLICIT:      [[T1:%.*]] = load i16* @us
  // IMPLICIT-NEXT: llvm.usub.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}

// IMPLICIT: define void @testunsignedshortpredec()
// EXPLICIT: define void @testunsignedshortpredec()
void testunsignedshortpredec() {
  opaqueshort(--us);

  // IMPLICIT:      [[T1:%.*]] = load i16* @us
  // IMPLICIT-NEXT: llvm.usub.with.overflow.i16(i16 [[T1]]
  // IMPLICIT:      ret void
  //
  // EXPLICIT-NOT: overflow
}
