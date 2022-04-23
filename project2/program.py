import time
import asyncio
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

def bestFit(process, allocations):
    start = 0
    end = 0
    allocations.sort(key=lambda a: a.start)
    options = []
    for allocation in allocations:
        end = allocation.start
        if end - start >= process.size:
            options.append(Allocation(process, start, start + process.size))
        start = allocation.end
    end = MEMORY_SIZE
    if end - start >= process.size:
        options.append(Allocation(process, start, start + process.size))
    if options:
        options.sort(key=lambda a: a.end - a.start)
        allocations.append(options[0])
        return True
    return False

async def scheduler(processQueue, algorithm, name):
    allocations = []
    while processQueue or allocations:
        for allocation in allocations:
            allocation.process.runTime -= 1
            if allocation.process.runTime == 0:
                allocations.remove(allocation)
                gui.removeFromGUI(allocation, MEMORY_SIZE, 1)
        if processQueue:
            nextProcess = processQueue.pop(0)
            if not algorithm(nextProcess, allocations):
                processQueue.insert(0,nextProcess)
        for allocation in allocations:
            print(allocation)
            gui.addToGUI(allocation, MEMORY_SIZE, 1)
        print("------------------------------------------")
        await asyncio.sleep(1)

async def main():
    processes = [
        Process(1, 40, 2),
        Process(2, 10, 5),
        Process(3, 25, 3),
        Process(4, 40, 4),
        Process(5, 5, 4),
        Process(6, 10, 2),
    ]
    # gui.InitGUI()
    await asyncio.gather(
        asyncio.create_task(scheduler(processes, firstFit, "ONE")),
        asyncio.create_task(scheduler(processes, firstFit, "TWO")),
    )

if __name__ == '__main__':
    asyncio.run(main())
