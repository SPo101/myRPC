#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int a;
	int b;
} ab;

void f(ab **var){
	ab *pvar = *var;

	pvar->a = 10;
	pvar->b = 20;

}
int main(){
	ab *var = malloc(sizeof(ab));

	f(&var);

	printf("%d %d\n", var->a, var->b);
}
