import time
from process import Process
from allocation import Allocation
import gui
MEMORY_SIZE = 100

def firstFit(process, allocations):
    start = 0
    end = 0
    allocations.sort(key=lambda a: a.start)
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
    while processQueue or allocations:
        for allocation in allocations:
            allocation.process.runTime -= 1
            if allocation.process.runTime == 0:
                allocations.remove(allocation)
                gui.removeFromGUI(allocation, MEMORY_SIZE)
        if processQueue:
            nextProcess = processQueue.pop(0)
            if not algorithm(nextProcess, allocations):
                processQueue.insert(0,nextProcess)
        for allocation in allocations:
            print(allocation)
            gui.addToGUI(allocation, MEMORY_SIZE)
        print("------------------------------------------")
        time.sleep(1)

def main():
    processes = [
        Process(1, 40, 2),
        Process(2, 10, 5),
        Process(3, 25, 3),
        Process(4, 40, 4),
        Process(5, 5, 4),
        Process(6, 10, 2),
    ]
    gui.InitGUI()
    scheduler(processes, firstFit)

if __name__ == '__main__':
    main()
