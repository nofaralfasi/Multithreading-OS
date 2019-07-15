/*
 * Solution: Use a mutex to protect the stack from simultaneous accesses
 *           by multiple threads.
 *
 * A mutex is a synchronization object that guarantees mutual exclusion
 * between threads (hence its name). A mutex can be either "unlocked" or
 * "locked" by exactly one thread (the "owner" of the mutex).
 *
 * Rules for using mutexes:
 * - A mutex must be initialized before use.
 * - The owner of a mutex may not lock it again.
 * - Only the owner of a mutex may unlock it.
 * - Multiple mutexes must always be acquired (ie. locked) in the same
 *   order by all threads that use them.
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

	pthread_mutex_lock (&lock);

	result = top;
	if (result)
		top = result->next;

	pthread_mutex_unlock (&lock);

	return result;
}
