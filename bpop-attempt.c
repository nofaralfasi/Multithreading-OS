/*
 * Suppose we want to modify pop() so that it always succeeds; if pop()
 * is called when the stack is empty, it must wait until another thread
 * calls push(), and then pop the new element from the stack and return
 * it.
 *
 * Attempt #1: Wait until the stack is not empty, then continue as
 *             before.
 *
 * Compile and link with -pthread.
 *
 * Exercise #9: What might happen if two threads call pop() at the same
 *              time, when the stack only has one element?
 */

#include <stddef.h> /* for NULL */
#include <pthread.h>

/*
 * This definition is equivalent to initializing the mutex with:
 *         pthread_mutex_init (&lock, NULL);
 */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct elem {
	void *data;
	struct elem *next;
};

static struct elem *top = NULL;

void push (struct elem *new)
{
	pthread_mutex_lock (&lock);

	new->next = top;
	top = new;

	pthread_mutex_unlock (&lock);
}

struct elem *pop (void)
{
	struct elem *result;

	/* wait until the stack is not empty */
	while (!top)
		;

	pthread_mutex_lock (&lock);

	result = top;
	if (result)
		top = result->next;

	pthread_mutex_unlock (&lock);

	return result;
}
