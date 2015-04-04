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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "ioc-helpers.h"
#include <stdio.h>

#define __OUTPUT_XML__

#define __OUTPUT_XML__


/* FIXME needs sanity checks */
void setFalseIOC(int *array, int pos)
{
    array[pos] = 0;
    return;
}

void setTrueIOC(int *array, int pos)
{
    if (array[pos] == 0)
        array[pos] = 1;
    return;
}

int checkIOC(int *array, int size)
{
    int terminate = 0;
    int i;

    for (i = 0; i < size; i++)
        terminate |= array[i];

    printf("size is %d,  result is %d\n", size, terminate);
    //	if (terminate)
    //		exit(1);
}

char* parseFName(char* fname) {

    char* s;
    int i;

    if (!fname)
        return NULL;

    for (i = strlen(fname), s = fname; i ; i--) {
        if (fname[i] == '/') {
            s = &fname[i];
            s++;
            break;
        }
    }
    return s;
}

//Returns 1 if the triple ('name', 'line', 'col') exists in file 'file'
//Ignores 'line' and 'col' if both are 0
int existsInExclude(char *file, char *name, uint32_t line, uint32_t col) {

    char line_buffer[BUFSIZ];
    char fname[BUFSIZ];
    uint32_t fline, fcol;
    uint8_t ignores_lines;
    FILE *fd;
    int i;

    fd = fopen(file, "r");
    if (!fd) {
        perror("Error opening file:");
        return 0;
    }

    while (fgets(line_buffer, sizeof(line_buffer), fd)) {
        sscanf(line_buffer, "%s %d %d", fname, &fline, &fcol);
        // just check the name if line and col are 0
        if (!(fline || fcol)) {
            if (strcmp(parseFName(name), fname) == 0){
                fclose(fd);
                return 1;
            }
        } else {
            if (strcmp(parseFName(name), fname) == 0 &&
                fline == line &&
                fcol == col){

                fclose(fd);
                return 1;
            }
        }
    }

    fclose(fd);
    return 0;
}

div_t   __ioc_div(int numerator, int denominator) {
#ifdef __OUTPUT_XML__
  if (denominator == 0) {
    const char* msg = "div: divsion-by-zero";
    char log[256];
    sprintf(log, "div: lval %d, rval %d", numerator, denominator);

    outputXML((char*) msg, (char*) "", 0, 0, log);
    //exit(-1);
  }
#else

#endif
  return div(numerator, numerator);
}

ldiv_t  __ioc_ldiv(int numerator, int denominator) {
#ifdef __OUTPUT_XML__
  if (denominator == 0) {
    const char* msg = "ldiv: divsion-by-zero";
    char log[256];
    sprintf(log, "ldiv: lval %d, rval %d", numerator, denominator);

    outputXML((char*) msg, (char*) "", 0, 0, log);
    //exit(-1);
  }
#else

#endif
  return ldiv(numerator, numerator);
}

lldiv_t __ioc_lldiv(int numerator, int denominator) {
#ifdef __OUTPUT_XML__
  if (denominator == 0) {
    const char* msg = "lldiv: divsion-by-zero";
    char log[256];
    sprintf(log, "lldiv: lval %d, rval %d", numerator, denominator);

    outputXML((char*) msg, (char*) "", 0, 0, log);
    //exit(-1);
  }
#else

#endif
  return lldiv(numerator, numerator);
}

size_t __ioc_iconv(iconv_t cd,
                   char **inbuf, size_t *inbytesleft,
                   char **outbuf, size_t *outbytesleft) {
#ifdef __OUTPUT_XML__

#else
#endif
  return iconv(cd, inbuf, inbytesleft, outbuf, outbytesleft);
}

int outputXML(char* log,
              char* fname,
              uint32_t line,
              uint32_t col,
              char* valStr) {

	//check if exclude this file from our rule set
  if (fname && existsInExclude(EXCLUDE_FNAME, fname, line, col))
	  return 1;

  const char *entry_id = NULL;
  const char *tc  = NULL;
  const char *impact = NULL;

  entry_id = getenv("ENTRY_ID");
  if (entry_id == NULL)
    entry_id = (char *) "EID_NEEDED";

  tc = getenv("TESTCASE");
  if (tc == NULL)
    tc = (char *) "TESTCASE_NEEDED";

  impact = getenv("IMPACT");
  if (impact == NULL)
    impact = (char *) "IMPACT_NEEDED";

  FILE *fp = fopen(FNAME, "w");
  if (!fp) {
    perror("Error opening file:");
    return 0;
  }
  fprintf(fp, XML_MSG, entry_id, tc, impact, tc, log,
          fname, line, col, valStr);
  fclose(fp);
  //exit(-1);
  return 1;
}

void __ioc___ioc_report_add_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_sub_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_mul_overflow(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_div_error(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_rem_error(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_shl_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_shr_bitwidth(uint32_t line, uint32_t column,
                               const char *filename, const char *exprstr,
                               uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_shl_strict(uint32_t line, uint32_t column,
                            const char *filename, const char *exprstr,
                             uint64_t lval, uint64_t rval, uint8_t T)
{
#ifdef __OUTPUT_XML__

#else
#endif

}
void __ioc___ioc_report_conversion(uint32_t line, uint32_t column,
                             const char *filename,
                             const char *srcty, const char *canonsrcty,
                             const char *dstty, const char *canondstty,
                             uint64_t src, uint8_t S)
{
#ifdef __OUTPUT_XML__

#else
#endif
}
