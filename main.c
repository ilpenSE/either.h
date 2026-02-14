#include <stdio.h>
#include "either.h"

DECL_EITHER(const_char_ptr, int);

Either(const_char_ptr, int) getSomething(int x) {
  if (x == 0) return EITHER_L(const_char_ptr, int, "Hello");
  return EITHER_R(const_char_ptr, int, 0);
}

int main() {
  Either(const_char_ptr, int) a = getSomething(1);
  if (a.is_left) {
    printf("%s\n", a.data.left);
  } else {
    printf("%d\n", a.data.right);
  } // TODO: Add unwrap mechanism
	return 0;
}
