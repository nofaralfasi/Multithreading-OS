/*
 * Attempt #1: Use a flag to indicate whether another thread is
 *             currently using the stack.
 *
 * Exercise #6: Find a case where more than one thread can enter the
 *              critical region at the same time.
 */

#include <stddef.h> /* for NULL */

static int in_use = 0;

static void lock (void)
{
	/* wait until the stack is not used by other threads */
	while (in_use)
		;

	in_use = 1;
}

static void unlock (void)
{
	in_use = 0;
}

struct elem {
	void *data;
	struct elem *next;
};

static struct elem *top = NULL;

void push (struct elem *new)
{
	lock ();

	new->next = top;
	top = new;

	unlock ();
}

struct elem *pop (void)
{
	struct elem *result;

	lock ();

	result = top;
	if (result)
		top = result->next;

	unlock ();

	return result;
}
