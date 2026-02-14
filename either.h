#ifndef RESULT_H
#define RESULT_H

/*
  Rust's Result<T, E> implementation in C.
  This library DOES NOT DEPEND ANYTHING EXCEPT LIBC!
  This header supposed to be used ONLY FOR C/C++!
  DO NOT USE RAW POINTERS IN EITHER/OPTION/RESULT DECLERATIONS!
  Instead, use (for int*) int_ptr or if you have a special pointer type
  define typedef of it. Because something like this: Result(int*)
  gives some errors. But this is safe: Result(int_ptr)
*/

#ifndef RESULTHDEF
  #ifdef RESULTH_DYNAMIC  // define this when generating DLL/SO
    #if defined(_WIN32) || defined(__CYGWIN__)
      #define RESULTHDEF __declspec(dllexport)
    #else
      #define RESULTHDEF __attribute__((visibility("default")))
    #endif
  #else
    #define RESULTHDEF extern // normal usage like stb-style
  #endif
#endif

#include <stddef.h>
#include <stdbool.h>

// Error Code enum, you can add or change here
typedef enum {
  ERROR_INVALID_ARGUMENT,
  ERROR_NOT_FOUND,
  ERROR_VALIDATION_FAILED,
  ERROR_INTERNAL
} ErrorCode;

typedef struct {
  ErrorCode code;
  const char* message;
} Error;

// Either(L, R) holds 2 different typed value
#define Either(L, R) Either_##L##_##R

#define DECL_EITHER(L, R)                       \
  typedef struct {                              \
    bool is_left;                               \
    union {                                     \
      L left;                                  \
      R right;                                  \
    } data;                                     \
  } Either(L, R);

#define EITHER_L(L, R, lval)                       \
  (Either(L, R)){ .is_left = 1, .data.left = (lval) }

#define EITHER_R(L, R, rval)                        \
  (Either(L, R)){ .is_left = 0, .data.right = (rval) }

// Result(T) aka Either(T, Error) (Wrapper of Either)
#define Result(T) Either(T, Error)
#define DECL_RESULT(T) DECL_EITHER(T, Error)

#define RES_OK(T, val) \
  EITHER_L(T, Error, val)

#define RES_ERR(T, err) \
  EITHER_R(T, Error, err)

// Option(T)
#define Option(T) Option_##T

#define DECL_OPTION(T)                          \
  typedef struct {                              \
    int is_some;                                \
    T value;                                    \
  } Option(T);

#define OPT_SOME(T, val)                          \
  (Option(T)){ .is_some = true, .value = (val) }

#define OPT_NONE(T)                             \
  (Option(T)){ .is_some = false }

// like Option<void>,
// use-case: instead of using Option<bool>
typedef struct {
  int is_error;
  Error error;
} ErrorOrNot;

// TYPES
// Instead of using "*" in Either, Option or Result, use these 

// pointer aliases
typedef int*  int_ptr;
typedef char* char_ptr;
typedef const char* const_char_ptr;
typedef void* void_ptr;

// you can add your own types here

#endif // RESULT_H
