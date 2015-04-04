/*
 * Copyright (c) 2014, Columbia University
 * All rights reserved.
 *
 * This software was developed by (alphabetically)
 * Kangkook Jee <jikk@cs.columbia.edu>
 * Theofilos Petsios <theofilos@cs.columbia.edu>
 * Marios Pomonis <mpomonis@cs.columbia.edu>
 * at Columbia University, New York, NY, USA, in September 2014.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Columbia University nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include <iconv.h>
#include <stdint.h>

div_t   __ioc_div(int numerator, int denominator);
ldiv_t  __ioc_ldiv(int numerator, int denominator);
lldiv_t __ioc_lldiv(int numerator, int denominator);
size_t __ioc_iconv(iconv_t cd,
                   char **inbuf, size_t *inbytesleft,
                   char **outbuf, size_t *outbytesleft);

#define XML_MSG                                     \
  "<structured_message>\n"                          \
  "<message_type>found_cwe</message_type>\n"        \
  "<cwe_entry_id>%s</cwe_entry_id>\n"               \
  "</structured_message>\n"                         \
  "<structured_message>\n"                          \
  "<message_type>controlled_exit</message_type>\n"  \
  "<test_case>%s</test_case>\n"                     \
  "</structured_message>\n"                         \
  "<structured_message>\n"                          \
  "<message_type>technical_impact</message_type>\n" \
  "<impact>%s</impact>\n"                           \
  "<test_case>%s</test_case>\n"                     \
  "</structured_message>\n"                         \
  "<!-- error class: %s   -->\n"                    \
  "<!-- file: %s   -->\n"                           \
  "<!-- line: %d   -->\n"                           \
  "<!-- column: %d   -->\n"                         \
  "<!-- value string: %s -->\n"

#define FNAME "/tmp/log.txt"
#define EXCLUDE_FNAME "/tmp/exclude.files"


int checkIOC(int *array, int size);
void setFalseIOC(int *array, int pos);
void setTrueIOC(int *array, int pos);

void __ioc____ioc_report_add_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_sub_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_mul_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_div_error(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_rem_error(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_shl_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_shr_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_shl_strict(uint32_t line, uint32_t column,
                             const char *filename, const char *exprstr,
                             uint64_t lval, uint64_t rval, uint8_t T);
void __ioc___ioc_report_conversion(uint32_t line, uint32_t column,
                             const char *filename,
                             const char *srcty, const char *canonsrcty,
                             const char *dstty, const char *canondstty,
                             uint64_t src, uint8_t S);
