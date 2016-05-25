#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"
#include "stack.h"
#include "colours.h"

static struct value *val_alloc() {
	struct value *v = calloc(1, sizeof(*v));

	if(!v)
		exit(1);

	v->flags = VAL_ALLOC;

	return v;
}

static void val_free(struct value *v) {

	if(v->flags & VAL_ALLOC) {
		free(v);
	}
}

#define IS_STRING(a) ((a)->type == type_string)
#define IS_INT(a) ((a)->type == type_int)
#define IS_FLOAT(a) ((a)->type == type_float)
#define IS_NUM(a) (IS_INT(a) || IS_FLOAT(a))

struct value *eval(struct stack *o) {
	struct token *t = pop(o);
	struct value *a, *b, *c;
	int i = tokid(t);

	if(i == tokn_label) {
		a = val_alloc();

		a->type = type_int;
		a->flags |= VAL_READONLY;

		a->data.i = 0;

		return a;
	}
	else if(i == tokn_value) {
		return &t->val;
	}
	else if(i == tokn_uplus) {
		a = eval(o);

		if(!IS_NUM(a)) {
			printf("Unary + requires a number\n");
			exit(1);
		}

		return a;
	}
	else if(i == tokn_uminus) {
		a = eval(o);

		if(!IS_NUM(a)) {
			printf("Unary + requires a number\n");
			exit(1);
		}

		b = val_alloc();
		b->type = a->type;
		b->flags |= VAL_READONLY;

		if(a->type == type_int)
			b->data.i = -a->data.i;
		else if(a->type == type_float)
			b->data.d = -b->data.d;
		
		val_free(a);

		return b;
	}
	else if(i == tokn_plus) {
		a = eval(o); b = eval(o);

		if(!IS_NUM(a) || !IS_NUM(b)) {
			printf("Wrong type!\n");
			exit(1);
		}
		c = val_alloc();
		c->flags |= VAL_READONLY;
		if(IS_FLOAT(a) || IS_FLOAT(b))
			c->type = type_float;

		if(IS_FLOAT(b)) {
			if(IS_FLOAT(c))
				c->data.d = b->data.d;
			else
				c->data.i = (int)b->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d = (double)b->data.i;
			else
				c->data.i = b->data.i;
		}
		if(IS_FLOAT(a)) {
			if(IS_FLOAT(c))
				c->data.d += a->data.d;
			else
				c->data.i += (int)a->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d += (double)a->data.i;
			else
				c->data.i += a->data.i;
		}

		val_free(a);
		val_free(b);

		return c;
	}
	else if(i == tokn_minus) {
		a = eval(o); b = eval(o);

		if(!IS_NUM(a) || !IS_NUM(b)) {
			printf("Wrong type!\n");
			exit(1);
		}
		c = val_alloc();
		c->flags |= VAL_READONLY;
		if(IS_FLOAT(a) || IS_FLOAT(b))
			c->type = type_float;

		if(IS_FLOAT(b)) {
			if(IS_FLOAT(c))
				c->data.d = b->data.d;
			else
				c->data.i = (int)b->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d = (double)b->data.i;
			else
				c->data.i = b->data.i;
		}
		if(IS_FLOAT(a)) {
			if(IS_FLOAT(c))
				c->data.d -= a->data.d;
			else
				c->data.i -= (int)a->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d -= (double)a->data.i;
			else
				c->data.i -= a->data.i;
		}

		val_free(a);
		val_free(b);

		return c;
	}
	else if(i == tokn_asterisk) {
		a = eval(o); b = eval(o);

		if(!IS_NUM(a) || !IS_NUM(b)) {
			printf("Wrong type!\n");
			exit(1);
		}
		c = val_alloc();
		c->flags |= VAL_READONLY;
		if(IS_FLOAT(a) || IS_FLOAT(b))
			c->type = type_float;

		if(IS_FLOAT(b)) {
			if(IS_FLOAT(c))
				c->data.d = b->data.d;
			else
				c->data.i = (int)b->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d = (double)b->data.i;
			else
				c->data.i = b->data.i;
		}
		if(IS_FLOAT(a)) {
			if(IS_FLOAT(c))
				c->data.d *= a->data.d;
			else
				c->data.i *= (int)a->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d *= (double)a->data.i;
			else
				c->data.i *= a->data.i;
		}

		val_free(a);
		val_free(b);

		return c;
	}
	else if(i == tokn_slash) {
		a = eval(o); b = eval(o);

		if(!IS_NUM(a) || !IS_NUM(b)) {
			printf("Wrong type!\n");
			exit(1);
		}
		c = val_alloc();
		c->flags |= VAL_READONLY;
		if(IS_FLOAT(a) || IS_FLOAT(b))
			c->type = type_float;

		if(IS_FLOAT(b)) {
			if(IS_FLOAT(c))
				c->data.d = b->data.d;
			else
				c->data.i = (int)b->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d = (double)b->data.i;
			else
				c->data.i = b->data.i;
		}
		if(IS_FLOAT(a)) {
			if(IS_FLOAT(c))
				c->data.d /= a->data.d;
			else
				c->data.i /= (int)a->data.d;
		}
		else {
			if(IS_FLOAT(c))
				c->data.d /= (double)a->data.i;
			else
				c->data.i /= a->data.i;
		}

		val_free(a);
		val_free(b);

		return c;
	}
	else if(i == tokn_fn) {
		int n = t->val.data.i;

		b = val_alloc();
		b->type = type_float;
		b->flags |= VAL_READONLY;

		while(n) {
			a = eval(o);

			if(IS_INT(a))
				b->data.d += a->data.i;
			else if(IS_FLOAT(a))
				b->data.d += a->data.d;

			val_free(a);
				
			n--;
		}
		return b;
	}

	printf("Error: unknown operator\n");
	exit(1);
}

void print_expression(struct stack *output) {
	struct value *v;

	/* Display evaluated expression */
	if(peek(output)) {
		v = eval(output);

		switch(v->type) {
			case type_int:    printf("%d ", v->data.i); break;
			case type_float:  printf("%f ", v->data.d); break;
			case type_string: printf("\"%s\" ", v->data.s); break;
			default: printf("<Unknown type>\n"); break;
		}

		val_free(v);
	}
}