#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int start = 0;
    int finish = 0;
    int waiting = 0;
    int turnaround = 0;
    bool completed = false;
};

// Sort by arrival time, and in case of tie, burst time
bool arrivalBurstComparator(Process a, Process b) {
    if (a.arrival == b.arrival)
        return a.burst < b.burst;
    return a.arrival < b.arrival;
}

// Utility to find the index of the next process to schedule
int findNextProcess(vector<Process>& processes, int current_time) {
    int idx = -1;
    int min_burst = 1e9;

    for (int i = 0; i < processes.size(); ++i) {
        if (!processes[i].completed && processes[i].arrival <= current_time) {
            if (processes[i].burst < min_burst) {
                min_burst = processes[i].burst;
                idx = i;
            }
        }
    }
    return idx;
}

int main() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].pid = i + 1;
        cout << "Enter arrival time for Process " << i + 1 << ": ";
        cin >> processes[i].arrival;
        cout << "Enter burst time for Process " << i + 1 << ": ";
        cin >> processes[i].burst;
    }

    sort(processes.begin(), processes.end(), arrivalBurstComparator);

    int current_time = 0;
    float total_waiting = 0, total_turnaround = 0;
    int completed = 0;

    while (completed < n) {
        int idx = findNextProcess(processes, current_time);

        if (idx == -1) {
            current_time++; // No process ready, CPU idle
            continue;
        }

        processes[idx].start = current_time;
        processes[idx].waiting = current_time - processes[idx].arrival;
        current_time += processes[idx].burst;
        processes[idx].finish = current_time;
        processes[idx].turnaround = processes[idx].finish - processes[idx].arrival;
        processes[idx].completed = true;

        total_waiting += processes[idx].waiting;
        total_turnaround += processes[idx].turnaround;
        completed++;
    }

    cout << "\nPID\tArrival\tBurst\tStart\tWaiting\tTurnaround\tFinish\n";
    for (auto& p : processes) {
        cout << p.pid << "\t" << p.arrival << "\t" << p.burst << "\t"
             << p.start << "\t" << p.waiting << "\t" << p.turnaround << "\t\t" << p.finish << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << total_waiting / n << endl;
    cout << "Average Turnaround Time: " << total_turnaround / n << endl;

    return 0;
}
