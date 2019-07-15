/*
 * Rules for using condition variables:
 * - A condition variable must be initialized before use.
 * - Both the condition variable and the associated condition must be
 *   protected by a mutex. This mutex must be acquired before doing any
 *   operation on the condition variable, and before testing or
 *   modifying the state of the condition.
 * - The condition must be re-checked after wait() returns. Thus,
 *   waiting for the condition to become true must be done in a loop, as
 *   in the following example.
 *
 * Compile and link with -pthread.
 */

#include <stddef.h> /* for NULL */
#include <pthread.h>

/*
 * This definition is equivalent to initializing the mutex with:
 *         pthread_mutex_init (&lock, NULL);
 */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/*
 * This definition is equivalent to initializing the condition variable
 * with:
 *         pthread_cond_init (&nonempty, NULL);
 */
static pthread_cond_t nonempty = PTHREAD_COND_INITIALIZER;

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

	pthread_cond_signal (&nonempty);

	pthread_mutex_unlock (&lock);
}

struct elem *pop (void)
{
	struct elem *result;

	pthread_mutex_lock (&lock);

	/* wait until the stack is not empty */
	while (!top)
		pthread_cond_wait (&nonempty, &lock);

	result = top;
	if (result)
		top = result->next;

	pthread_mutex_unlock (&lock);

	return result;
}
