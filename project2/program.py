import time
import copy
import asyncio
from process import Process
from allocation import Allocation
from gui import GUI
MEMORY_SIZE = 100
timeUnitsPassed = 0

def firstFit(options):
    pass

def bestFit(options):
    options.sort(key=lambda a: (a.end - a.start))

def worstFit(options):
    options.sort(key=lambda a: (a.end - a.start), reverse=True)

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
            start = 0
            end = 0
            allocations.sort(key=lambda a: a.start)
            options = []
            for allocation in allocations:
                end = allocation.start
                if end - start >= nextProcess.size:
                    options.append(Allocation(nextProcess, start, end))
                start = allocation.end
            end = MEMORY_SIZE
            if end - start >= nextProcess.size:
                options.append(Allocation(nextProcess, start, end))
            if options:
                algorithm(options) 
                options[0].end = options[0].start + nextProcess.size
                allocations.append(options[0])
            else:
                processQueue.insert(0,nextProcess)

        for allocation in allocations:
            print(allocation)
            gui.addToGUI(allocation, column)
        print("------------------------------------------")
        await asyncio.sleep(1)

async def updateClock(gui):
    timeUnitsPassed += 1
    gui.updateTimeGUI(timeUnitsPassed)
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
        asyncio.create_task(updateClock(g))
    )

if __name__ == '__main__':
    asyncio.run(main())
