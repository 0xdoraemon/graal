#include <polyglot.h>

void *global;

int argTest(int a, int b, int c, int d) {
	return polyglot_get_arg(2);
}
int main() {
  return argTest(1, 2, 42, 4);
}
