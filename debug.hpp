/*
 * debug.h
 *
 *  Created on: Aug 10, 2020
 *      Author: bear
 */

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <errno.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(QT_CORE_LIB)
    #include <QDebug>
#endif

#if defined(NDEBUG)

  #define Q_ASSERT(x) (void(0))
  #define PRINT_ERROR(x) (void(0))

#else

  #if !defined(QT_CORE_LIB)
    #define Q_ASSERT(x) (assert(x))
  #endif

  #define PRINT_ERROR(x) printError( __FILE__, __LINE__, __FUNCTION__, x)


  /** Вывод на экран ошибки и места где она произошла.
   *
   *  @param[in] file
   *  @param[in] line
   *  @param[in] function
   *  @param[in] error
   */
  void printError(const char *file, int line, const char *function, int error);
#endif

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_HPP_ */
