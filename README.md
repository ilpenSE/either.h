## either.h

A tiny, header-only implementation of Rust-like **Result**, **Either**, and **Option** types for C.

* No dependencies except libc
* Works in C and C++
* Simple macro-based type generation
* Suitable for small to medium projects

---

## Features

* `Either(L, R)` – holds one of two types
* `Result(T)` – alias for `Either(T, Error)`
* `Option(T)` – represents optional values
* `ErrorOrNot` – lightweight success/error type
* Cross-platform export support (DLL/SO)

---

## Installation

Just copy `either.h` into your project and include it:

```c
#include "either.h"
```

No build system required.

---

## Error Type

```c
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
```

You can modify `ErrorCode` as needed.

---

# Usage

---

## Result(T)

Declare a Result type:

```c
DECL_RESULT(int, int)
```

This creates:

```c
Result(int)
```

### Returning a value

```c
Result(int) divide(int a, int b) {
  if (b == 0)
    return RES_ERR(int, (Error){ ERR_INVALID_ARG, "division by zero" });

  return RES_OK(int, a / b);
}
```

### Using it

```c
Result(int) res = divide(10, 2);

if (RES_ISERR(&res)) {
  Error err = RES_GETE(&res);
  printf("Error: %s\n", err.message);
} else {
  int value = RES_UNWRAP(&res);
  printf("Result: %d\n", value);
}
```

---

## Either(L, R)

Declare:

```c
DECL_EITHER(int, int, const_char_ptr, str)
```

Use:

```c
Either(int, str) e = EITHER_L(int, str, 42);
```

Check:

```c
if (e.is_left) {
  printf("%d\n", e.data.left);
} else {
  printf("%s\n", e.data.right);
}
```

---

## Option(T)

Declare:

```c
DECL_OPTION(int, int)
```

Return:

```c
Option(int) find_value(int x) {
  if (x > 0)
    return OPT_SOME(int, x);

  return OPT_NONE(int);
}
```

Use:

```c
Option(int) opt = find_value(5);

if (opt.is_some) {
  printf("Value: %d\n", OPT_UNWRAP(&opt));
}
```

---

## ErrorOrNot

For functions that only return success or error:

```c
ErrorOrNot do_work(void) {
  return EON_ERROR(ERR_INTERNAL, "something failed");
}
```

Check:

```c
ErrorOrNot r = do_work();

if (r.is_error) {
  printf("Error: %s\n", r.error.message);
}
```

---

# Full Example (`main.c`)

```c
#include <stdio.h>
#include "either.h"

DECL_RESULT(int, int)
DECL_OPTION(int, int)

Result(int) safe_div(int a, int b) {
  if (b == 0)
    return RES_ERR(int, (Error){ ERR_INVALID_ARG, "division by zero" });

  return RES_OK(int, a / b);
}

Option(int) maybe_positive(int x) {
  if (x > 0)
    return OPT_SOME(int, x);

  return OPT_NONE(int);
}

int main(void) {
  Result(int) r = safe_div(10, 2);

  if (RES_ISERR(&r)) {
    printf("Error: %s\n", RES_GETE(&r).message);
  } else {
    printf("Result: %d\n", RES_UNWRAP(&r));
  }

  Option(int) o = maybe_positive(5);

  if (o.is_some)
    printf("Option value: %d\n", OPT_UNWRAP(&o));

  return 0;
}
```

Compile:

```bash
gcc main.c -o main
```

---

# Notes

* Do not use raw pointer types directly in macro names.
  Instead use typedefs:

  ```c
  typedef int* int_ptr;
  DECL_RESULT(int_ptr, int_ptr)
  ```

* `RES_UNWRAP` and `OPT_UNWRAP` do not perform safety checks.

* This library does not handle ownership or memory management.

---

## License

Use freely in your projects.
