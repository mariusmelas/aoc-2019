#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./lexicographical_permutations.h"

int main() {

    FILE *fp; 
    
    // One sequence of amp phases
    int phase_sequence[5] = {0,1,2,3,4};
    // Input signal for the first amp
    int output_signal = 0;
    int max_thrust = 0;

    
    while(next_lexicographical_permutaion(phase_sequence, 5)) {
        for(int j = 0; j < 1; j++) {
            for(int i = 0; i < 5; i++) {
                //printf("OUTPUT: %d\n", output_signal);
                char *format_str = "{ echo %d; echo %d; } | ./intcode_computer";
                char run_str[200];
                snprintf(run_str,200, format_str,phase_sequence[i], output_signal);

                char buffer[10];
                // Opens new process, redirects phase_sequence and input_signal to stdin
                fp = popen(run_str, "r");
                fgets(buffer, sizeof(buffer), fp);
            // printf("waits here?\n");
                fclose(fp);
                // Sets outputsignal
                output_signal = atoi(buffer);
            }

            if(output_signal > max_thrust) {
                max_thrust = output_signal;
            }
            output_signal = 0;
        }

    }

    

    
   
    printf("Last output: %d\n", output_signal);
    printf("max: %d\n", max_thrust);
    return 0;
}