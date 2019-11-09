#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Process {
    int pid;
    bool alive;
};

int sendElection(const struct Process* src, const struct Process* dst) {
    printf("[%d] sent ELECTION to [%d]\n", src->pid, dst->pid);
    
    if(dst->alive) {
        printf("[%d] sent OK to [%d]\n", dst->pid, src->pid);
        return 0; 
    } else return -1;
}

void holdElection(int pid, const struct Process* system, int systemSize) {
    int curPID = pid;
    int nextElectionPID = -1;
    printf("----\n[%d] is holding election\n", pid);
    
    for(int id=systemSize-1; id>curPID; id--) {
        int rv = sendElection(&system[curPID], &system[id]);
        if(rv == 0) nextElectionPID = id;
    }

    if(nextElectionPID == -1)
        printf("Election concluded, [%d] is elected as the coordinator.\n", curPID);
    else holdElection(nextElectionPID, system, systemSize);
}

int main() {
    int N;
    printf("Enter the number of processes in the system: ");
    scanf("%d", &N);

    printf("Creating %d processes with PIDs 0 - %d.\n\n", N, N-1);
    struct Process* system = (struct Process*) malloc(N*sizeof(struct Process));
    for(int i = 0; i < N; i++) {
        system[i].pid = i;
        system[i].alive = true;
    }
    
    
    printf("Enter the PID of the process that holds elections: ");
    int pid;
    scanf("%d", &pid);

    holdElection(pid, system, N);
    return 0;
}