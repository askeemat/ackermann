#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define ALLOC_MEM_SIZE (100 * 1024 * 1024)

unsigned long *buf, *curr_pos;
unsigned long func_call = 0;

/* ackermann function */
unsigned long ack(unsigned long m, unsigned long n)
{
	unsigned long val;
	unsigned long random_num = rand();
	if (m == 0) {
		val = n + 1;
	}
	else if (n == 0) {
		val = ack(m - 1, 1);
	}
	else {
		val = ack(m - 1, ack(m, n - 1));
	}
	if (func_call < (ALLOC_MEM_SIZE / sizeof(unsigned long))) {
		/* memory write */
		*curr_pos = val;
		curr_pos++;
	}
	func_call++;
	return val;
}
		
		

int main(int argc, char** argv)
{
	/* ackermann fucntion argument */
	unsigned long m = 4, n = 1;
	unsigned long sum = 0, randnum, ackres, caltime;
	int i;
	struct timeval tv1, tv2;

	/* init random */
	srand((unsigned)time(NULL));
	randnum = rand() % 10;
	
	/* memory allocation */
	buf = (unsigned long *)calloc((ALLOC_MEM_SIZE / sizeof(unsigned long)), sizeof(unsigned long));
	if (!buf) {
		fprintf(stderr, "Memory allcation error.\n");
		return -1;
	}
	curr_pos = buf;

	/* get time */
	if (gettimeofday(&tv1, NULL) < 0) {
		fprintf(stderr, "error : gettimeofday()\n");
		return -1;
	}
	
	/* call ackermann and calculation(plus operation) */
	ackres = ack(m, n);

	/* memory read */
	curr_pos = buf;
	for (i = 0; i < (ALLOC_MEM_SIZE / sizeof(unsigned long)); i++) {
		sum += (*curr_pos % (randnum + 3));
		curr_pos++;
	}

	/* get time */
	if (gettimeofday(&tv2, NULL) < 0) {
		fprintf(stderr, "error : gettimeofday()\n");
		return -1;
	}

	/* calculate time */
	if (tv1.tv_usec <= tv2.tv_usec) {
		caltime = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
	}
	else {
		caltime = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (1000000 + tv2.tv_usec - tv1.tv_usec);
	}

	printf("ackermann result=%ld\n", ackres);
	printf("time=%ld[usec]\n", caltime);
	//printf("func_call=%ld\n", func_call);
	//printf("random value=%ld\n", randnum);
	//printf("sum=%ld\n", sum);

	/* release memory */
	free((void *)buf);
	
	return 0;
}
