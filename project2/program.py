# very basic
# right now, it only does 1 process at a time
# need to figure out how to allocate new process every second

import time

class Process:
    def __init__(self, num, size, runTime):
        self.num = num # 1-30
        self.size = size # 1-10
        self.runTime = runTime # 1-5

    def __str__(self):
        return f'Process #:{self.num} Memory size:{self.size} Runtime remaining:{self.runTime}'

class Allocation:
    def __init__(self, process, start, end):
        self.process = process
        self.start = start
        self.end = end

def firstFit(processes):
    print("------First Fit------")

    blockSize = 50
    numProcesses = len(processes)
    status = ["Unallocated"] * numProcesses

    for i in range(numProcesses):

        # check if block has enough free space
        if blockSize >= processes[i].size:
            status[i] = "Allocated"
            # remove free space from block
            blockSize -= processes[i].size
            processStartTime = time.time()

        while status[i] != "Freed":
            processCurrentTime = time.time()
            processElapsedTime = processCurrentTime - processStartTime

            # terminate process if it completed its run time
            if processElapsedTime >= processes[i].runTime:
                status[i] = "Freed"
                blockSize += processes[i].size
      
            # print out status every second
            if processElapsedTime.is_integer():
                print(status)

def bestFit(nextProcess, allocations):
    allocations.append(Allocation(nextProcess, 0, 0))
    return True

def scheduler(processQueue, algorithm):
    allocations = []
    while(processQueue or allocations):
        nextProcess = processQueue.pop(0)
        algorithm(nextProcess, allocations)
        for allocation in allocations:
            allocation.process.runTime -= 1
            if allocation.process.runTime == 0:
                allocations.remove(allocation)

if __name__ == '__main__':
    processes = [
        Process(1, 20, 3),
        Process(2, 10, 4),
        Process(3, 30, 2),
        Process(4, 25, 5)
    ]
    scheduler(processes, bestFit)