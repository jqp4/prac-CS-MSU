#include <stdio.h>
#include <stdlib.h>

/*
 https://seminar.ruro.live/ejudge1/0020
 account (acc) - Bank account balance
 dp_rate (dpr) - Annual interest rate on the deposit
 ln_rate (lnr) - Annual interest rate on the loan
 */

enum {
    FPERCENT = 100,
    ACC_ARGN = 1,
    DPR_ARGN = 2,
    LNR_ARGN = 3,
    MIN_ARGC = 4
};

int interestAccrual(double acc, double dpr, double lnr){
    return acc * (1 + (acc > 0 ? dpr : lnr) / FPERCENT);
}

int main(int argc, char *argv[]){
    if (argc < MIN_ARGC){
        fprintf(stderr, "invalid input\n");
        return 1;
    }
    double account = atof(argv[ACC_ARGN]);
    double dp_rate = atof(argv[DPR_ARGN]);
    double ln_rate = atof(argv[LNR_ARGN]);
    int actc = argc - MIN_ARGC;
    if (actc) {
        for (int i = 0; i < actc; i++) {
            double act = atof(argv[MIN_ARGC + i]);
            account = interestAccrual(account + act, dp_rate, ln_rate);
        }
    } else {
        account = interestAccrual(account, dp_rate, ln_rate);
    }
    fprintf(stdout, "%.10g\n", account);
    return 0;
}
