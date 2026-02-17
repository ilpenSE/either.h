#ifndef EITHER_H
#define EITHER_H

/*
  Rust's Result<T, E> implementation in C.
  This library DOES NOT DEPEND ANYTHING EXCEPT LIBC!
  This header supposed to be used ONLY FOR C/C++!
  DO NOT USE RAW POINTERS IN EITHER/OPTION/RESULT NAME PARAMS!
  Instead, use (for int*) int_ptr or if you have a special pointer type
  define typedef of it. Because something like this: Result(int*)
  gives some errors. But this is safe: Result(int_ptr)
*/

#ifndef EITHERHDEF
  #ifdef EITHERHDEF_DYNAMIC  // define this when generating DLL/SO
    #if defined(_WIN32) || defined(__CYGWIN__)
      #define EITHERHDEF __declspec(dllexport)
    #else
      #define EITHERHDEF __attribute__((visibility("default")))
    #endif
  #else
    #define EITHERHDEF extern // normal usage like stb-style
  #endif
#endif

#include <stddef.h>
#include <stdbool.h>

// Error Code enum, you can add or change here
typedef enum {
  ERR_NOERR,
  ERR_INVALID_ARG,
  ERR_NOT_FOUND,
  ERR_VALIDATION_FAILED,
  ERR_INTERNAL
} ErrorCode;

typedef struct {
  ErrorCode code;
  const char* message;
} Error;

#define MAKE_ERR(ecode, emsg) \
  ((Error){.code=(ecode), .message=(emsg)})

// Either(L, R) holds 2 different typed value
#define Either(LName, RName) Either_##LName##_##RName

#define DECL_EITHER(LType, LName, RType, RName) \
  typedef struct {                              \
    bool is_left;                               \
    union {                                     \
      LType left;                               \
      RType right;                              \
    } data;                                     \
  } Either(LName, RName);

// Producing Either structs for returns at functions
#define EITHER_L(LName, RName, lval)                          \
  (Either(LName, RName)){ .is_left = 1, .data.left = (lval) }
#define EITHER_R(LName, RName, rval)                            \
  (Either(LName, RName)){ .is_left = 0, .data.right = (rval) }

// Unwrap mechanism to get R (right) and L (left)
#define EITHER_GETL(either)                     \
  ((either)->data.left)
#define EITHER_GETR(either)                     \
  ((either)->data.right)

// we have already is_left field to query value is left or right

// Result(T) aka Either(T, Error) (Wrapper of Either)
#define Result(TName) Either(TName, Error)
#define DECL_RESULT(T, TName) DECL_EITHER(T, TName, Error, Error)

// Producing Result objects for returns
#define RES_OK(TName, val)                      \
  EITHER_L(TName, Error, (val))
#define RES_ERR(TName, errcode, errmsg)         \
  EITHER_R(TName, Error, MAKE_ERR(errcode, errmsg))

// Unwraps blindly, just unwrap
#define RES_UNWRAP(res)                         \
  ((res)->data.left)

// Left: Value, Right: Error
#define RES_ISERR(res)                          \
  (!(res)->is_left)

// Get error without asking
#define RES_GETE(res) \
  (EITHER_GETR((res)))

// Option(T)
#define Option(TName) Option_##TName

#define DECL_OPTION(TType, TName)               \
  typedef struct {                              \
    bool is_some;                               \
    TType value;                                \
  } Option(TName);

// Producing Option structs for returns
#define OPT_SOME(TName, val)                     \
  ((Option(TName)){ .is_some = true, .value = (val) })
#define OPT_NONE(TName)                         \
  ((Option(TName)){ .is_some = false })

// Unwrap blindly
#define OPT_UNWRAP(opt)                         \
  ((opt)->value)

// we have already is_some to query the value is either Some or None

// like Option<void>,
// instead of using Option<bool>, use this
// (if bool is just an error state)
typedef struct {
  bool is_error;
  Error error;
} ErrorOrNot;

#define EON_SUCCESS ((ErrorOrNot){ false, { ERR_NOERR, NULL } })

#define EON_ERROR(EEnum, EMessage) ((ErrorOrNot){ true, { .code=EEnum, .message=EMessage } })

#endif // EITHER_H
