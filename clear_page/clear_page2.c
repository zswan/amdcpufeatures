/*
 * Build:
 *
 * gcc -Wall -O2 -o clear_page clear_page.c
 *
 * Run:
 *
 * taskset -c <core number> ./clear_page
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long u64;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define DECLARE_ARGS(val, low, high)    unsigned low, high
#define EAX_EDX_VAL(val, low, high)     ((low) | ((u64)(high) << 32))
#define EAX_EDX_ARGS(val, low, high)    "a" (low), "d" (high)
#define EAX_EDX_RET(val, low, high)     "=a" (low), "=d" (high)

static __always_inline unsigned long long rdtsc(void)
{
        DECLARE_ARGS(val, low, high);

        asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

        return EAX_EDX_VAL(val, low, high);
}

static inline u64 read_tsc(void)
{
	asm volatile("mfence");
	return rdtsc();
}

static inline void clear_page_q(void *page)
{
	asm volatile("movl $4096/8,%%ecx\n\t"
		     "xorl %%eax,%%eax\n\t"
		     "rep stosq\n\t"
		     :: "D" (page));
}

static inline void clear_page_b(void *page)
{
	asm volatile("movl $4096,%%ecx\n\t"
		     "xorl %%eax,%%eax\n\t"
		     "rep stosb\n\t"
		     :: "D" (page));
}

static struct function {
	const char *name;
	void (*func)(void *);
} funcs[] = {
	{ "clear_page_q", clear_page_q },
	{ "clear_page_b", clear_page_b },
};

#define LOOPS 100000000ULL
void benchmark_func(struct function *f, void *page)
{
	u64 p1, p2;
	int i;

	p1 = read_tsc();
	for (i = 0; i < LOOPS; i++) {
		f->func(page);
	}
	p2 = read_tsc();

	printf("%s, avg cycles: %llu\n", f->name, (p2 - p1) / LOOPS);
}

int main()
{
	void *page;
	int f;

	page = malloc(4096);
	assert(page);

	for (f = 0; f < ARRAY_SIZE(funcs); f++)
		benchmark_func(&funcs[f], page);

	free(page);

	return 0;
}

