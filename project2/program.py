import time
MEMORY_SIZE = 50

class Process:
    def __init__(self, num, size, runTime):
        self.num = num
        self.size = size
        self.runTime = runTime

    def __str__(self):
        return f'Process #:{self.num} Memory size:{self.size} Runtime remaining:{self.runTime}'

class Allocation:
    def __init__(self, process, start, end):
        self.process = process
        self.start = start
        self.end = end

    def __str__(self):
        return f'Process #:{self.process.num} Runtime remaining:{self.process.runTime} Start:{self.start} End:{self.end}'

def firstFit(process, allocations):
    start = 0
    end = 0
    for allocation in allocations:
        end = allocation.start
        if end - start >= process.size:
            allocations.append(Allocation(process, start, start + process.size))
            return True
        start = allocation.end
    end = MEMORY_SIZE
    if end - start >= process.size:
        allocations.append(Allocation(process, start, start + process.size))
        return True
    return False

def bestFit(nextProcess, allocations):
    allocations.append(Allocation(nextProcess, 0, 0))

def scheduler(processQueue, algorithm):
    allocations = []
    while(processQueue or allocations):
        for allocation in allocations:
            allocation.process.runTime -= 1
            if allocation.process.runTime == 0:
                allocations.remove(allocation)
        if(processQueue):
            nextProcess = processQueue.pop(0)
            if not algorithm(nextProcess, allocations):
                processQueue.insert(0,nextProcess)
        for allocation in allocations:
            print(allocation)
        print("------------------------------------------")
        time.sleep(1)

if __name__ == '__main__':
    processes = [
        Process(1, 20, 3),
        Process(2, 10, 4),
        Process(3, 30, 2),
        Process(4, 25, 5)
    ]
    scheduler(processes, firstFit)