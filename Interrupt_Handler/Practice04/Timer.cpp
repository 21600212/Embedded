#include "mbed.h"

Timer t;

int main() {
    int i;
    float sum;
    int start, end;

    printf("\r\n*** Basic Timer Example ***\r\n");

    t.start();
    while(1) {
        sum = 0.0;
        //start = t.read_us();
        start = t.elapsed_time().count();
        for(int i=0; i <= 1000000; i++) {
            sum += i;
        }
        //end = t.read_us();
        end = t.elapsed_time().count();

        printf("The time elapsed is %d us.\r\n", end - start);
    }
}
