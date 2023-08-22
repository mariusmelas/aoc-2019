#pragma once
#include <stdio.h>
int swap(int *sequence, int n, int m, size_t len);
int turn_left_right(int *sequence, int l, int r, size_t len);
int next_lexicographical_permutaion(int *sequence, size_t len);
void print_sequence(int *sequence, size_t len);