
#include <stdio.h>

class Base {
public:
  virtual int foo() { return 13; }
};

class A : public Base {
public:
  int foo() { return 11; }
  int tar() { return 77; }
};

class B : public Base {
public:
  int foo() { return 15; }
  int bar() { return 99; }
};

B b;

int foo(int a) {

  if (a == 0) {
    throw & b;
  }
  return a;
}

int main(int argc, char *argv[]) {
  try {
    foo(0);
    return 0;
  } catch (const char *msg) {
    return 1;
  } catch (long value) {
    return 2;
  } catch (int *value) {
    return 3;
  } catch (A *value) {
    printf("Catch A\n");
    return value->foo();
  } catch (Base *value) {
    printf("Catch B\n");
    return value->foo();
  }
}