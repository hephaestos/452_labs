import time
import copy
import asyncio
from process import Process
from allocation import Allocation
from gui import GUI

def firstFit(options):
    """ No sort. Just use the first gap """
    pass

def bestFit(options):
    """ Find the smallest gap """
    options.sort(key=lambda a: (a.end - a.start))

def worstFit(options):
    """ Find the largest gap """
    options.sort(key=lambda a: (a.end - a.start), reverse=True)
class MemoryAllocationVisualizer:
    def __init__(self, memory_size):
        """ Initialize the GUI and memory size"""
        self.memory_size = memory_size
        self.gui = GUI(self.memory_size)

    async def scheduler(self, processes, algorithm, column):
        processQueue = copy.deepcopy(processes)
        timeUnitsPassed = 0
        allocations = []

        # Keep updating untill all processes are done
        while processQueue or allocations:
            # Decrement the remaining runtime of each running process and remove it if it's done
            for allocation in allocations:
                allocation.process.runTime -= 1
                if allocation.process.runTime == 0:
                    allocations.remove(allocation)
                    self.gui.remove(allocation, column)

            # Allocate the next process in the queue if possible
            if processQueue:
                nextProcess = processQueue.pop(0)
                start = 0
                end = 0
                allocations.sort(key=lambda a: a.start)
                options = []

                # Find all the gaps that can fit the process
                for allocation in allocations:
                    end = allocation.start
                    if end - start >= nextProcess.size:
                        options.append(Allocation(nextProcess, start, end))
                    start = allocation.end
                end = self.memory_size
                if end - start >= nextProcess.size:
                    options.append(Allocation(nextProcess, start, end))

                # If any gaps are found, pick one based on the scheduling algorithm
                if options:
                    algorithm(options) 
                    options[0].end = options[0].start + nextProcess.size
                    allocations.append(options[0])
                # Otherwise, put the process back in the queue to be allocated
                else:
                    processQueue.insert(0,nextProcess)

            # Update the GUI with the current allocations and runtime for each algorithm
            for allocation in allocations:
                self.gui.add(allocation, column)
            self.gui.updateTime(timeUnitsPassed, column)
            timeUnitsPassed += 1
            await asyncio.sleep(1)

    async def main(self):
        processes = [
            Process(1, 70, 2),
            Process(2, 10, 5),
            Process(3, 10, 5),
            Process(4, 20, 3),
        ]
        await asyncio.gather(
            asyncio.create_task(self.scheduler(processes, firstFit, 1)),
            asyncio.create_task(self.scheduler(processes, bestFit, 2)),
            asyncio.create_task(self.scheduler(processes, worstFit, 3))
        )

if __name__ == '__main__':
    m = MemoryAllocationVisualizer(memory_size=100)
    asyncio.run(m.main())
