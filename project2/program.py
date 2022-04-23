import time
import copy
import asyncio
from process import Process
from allocation import Allocation
from gui import GUI
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

def bestFit(process, allocations):
    start = 0
    end = 0
    allocations.sort(key=lambda a: a.start)
    options = []
    for allocation in allocations:
        end = allocation.start
        if end - start >= process.size:
            options.append(Allocation(process, start, end))
        start = allocation.end
    end = MEMORY_SIZE
    if end - start >= process.size:
        options.append(Allocation(process, start, end))
    if options:
        options.sort(key=lambda a: (a.end - a.start))
        options[0].end = options[0].start + process.size
        allocations.append(options[0])
        return True
    return False

def worstFit(process, allocations):
    start = 0
    end = 0
    allocations.sort(key=lambda a: a.start)
    options = []
    for allocation in allocations:
        end = allocation.start
        if end - start >= process.size:
            options.append(Allocation(process, start, end))
        start = allocation.end
    end = MEMORY_SIZE
    if end - start >= process.size:
        options.append(Allocation(process, start, end))
    if options:
        options.sort(key=lambda a: (a.end - a.start), reverse=True)
        options[0].end = options[0].start + process.size
        allocations.append(options[0])
        return True
    return False

async def scheduler(processes, algorithm, gui, column):
    processQueue = copy.deepcopy(processes)
    allocations = []
    while processQueue or allocations:
        for allocation in allocations:
            allocation.process.runTime -= 1
            if allocation.process.runTime == 0:
                allocations.remove(allocation)
                gui.removeFromGUI(allocation, column)
        if processQueue:
            nextProcess = processQueue.pop(0)
            if not algorithm(nextProcess, allocations):
                processQueue.insert(0,nextProcess)
        for allocation in allocations:
            print(allocation)
            gui.addToGUI(allocation, column)
        print("------------------------------------------")
        await asyncio.sleep(1)

async def main():
    processes = [
        Process(1, 70, 2),
        Process(2, 10, 5),
        Process(3, 10, 5),
        Process(4, 20, 3),
    ]
    g = GUI(MEMORY_SIZE)
    await asyncio.gather(
        asyncio.create_task(scheduler(processes, firstFit, g, 1)),
        asyncio.create_task(scheduler(processes, bestFit, g, 2)),
        asyncio.create_task(scheduler(processes, worstFit, g, 3))
    )

if __name__ == '__main__':
    asyncio.run(main())
