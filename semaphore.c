/*
 * A semaphore is similar to a condition variable, but with an
 * additional counter of unhandled "signals".
 *
 * A thread can call wait() to wait for the semaphore, or post() to wake
 * up other threads waiting for the semaphore. When a thread calls
 * wait(), if there are unhandled "signals" (ie. the counter is greater
 * than zero), the counter is decremented by 1 and the thread continues.
 * Otherwise (ie. the counter is zero), the thread is put to sleep until
 * another thread calls post().
 *
 * When a thread calls post(), if there are threads waiting for the
 * semaphore, one of them is woken up. Otherwise, the counter is
 * incremented by 1 in order to "remember" the unhandled wake-up.
 *
 * Compile and link with -pthread.
 */

#include <stddef.h> /* for NULL */
#include <pthread.h>
#include <semaphore.h>

/*
 * This definition is equivalent to initializing the mutex with:
 *         pthread_mutex_init (&lock, NULL);
 */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* This semaphore "remembers" how many elements are on the stack */
static sem_t pushed;

/*
 * stack_init() will be called before the program starts.
 * stack_exit() will be called after the program terminates.
 */
static void __attribute__((constructor)) stack_init (void)
{
	sem_init (&pushed, 0, 0);
}

static void __attribute__((destructor)) stack_exit (void)
{
	sem_destroy (&pushed);
}

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

	sem_post (&pushed);
}

struct elem *pop (void)
{
	struct elem *result;

	/* wait until the stack is not empty */
	sem_wait (&pushed);

	pthread_mutex_lock (&lock);

	result = top;
	if (result)
		top = result->next;

	pthread_mutex_unlock (&lock);

	return result;
}
