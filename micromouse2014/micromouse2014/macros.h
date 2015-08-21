//
//  macros.h
//  CNC
//
//  Created by Lyle Moffitt on 6/6/14.
//  Copyright (c) 2014 Lyle Moffitt. All rights reserved.
//

#ifndef CNC_macros_h
#define CNC_macros_h

//#include <cstddef>
//#include <cstdint>
#include <sys/types.h>

/// unsigned integer
typedef unsigned int                u_int;

/// long long integer
typedef long long int               ll_int;

/// long long unsigned integer
typedef long long unsigned          llu_int;



/** @addtogroup b_macros Basic Macros
 * @{ */

/** Convienantly shortens "long double" to 3 characters.
 * @def LDB */
# define LDB long double

/** Return a minimum integral value equivalent to sign input.
 * @def SIGN()
 * @param expr Anything that can evaluate to a number.
 * @return +1, -1, or 0
 */
#define SIGN( expr )		\
(	(expr) < 0 ? (-1.0)	:	\
    (expr) > 0 ? (+1.0)	:	\
(+0))

/** Return a directional character based on sign of input 
 * @def C_DIR( expr )
 * @param expr Anything that can evaluate to a number.
 * @return ">" , "<" , or "|"
 */
#define C_DIR( expr )		\
(   (expr) ? "<" : ">"	 	\
)

/** Evalueate the expression but do not return its value
 * @def EVAL( expression )
 * @param expression a commant or anthing that can evaluated on its own
 * @return Nothing
 */
#define EVAL( expression )	\
do{                         \
    expression ;            \
}while(0)

/** Concatenate two things together.
 * @def CAT( a,b )
 * @param a,b Any two things. 
 * @return Those two things concatenated together. NOT a string.
 */
#define CAT( a,b )			\
a ## b

/** Division with (long double) casting and DIV0 checking.
 *
 * This is a super handy macro. There are so many times in C/C++ that
 * a division operation gets messed up. This avoids all the technical 
 * complications and simple divides as ccurately. 
 * @note <tt> long double </tt> Is a **quadrouple** precision float.   
 * @def DIV( a,b )
 * @param 	a,b 	Any two things that evaluate to numbers.
 * @return  Division of \c a over \c b , or just \c a if \c b is zero.
 */
#define DIV( a,b )		    \
(	((double) a ) /	\
    ((double) NZ( b ))	\
)

/** Make sure it's Not Zero.
 * @def NZ( a )
 * @param 	a 	Anything that evaluates to a number.
 * @return If \c a is not zero, then \a is returned. 
 * Otherwise 1.0 is returned.
 */
#define NZ( a )				\
(	((a) != 0)	? (a) : 1	\
)

/** Converts directly to a string, does not evaluate.
 * @def STR( str )
 * @param 	str 	Something that you like to be literally converted 
 * to a sring without evaluation.
 * @return The _literal_ C-string of what you put in.
 */
#define STR( str )			\
#str

/** Returns the greater of the two inputs
 * @def MAX( a,b )
 * @param 	a,b 	Any two thing that evaluate to numbers.
 * @return  The greater of the two.
 */
#define MAX( a,b )			\
(	(a) < (b) ? (b) : (a)	\
)

/** Returns the lesser of the two inputs
 * @def MIN( a,b )
 * @param 	a,b 	Any two thing that evaluate to numbers.
 * @return  The lesser of the two.
 */
#define MIN( a,b )			\
(	(a) > (b) ? (b) : (a)	\
)

/** Return the absolute value of the input
 * @def ABS( a )
 * @param 	a 	Anything that evaluate to a number.
 * @return  The number or its negation, whichever is not less than 0
 */
#define ABS( a )			\
(	(a) < 0 ? -(a) : (a)	\
)

/** A basic "No Op" style command
 * @def NO_OP
 */
#define NO_OP               \
do{                         \
}while(0)	

/** Print command to LOUT and then print its evaluated result to LOUT
 * @def ECHO( a )
 * @param 	a Any evaluatable expression 
 * @return  LOUT "\t" STR( a ) " = " << a << "\t"
 */
#define SHOW( a )			 \
printf( "[ " STR( a ) " ] = ", (a)) \

/** @} */ 

#define AVG(a,b) ( 0.5 * double(a) + 0.5 * double(b) )



/** Define which log levels are on.
 *
 * Each 'place' of the value this macro is assigned to corresponds to 
 * a level of logging. The digit at the place value of 10^N is checked 
 * against by the operation \c LOG_LVL%(10^(N+1)). If the result is 
 * greater than or equal to 10^N, then \c LOG(N) is enabled.
 * Ergo, if <tt> LOG_LVL = 110101 </tt> then, logging levels 1, 3, 5, 
 * and 6 are all enabled; logging levels 2 and 4 are disabled.
 * @addtogroup log_lvl Logging Levels
 * @{ */
#define LOG_LVL 100110111
//              987654321

#if(LOG_LVL%10   >= 1  )
#define LOG1 if(1)
#else
#define LOG1 if(0)
#endif

#if(LOG_LVL%100  >= 10 )
#define LOG2 if(1)
#else
#define LOG2 if(0)
#endif

#if(LOG_LVL%1000 >= 100)
#define LOG3 if(1)
#else
#define LOG3 if(0)
#endif

#if(LOG_LVL%10000 >= 1000)
#define LOG4 if(1)
#else
#define LOG4 if(0)
#endif

#if(LOG_LVL%100000 >= 10000)
#define LOG5 if(1)
#else
#define LOG5 if(0)
#endif
#if(LOG_LVL%1000000 >= 100000)
#define LOG6 if(1)
#else
#define LOG6 if(0)
#endif

#if(LOG_LVL%10000000 >= 1000000)
#define LOG7 if(1)
#else
#define LOG7 if(0)
#endif
#if(LOG_LVL%100000000 >= 10000000)
#define LOG8 if(1)
#else
#define LOG8 if(0)
#endif
#if(LOG_LVL%1000000000 >= 100000000)
#define LOG9 if(1)
#else
#define LOG9 if(0)
#endif
/** @} */

#endif
