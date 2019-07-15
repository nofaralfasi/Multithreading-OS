/*
 * Solution: Use a condition variable to wait until the stack is not
 *           empty.
 *
 * A condition variable is a list of threads waiting for a certain
 * condition to become true. Threads can call wait() to add themselves
 * to the list and wait, or call signal() to wake up one of the threads
 * on the list after the condition has become true.
 *
 * Attempt #1: Wait on the condition variable before locking the mutex.
 *
 * Exercise #11: What might happen if two threads call pop() at the same
 *               time, when the stack only has one element?
 *
 * Conclusion: Testing the condition must be done with the mutex held.
 *
 * NOTE: The syntax here is intentionally incorrect, so this example
 *       does not compile. See 14-condvar.c for the correct syntax.
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

	pthread_mutex_unlock (&lock);

	pthread_cond_signal (&nonempty);
}

struct elem *pop (void)
{
	struct elem *result;

	/* if the stack is empty, wait until another thread pushes a new
	   element */
	if (!top)
		pthread_cond_wait (&nonempty);

	pthread_mutex_lock (&lock);

	result = top;
	if (result)
		top = result->next;

	pthread_mutex_unlock (&lock);

	return result;
}
