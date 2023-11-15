#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char** argv) {
    if(argc < 2 || argc > 4){
        fprintf(2, "usage: kill pid...\n");
        exit(1);
    }
    else {
        if (strcmp(argv[1], "sjf") == 0)
            set_sch_alg(0, atoi(argv[2]), atoi(argv[3]));
        else
            set_sch_alg(1, 0, 0);
    }
    exit(0);
}
