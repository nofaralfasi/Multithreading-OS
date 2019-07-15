/*
 * This is an implementation of a stack which is not thread-safe.
 * Based on an implementation from "What Every Programmer Should Know
 * About Memory" by Ulrich Drepper, p. 90.
 *
 * Exercise #3: What might happen when two threads call push() at the
 *              same time?
 * Exercise #4: What might happen when two threads call pop() at the
 *              same time?
 * Exercise #5: What might happen when one thread calls push() and
 *              another thread calls pop() at the same time?
 *
 * Conclusion: We must make sure that only one thread can execute any of
 *             the following functions at any time.
 */

#include <stddef.h> /* for NULL */

struct elem {
	void *data;
	struct elem *next;
};

static struct elem *top = NULL;

void push (struct elem *new)
{
	new->next = top;
	top = new;
}

struct elem *pop (void)
{
	struct elem *result = top;

	if (result)
		top = result->next;

	return result;
}
