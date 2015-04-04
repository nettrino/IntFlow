//===-- ioc_interface.h -----------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Interface for Integer Overflow Checker (IOC)
//
//===----------------------------------------------------------------------===//

#ifndef _IOC_INTERFACE_H_
#define _IOC_INTERFACE_H_

// For now, only support linux.
// Other platforms should be easy to add,
// and probably work as-is.
#if !defined(__linux__)
#error "IOC not supported for this platform!"
#endif

#include <stdint.h>


void __ioc_report_add_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *expstr,
                               uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_sub_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *expstr,
                               uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_mul_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *expstr,
                               uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_div_error(uint32_t line, uint32_t column,
                            const char *filename, const char *expstr,
                            uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_rem_error(uint32_t line, uint32_t column,
                            const char *filename, const char *expstr,
                            uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_shl_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *expstr,
                               uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_shr_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *expstr,
                               uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_shl_strict(uint32_t line, uint32_t column,
                             const char *filename, const char *expstr,
                             uint64_t lval, uint64_t rval, uint8_t Type);
void __ioc_report_conversion(uint32_t line, uint32_t column,
                             const char *filename,
                             const char *srcty, const char *canonsrcty,
                             const char *dstty, const char *canondstty,
                             uint64_t src, uint8_t is_signed);

typedef struct
{
  int quot;           /* Quotient.  */
  int rem;            /* Remainder.  */
} div_t;

// div_t   __ioc_div(int numerator, int denominator);
// ldiv_t  __ioc_ldiv(int numerator, int denominator);
// lldiv_t __ioc_lldiv(int numerator, int denominator);
// size_t __ioc_iconv(iconv_t cd,
//                    char **inbuf, size_t *inbytesleft,
//                    char **outbuf, size_t *outbytesleft);


#endif // _IOC_INTERFACE_H_
