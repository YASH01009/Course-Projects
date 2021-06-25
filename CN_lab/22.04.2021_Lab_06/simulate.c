#include <stdio.h>

int main(void) {
    FILE *fp = fopen("observations.csv", "w");

    // MU is the service rate (#packets going out per unit time)
    // NP is the number of packets transmitted for each lambda
    // ML is the maximum lambda we are plotting values upto
    double MU = 20, NP = 1000, ML = 50;
    double ready_ts, serv_start_ts, delay, throughput;

    for(double lambda = 1; lambda < ML; lambda++) {
        ready_ts = 0, serv_start_ts = 1/MU, delay = 0;

        for(int i = 0; i < NP; i++) {
            // print each packet info (uncomment if required)
            // printf("Packet_%d: ready_ts = %lf, serv_start_ts = %lf\n", i, ready_ts, serv_start_ts);\

            delay += serv_start_ts - ready_ts;
            ready_ts += 1/lambda;
            serv_start_ts += 1/MU;

            // this if clause handles lambda < mu
            if(serv_start_ts < (ready_ts + 1/MU))
                serv_start_ts = ready_ts + 1/MU;
        }

        delay /= NP;
        throughput = NP/serv_start_ts;

        fprintf(fp, "%lf, %lf, %lf\n", lambda, delay, throughput);
        printf("lambda = %lf, avg_delay = %lf, throughput = %lf\n", lambda, delay, throughput);
    }

    fclose(fp);
    return 0;
}
