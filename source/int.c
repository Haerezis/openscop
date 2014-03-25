
    /*+-----------------------------------------------------------------**
     **                       OpenScop Library                          **
     **-----------------------------------------------------------------**
     **                             int.c                               **
     **-----------------------------------------------------------------**
     **                   First version: 18/07/2011                     **
     **-----------------------------------------------------------------**

 
 *****************************************************************************
 * OpenScop: Structures and formats for polyhedral tools to talk together    *
 *****************************************************************************
 *    ,___,,_,__,,__,,__,,__,,_,__,,_,__,,__,,___,_,__,,_,__,                *
 *    /   / /  //  //  //  // /   / /  //  //   / /  // /  /|,_,             *
 *   /   / /  //  //  //  // /   / /  //  //   / /  // /  / / /\             *
 *  |~~~|~|~~~|~~~|~~~|~~~|~|~~~|~|~~~|~~~|~~~|~|~~~|~|~~~|/_/  \            *
 *  | G |C| P | = | L | P |=| = |C| = | = | = |=| = |=| C |\  \ /\           *
 *  | R |l| o | = | e | l |=| = |a| = | = | = |=| = |=| L | \# \ /\          *
 *  | A |a| l | = | t | u |=| = |n| = | = | = |=| = |=| o | |\# \  \         *
 *  | P |n| l | = | s | t |=| = |d| = | = | = | |   |=| o | | \# \  \        *
 *  | H | | y |   | e | o | | = |l|   |   | = | |   | | G | |  \  \  \       *
 *  | I | |   |   | e |   | |   | |   |   |   | |   | |   | |   \  \  \      *
 *  | T | |   |   |   |   | |   | |   |   |   | |   | |   | |    \  \  \     *
 *  | E | |   |   |   |   | |   | |   |   |   | |   | |   | |     \  \  \    *
 *  | * |*| * | * | * | * |*| * |*| * | * | * |*| * |*| * | /      \* \  \   *
 *  | O |p| e | n | S | c |o| p |-| L | i | b |r| a |r| y |/        \  \ /   *
 *  '---'-'---'---'---'---'-'---'-'---'---'---'-'---'-'---'          '--'    *
 *                                                                           *
 * Copyright (C) 2008 University Paris-Sud 11 and INRIA                      *
 *                                                                           *
 * (3-clause BSD license)                                                    *
 * Redistribution and use in source  and binary forms, with or without       *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 * 3. The name of the author may not be used to endorse or promote products  *
 *    derived from this software without specific prior written permission.  *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR      *
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES *
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.   *
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,          *
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT  *
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT       *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF  *
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.         *
 *                                                                           *
 * OpenScop Library, a library to manipulate OpenScop formats and data       *
 * structures. Written by:                                                   *
 * Cedric Bastoul     <Cedric.Bastoul@u-psud.fr> and                         *
 * Louis-Noel Pouchet <Louis-Noel.pouchet@inria.fr>                          *
 *                                                                           *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#ifdef OSL_GMP_IS_HERE
# include <gmp.h>
#endif

#include <osl/macros.h>
#include <osl/int.h>


/*+***************************************************************************
 *                                Basic Functions                            *
 *****************************************************************************/


/**
 * osl_int_is_precision_supported function:
 * this function returns 1 if the precision provided as parameter is supported
 * by the library and 0 otherwise. Possible values for the precision
 * parameter are OSL_PRECISION_SP for 32 bits (single) precision,
 * OSL_PRECISION_DP for 64 bits (double) precision and OSL_PRECISION_MP for
 * multiple precision.
 * \param[in] precision The precision to check for.
 * \return 1 if the precision is supported, 0 otherwise.
 */
int osl_int_is_precision_supported(int precision) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return 1;
    case OSL_PRECISION_DP:
      return 1;
#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return 1;
#endif
  }

  return 0;
}


/**
 * osl_int_dump_precision function:
 * this function prints in a human readable fashion the precision
 * corresponding to the "precision" parameter.
 * \param[in] file      The file where to print the precision.
 * \param[in] precision The precision to print.
 */
void osl_int_dump_precision(FILE * file, int precision) {
  switch (precision) {
    case OSL_PRECISION_SP:
      fprintf(file, "32 bits");
      break;
    case OSL_PRECISION_DP:
      fprintf(file, "64 bits");
      break;
#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      fprintf(file, "GMP");
      break;
#endif
    default:
      fprintf(file, "unknown precision %d", precision);
  }
}


void osl_int_init(int precision, osl_int_const_p variable) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = 0;
      break;

    case OSL_PRECISION_DP:
      variable->dp = 0;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      OSL_malloc(variable->mp, void*, sizeof(mpz_t)); 
      mpz_init(*variable->mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


osl_int_p osl_int_malloc(int precision) {
  osl_int_p variable;

  OSL_malloc(variable, osl_int_p, sizeof(osl_int_t));
  osl_int_init(precision, variable);
  return variable;
}


/**
 * variable = value;
 */
void osl_int_assign(int precision,
                    osl_int_const_p variable, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = value.sp;
      break;

    case OSL_PRECISION_DP:
      variable->dp = value.dp;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_set(*variable->mp, *value.mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable = i;
 */
void osl_int_set_si(int precision, osl_int_const_p variable, int i) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = (long int)i;
      break;

    case OSL_PRECISION_DP:
      variable->dp = (long long int)i;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_set_si(*variable->mp, i);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * return value;
 */
int osl_int_get_si(int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      #ifndef NDEBUG
        if (value.sp > (long int)(INT_MAX))
          OSL_overflow("osl_int_get_si overflow");
        if (value.sp < (long int)(INT_MIN))
          OSL_overflow("osl_int_get_si overflow");
      #endif
      return (int)value.sp;

    case OSL_PRECISION_DP:
      #ifndef NDEBUG
        if (value.dp > (long long int)(INT_MAX))
          OSL_overflow("osl_int_get_si overflow");
        if (value.dp < (long long int)(INT_MIN))
          OSL_overflow("osl_int_get_si overflow");
      #endif
      return (int)value.dp;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      #ifndef NDEBUG
        if (mpz_fits_sint_p(*value.mp) == 0)
          OSL_overflow("osl_int_get_si overflow");
      #endif
      return mpz_get_si(*value.mp);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable = i; // including initialization for GMP
 */
void osl_int_init_set_si(int precision, osl_int_const_p variable, int i) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = (long int)i;
      break;

    case OSL_PRECISION_DP:
      variable->dp = (long long int)i;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      OSL_malloc(variable->mp, void*, sizeof(mpz_t)); 
      mpz_init_set_si(*variable->mp, i);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * var1 <=> var2;
 */
void osl_int_swap(int precision, osl_int_const_p var1, osl_int_const_p var2) {
  switch (precision) {
    case OSL_PRECISION_SP: {
      long int temp = var1->sp;
      var1->sp = var2->sp;
      var2->sp = temp;
      break;
    }

    case OSL_PRECISION_DP: {
      long long int temp = var1->dp;
      var1->dp = var2->dp;
      var2->dp = temp;
      break;
    }

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP: {
      mpz_t temp;
      mpz_init(temp);
      mpz_set(temp, *var1->mp);
      mpz_set(*var1->mp, *var2->mp);
      mpz_set(*var2->mp, temp);
      mpz_clear(temp);
      break;
    }
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable = 0; // Including cleaning for GMP
 */
void osl_int_clear(int precision, osl_int_const_p variable) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = 0;
      break;

    case OSL_PRECISION_DP:
      variable->dp = 0;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_clear(*variable->mp);
      free(variable->mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


void osl_int_free(int precision, osl_int_const_p variable) {
  osl_int_clear(precision, variable);
  free(variable);
}


/**
 * osl_int_print function:
 * this function displays an integer value into a file (file, possibly stdout).
 * \param file      The file where the integer has to be printed.
 * \param precision The precision of the integer.
 * \param value     The integer element to print.
 */
void osl_int_print(FILE * file, int precision, osl_const_int_t value) {
  char string[OSL_MAX_STRING];
  
  osl_int_sprint(string, precision, value);
  fprintf(file, "%s", string);
}


/**
 * osl_int_sprint function:
 * this function prints an integer value into a string, it uses the
 * OpenScop Library formats OSL_FMT_* to format the printing.
 * \param string    The string where the integer has to be printed.
 * \param precision The precision of the integer.
 * \param value     The integer element to print.
 */
void osl_int_sprint(char * string, int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      sprintf(string, OSL_FMT_SP, value.sp);
      break;

    case OSL_PRECISION_DP:
      sprintf(string, OSL_FMT_DP, value.dp);
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP: {
      char * str;
      str = mpz_get_str(0, 10, *value.mp); //TODO: 10 -> #define
      sprintf(string, OSL_FMT_MP, str);
      free(str);
      break;
    }
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * osl_int_sprint_txt function:
 * this function is similar to osl_int_sprintf but it prints the value
 * using OSL_TMT_TXT_* formats.
 * \see osl_int_sprintf
 */
void osl_int_sprint_txt(char * string, int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      sprintf(string, OSL_FMT_TXT_SP, value.sp);
      break;

    case OSL_PRECISION_DP:
      sprintf(string, OSL_FMT_TXT_DP, value.dp);
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP: {
      char * str;
      str = mpz_get_str(0, 10, *value.mp); //TODO: 10 -> #define
      sprintf(string, OSL_FMT_TXT_MP, str);
      free(str);
      break;
    }
#endif

    default:
      OSL_error("unknown precision");
  }
}


void osl_int_sread(char ** string, int precision, osl_int_const_p variable) {
  int nb_read = 0;

  switch (precision) {
    case OSL_PRECISION_SP:
      nb_read = sscanf(*string, OSL_FMT_TXT_SP, &(variable->sp));
      if (nb_read == 0)
        OSL_error("failed to read an integer");
      break;

    case OSL_PRECISION_DP:
      nb_read = sscanf(*string, OSL_FMT_TXT_DP, &(variable->dp));
      if (nb_read == 0)
        OSL_error("failed to read an integer");
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP: {
      long long int tmp;
      nb_read = sscanf(*string, OSL_FMT_TXT_DP, &tmp);
      if (nb_read == 0)
        OSL_error("failed to read an integer");
      mpz_set_si(*variable->mp, tmp);
      break;
    }
#endif

    default:
      OSL_error("unknown precision");
  }

  // Update the position in the input string.
  *string = *string + nb_read;
}


/*+***************************************************************************
 *                            Arithmetic Operations                          *
 *****************************************************************************/


/**
 * variable <- value + 1;
 */
void osl_int_increment(int precision,
                       osl_int_const_p variable, osl_const_int_t value) {
  osl_int_add_si(precision, variable, value, 1);
}


/**
 * variable <- value - 1;
 */
void osl_int_decrement(int precision,
                       osl_int_const_p variable, osl_const_int_t value) {
  osl_int_add_si(precision, variable, value, -1);
}


/**
 * variable <- val1 + val2;
 */
void osl_int_add(int precision, osl_int_const_p variable,
                 osl_const_int_t val1, osl_const_int_t val2) {
  switch (precision) {
    case OSL_PRECISION_SP:
      #ifndef NDEBUG
        if (osl_int_pos(precision, val1) && osl_int_pos(precision, val2)) {
          if (LONG_MAX - val1.sp < val2.sp)
            OSL_overflow("osl_int_add overflow");
        }
        else if (osl_int_neg(precision, val1) && osl_int_neg(precision, val2)) {
          if (val1.sp - LONG_MIN < -val2.sp)
            OSL_overflow("osl_int_add overflow");
        }
      #endif
      variable->sp = val1.sp + val2.sp;
      break;

    case OSL_PRECISION_DP:
      #ifndef NDEBUG
        if (osl_int_pos(precision, val1) && osl_int_pos(precision, val2)) {
          if (LLONG_MAX - val1.dp < val2.dp)
            OSL_overflow("osl_int_add overflow");
        }
        else if (osl_int_neg(precision, val1) && osl_int_neg(precision, val2)) {
          if (val1.dp - LLONG_MIN < -val2.dp)
            OSL_overflow("osl_int_add overflow");
        }
      #endif
      variable->dp = val1.dp + val2.dp;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_add(*variable->mp, *val1.mp, *val2.mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable <- value + i;
 */
void osl_int_add_si(int precision,
                    osl_int_const_p variable, osl_const_int_t value, int i) {
  switch (precision) {
    case OSL_PRECISION_SP:
      #ifndef NDEBUG
        if (osl_int_pos(precision, value) && i > 0) {
          if (LONG_MAX - value.sp < i)
            OSL_overflow("osl_int_add_si overflow");
        }
        else if (osl_int_neg(precision, value) && i < 0) {
          if (value.sp - LONG_MIN < -i)
            OSL_overflow("osl_int_add_si overflow");
        }
      #endif
      variable->sp = value.sp + (long int)i;
      break;

    case OSL_PRECISION_DP:
      #ifndef NDEBUG
        if (osl_int_pos(precision, value) && i > 0) {
          if (LLONG_MAX - value.dp < i)
            OSL_overflow("osl_int_add_si overflow");
        }
        else if (osl_int_neg(precision, value) && i < 0) {
          if (value.dp - LLONG_MIN < -i)
            OSL_overflow("osl_int_add_si overflow");
        }
      #endif
      variable->dp = value.dp + (long long int)i;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP: {
      mpz_t si;
      mpz_init_set_si(si, i);
      mpz_add(*variable->mp, *value.mp, si);
      mpz_clear(si);
      break;
    }
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable <- val1 - val2;
 */
void osl_int_sub(int precision, osl_int_const_p variable,
                 osl_const_int_t val1, osl_const_int_t val2) {
#ifdef OSL_GMP_IS_HERE
  if (precision == OSL_PRECISION_MP) {
    mpz_sub(*variable->mp, *val1.mp, *val2.mp);
  }
  else
#endif
  {
    osl_int_t mval2; osl_int_init(precision, &mval2);
    osl_int_oppose(precision, &mval2, val2);
    osl_int_add(precision, variable, val1, mval2);
  }
}


/**
 * variable <- val1 * val2;
 */
void osl_int_mul(int precision, osl_int_const_p variable,
                 osl_const_int_t val1, osl_const_int_t val2) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = val1.sp * val2.sp;
      #ifndef NDEBUG
        if (!osl_int_zero(precision, val1) && !osl_int_zero(precision, val2)) {
          if (variable->sp / val2.sp != val1.sp) {
            OSL_overflow("osl_int_mul overflow");
          }
        }
      #endif
      break;

    case OSL_PRECISION_DP:
      variable->dp = val1.dp * val2.dp;
      #ifndef NDEBUG
        if (!osl_int_zero(precision, val1) && !osl_int_zero(precision, val2)) {
          if (variable->dp / val2.dp != val1.dp) {
            OSL_overflow("osl_int_mul overflow");
          }
        }
      #endif
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_mul(*variable->mp, *val1.mp, *val2.mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable <- value * i;
 */
void osl_int_mul_si(int precision,
                    osl_int_const_p variable, osl_const_int_t value, int i) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = value.sp * (long int)i;
      #ifndef NDEBUG
        if (!osl_int_zero(precision, value) && i != 0) {
          if (variable->sp / (long int)i != value.sp) {
            OSL_overflow("osl_int_mul_si overflow");
          }
        }
      #endif
      break;

    case OSL_PRECISION_DP:
      variable->dp = value.dp * (long long int)i;
      #ifndef NDEBUG
        if (!osl_int_zero(precision, value) && i != 0) {
          if (variable->dp / (long long int)i != value.dp) {
            OSL_overflow("osl_int_mul_si overflow");
          }
        }
      #endif
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_mul_si(*variable->mp, *value.mp, i);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable <- -value;
 */
void osl_int_oppose(int precision,
                    osl_int_const_p variable, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = -value.sp;
      break;

    case OSL_PRECISION_DP:
      variable->dp = -value.dp;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_neg(*variable->mp, *value.mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * variable <- | value |;
 */
void osl_int_abs(int precision,
                 osl_int_const_p variable, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      variable->sp = (value.sp > 0) ? value.sp : -value.sp;
      break;

    case OSL_PRECISION_DP:
      variable->dp = (value.dp > 0) ? value.dp : -value.dp;
      break;

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      mpz_abs(*variable->mp, *value.mp);
      break;
#endif

    default:
      OSL_error("unknown precision");
  }
}


/*+***************************************************************************
 *                            Conditional Operations                         *
 *****************************************************************************/


/**
 * (val1 == val2)
 */
int osl_int_eq(int precision, osl_const_int_t val1, osl_const_int_t val2) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return (val1.sp == val2.sp);

    case OSL_PRECISION_DP:
      return (val1.dp == val2.dp);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return (mpz_cmp(*val1.mp, *val2.mp) == 0);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * (val1 != val2)
 */
int osl_int_ne(int precision, osl_const_int_t val1, osl_const_int_t val2) {
  return !osl_int_eq(precision, val1, val2);
}


/**
 * (value > 0)
 */
int osl_int_pos(int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return (value.sp > 0);

    case OSL_PRECISION_DP:
      return (value.dp > 0);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return (mpz_sgn(*value.mp) > 0);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * (value < 0)
 */
int osl_int_neg(int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return (value.sp < 0);

    case OSL_PRECISION_DP:
      return (value.dp < 0);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return (mpz_sgn(*value.mp) < 0);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * (value == 0)
 */
int osl_int_zero(int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return (value.sp == 0);

    case OSL_PRECISION_DP:
      return (value.dp == 0);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return (mpz_sgn(*value.mp) == 0);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * (value == 1)
 */
int osl_int_one(int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return (value.sp == (long int)1);

    case OSL_PRECISION_DP:
      return (value.dp == (long long int)1);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return (mpz_cmp_si(*value.mp, 1) == 0);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * (value == -1)
 */
int osl_int_mone(int precision, osl_const_int_t value) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return (value.sp == (long int)-1);

    case OSL_PRECISION_DP:
      return (value.dp == (long long int)-1);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return (mpz_cmp_si(*value.mp, -1) == 0);
#endif

    default:
      OSL_error("unknown precision");
  }
}


/**
 * ((val1 % val2) == 0)
 */
int osl_int_divisible(int precision,
                      osl_const_int_t val1, osl_const_int_t val2) {
  switch (precision) {
    case OSL_PRECISION_SP:
      return ((val1.sp % val2.sp) == 0);

    case OSL_PRECISION_DP:
      return ((val1.dp % val2.dp) == 0);

#ifdef OSL_GMP_IS_HERE
    case OSL_PRECISION_MP:
      return mpz_divisible_p(*val1.mp, *val2.mp);
#endif

    default:
      OSL_error("unknown precision");
  }
}
