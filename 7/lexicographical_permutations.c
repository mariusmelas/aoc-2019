#include <stdlib.h>
#include <stdio.h>

int swap(int *sequence, int n, int m, size_t len) {
    // Swap sequence[n] with sequence[m]
    if(n < 0 || n >= len || m < 0 || m >= len) {
        printf("m and n must be in range 0-%d\n", len-1);
        return 0;
    }

    int temp = sequence[n];
    sequence[n] = sequence[m];
    sequence[m] = temp;

    return 1;
}

// Turns array seq[l:r]
int turn_left_right(int *sequence, int l, int r, size_t len) {
        if(l < 0 || l >= len || r < 0 || r >= len) {
        printf("l and r must be in range 0-%d\n", len-1);
        return 0;
    }

    while(l < r) swap(sequence, l++, r--, len);
    return 1;
}



int next_lexicographical_permutaion(int *sequence, size_t len) {

    // Find longest non-increasing suffix
    int i = len - 2;
    while(i >= 0 && sequence[i] > sequence[i+1]) i--;
    if(i < 0) return 0;

    int j = len -1;
    while(sequence[j] < sequence[i]) j--;
    swap(sequence, i,j,len);
    turn_left_right(sequence,i+1,len-1, len);

    return 1;
}

void print_sequence(int *sequence, size_t len) {
    for(int i = 0; i < len; i++)
        printf("%d,",sequence[i]);
    printf("\n");
}
