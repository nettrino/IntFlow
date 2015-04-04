//===-- ioc_report.c ------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Error logging entry points for the IOC runtime.
//
//===----------------------------------------------------------------------===//

#include "ioc_interface.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

//#define __OUTPUT_XML__

#if 0
#ifdef __OUTPUT_XML__
int outputXML(char* log,
              char* fname,
              uint32_t line,
              uint32_t col,
              char* valStr);
#endif
#endif
// Shared helper for reporting failed checks
void __ioc_report_error(uint32_t line, uint32_t column,
                        const char *filename, const char *exprstr,
                        uint64_t lval, uint8_t LType,
                        uint64_t rval, uint8_t RType,
                        const char *msg);

// Helpers to parse the encoded integer type
void __ioc_print_val(char *output, uint64_t V, uint8_t T);
char __ioc_is_signed(uint8_t T);

// Forward each entry point to the shared helper:
void __ioc_report_add_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                     __ioc_is_signed(T) ? "signed addition overflow" :
                                          "unsigned addition overflow");
}

void __ioc_report_sub_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                     __ioc_is_signed(T) ? "signed subtraction overflow" :
                                          "unsigned subtraction overflow");
}

void __ioc_report_mul_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                     __ioc_is_signed(T) ? "signed multiplication overflow" :
                                          "unsigned multiplication overflow");
}

void __ioc_report_div_error(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  if (rval == 0)
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "division by zero is undefined");
  else
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "division overflow (INT_MIN / -1)");
}

void __ioc_report_rem_error(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  if (rval == 0)
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "remainder by zero is undefined");
  else
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "remainder overflow (INT_MIN % -1)");
}

void __ioc_report_shl_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  // For shifts, we use a single check for efficiency.
  // Depending on sign of the operand, give a more specific error message:
  if (__ioc_is_signed(T) && (int64_t)rval < 0)
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "left shift by negative amount");
  else
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "left shift by amount >= bitwidth");
}

void __ioc_report_shr_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T) {
  // For shifts, we use a single check for efficiency.
  // Depending on sign of the operand, give a more specific error message:
  if (__ioc_is_signed(T) && (int64_t)rval < 0)
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "right shift by negative amount");
  else
    __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                       "right shift by amount >= bitwidth");
}

void __ioc_report_shl_strict(uint32_t line, uint32_t column,
                             const char *filename, const char *exprstr,
                             uint64_t lval, uint64_t rval, uint8_t T) {
  __ioc_report_error(line, column, filename, exprstr, lval, T, rval, T,
                     "left shift into or beyond sign bit");
}

void __ioc_report_conversion(uint32_t line, uint32_t column,
                             const char *filename,
                             const char *srcty, const char *canonsrcty,
                             const char *dstty, const char *canondstty,
                             uint64_t src, uint8_t S) {
  char srcstr[100];
  if (S)
    sprintf(srcstr, "%lld", (signed long long)src);
  else
    sprintf(srcstr, "%llu", (unsigned long long)src);

#if 0
#ifdef __OUTPUT_XML__
  char log[256];
  sprintf(log, "conversion error from %s (%s) to %s (%s)",
          srcty, canonsrcty, dstty, canondstty);
  
  outputXML(log, (char*) filename, line, column, srcstr);
#else
  fprintf(stderr, "%s:%d:%d: runtime error occured: value lost in conversion of '%s'"
                  " from '%s' (%s) to '%s' (%s)\n",
                  filename, line, column, srcstr,
                  srcty, canonsrcty, dstty, canondstty);
#endif
#endif
}

// Handling of encoded types:
char __ioc_is_signed(uint8_t T) {
  return (T & 8) != 0;
}

// Print helper for operand values:
void __ioc_print_val(char *output, uint64_t V, uint8_t T) {
  unsigned width = (1 << (T & 7));
  if (__ioc_is_signed(T))
    sprintf(output, "(sint%u) %lld", width, (signed long long)V);
  else
    sprintf(output, "(uint%u) %llu", width, (unsigned long long)V);
}

void __ioc_report_error(uint32_t line, uint32_t column,
                        const char *filename, const char *exprstr,
                        uint64_t lval, uint8_t LT, uint64_t rval, uint8_t RT,
                        const char *msg) {
  // Convert operands to strings:
  char lstr[100], rstr[100];
  __ioc_print_val(lstr, lval, LT);
  __ioc_print_val(rstr, rval, RT);

#if 0
#ifdef __OUTPUT_XML__
  char log[256];
  sprintf(log,"[ expr = '%s', lval = %s, rval = %s ]", exprstr, lstr, rstr);
  outputXML((char*) msg, (char*) filename, line, column, log);
#else
#endif

#if 0
  fprintf(stderr, "%s:%d:%d: runtime error occured: %s "
                  "[ expr = '%s', lval = %s, rval = %s ]\n",
                  filename, line, column, msg,
                  exprstr, lstr, rstr);
#endif

#endif
}



/* Returns: a * b */

/* Effects: sets *overflow to 1  if a * b overflows */

typedef long long di_int;

di_int
__mulodi4(di_int a, di_int b, int* overflow)
{
  const int N = (int)(sizeof(di_int) * CHAR_BIT);
  const di_int MIN = (di_int)1 << (N-1);
  const di_int MAX = ~MIN;
  *overflow = 0;
  di_int result = a * b;
  if (a == MIN)
    {
      if (b != 0 && b != 1)
        *overflow = 1;
      return result;
    }
  if (b == MIN)
    {
      if (a != 0 && a != 1)
        *overflow = 1;
      return result;
    }
  di_int sa = a >> (N - 1);
  di_int abs_a = (a ^ sa) - sa;
  di_int sb = b >> (N - 1);
  di_int abs_b = (b ^ sb) - sb;
  if (abs_a < 2 || abs_b < 2)
    return result;
  if (sa == sb)
    {
      if (abs_a > MAX / abs_b)
        *overflow = 1;
    }
  else
    {
      if (abs_a > MIN / -abs_b)
        *overflow = 1;
    }
  return result;
}
