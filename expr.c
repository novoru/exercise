/* Reverse Polish caluculator */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vector {
  void **data;
  unsigned int length;
  unsigned int size;
} Vector;

typedef struct Stack {
  Vector *vec;
} Stack;

Vector *new_vec() {
  Vector *vec = malloc(sizeof(Vector));
  vec->data = malloc(sizeof(void *) * 8);
  vec->length = 0;
  vec->size = 8;
  
  return vec;
}

void vec_push(Vector *vec, void* data) {
  if(vec->length > vec->size-1) {
    vec->size *= 2;
    vec->data = realloc(vec->data, sizeof(void*)*vec->size);
  }
  vec->data[vec->length++] = data;
}

Stack *new_stack() {
  Stack *stack = malloc(sizeof(Stack));
  stack->vec = new_vec();

  return stack;
}

void stack_push(Stack *stack, void* data) {
  vec_push(stack->vec, data);
}

void *stack_pop(Stack *stack) {
  if(stack->vec->length == 0)
    return NULL;
  
  void *data = stack->vec->data[stack->vec->length-1];
  stack->vec->length--;
  
  return data;
}

int parse_int(char *s) {
  int i = (int) (*s - '0');
  *s++;
  
  while((isdigit(*s)) && (*s != '\0')) {
    i *= 10;
    i += (*s - '0');
    *s++;
  }

  return i;
}

int main(int argc, char **argv) {

  if (argc < 2)
    exit(0);

  Stack *stack = new_stack();
  
  for(int i = 1; i < argc; i++) {
    if(isdigit(*argv[i])) {
      stack_push(stack, (void *)parse_int(argv[i]));
    }
    else if(strchr("+-*/", *argv[i])) {
      switch(*argv[i]) {
      case '+':
	stack_push(stack, (void*)((int)stack_pop(stack) + (int)stack_pop(stack)));
	break;
      case '-':
	;
	int sub = (int)stack_pop(stack);
	stack_push(stack, (void*)((int)stack_pop(stack) - sub));
	break;
      case '*':
	stack_push(stack, (void*)((int)stack_pop(stack) * (int)stack_pop(stack)));
	break;
      case '/':
	;
	int denom = (int)stack_pop(stack);
	stack_push(stack, (void*)((int)stack_pop(stack) / denom));
	break;
      }
    }
  }
  
  printf("%d\n", (int)stack_pop(stack));

  return 0;
}
