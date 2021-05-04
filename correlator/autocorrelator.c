#include <stdio.h>
#include <assert.h>
#include "autocorrelator.h"

int
autocorrelate(int period, uint8_t *restrict hard, int len)
{
	int i, j;
	uint8_t tmp, xor, window;
	int ones_count[8*period];
	int best_corr, best_idx;

	/* Make len a multiple of the period */
	len -= len % period;

	for (i=0; i<(int)LEN(ones_count); i++) {
		ones_count[i] = 0;
	}

	/* XOR the bitstream with a delayed version of itself */
	for (i=0; i<period; i++) {
		j = len - period + i - 1;
		tmp = hard[j];
		for (j -= period; j >= 0; j -= period) {
			xor = hard[j] ^ tmp;
			tmp = hard[j];
			hard[j] = xor;
		}
	}

	/* Find the bit offset with the most zeroes */
	window = 0;
	hard--;
	for (i=0; i<8*(len-period); i++) {
		if (!(i%8)) hard++;
		window = (window >> 1) | ((*hard << (i%8)) & 0x80);
		ones_count[i % (8*period)] += count_ones(window);
	}

	best_idx = 0;
	best_corr = ones_count[0] * 9 / 10;
	for (i=1; i<(int)LEN(ones_count); i++) {
		if (ones_count[i] < best_corr) {
			best_corr = ones_count[i];
			best_idx = i;
		}
	}

	return best_idx;
}
