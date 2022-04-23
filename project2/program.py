"""
CIS 452 Project 2: Dynamic Memory Allocation
Winter 2022

Authors:
Nick Biesbrock
Daniel Floyd
Caleb Poe

A GUI that visualized different memory allocation algorithms for process scheduling,
showing how they schedule processes and how they allocate memory.
"""
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
        """ Schedule the processes using the given algorithm """
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
                self.gui.updateNextProcess(nextProcess, column)
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
        """ Initialize the process queue and asychronously schedule the processes with each algorithm """
        processes = [
            Process(1, 70, 2),
            Process(2, 20, 4),
            Process(3, 5, 7),
            Process(4, 20, 2),
            Process(5, 5, 4),
            Process(6, 15, 3),
            Process(7, 37, 7),
            Process(8, 20, 10),
            Process(9, 22, 5),
            Process(10, 7, 3),
            Process(11, 10, 4),
            Process(12, 20, 4),
            Process(13, 6, 2),
            Process(14, 68, 3),
            Process(15, 7, 6),
            Process(16, 28, 5),
            Process(17, 27, 4),
            Process(18, 35, 5),
            Process(19, 23, 5),
            Process(20, 25, 2),
        ]
        algos = [firstFit, bestFit, worstFit]
        await asyncio.gather(*[asyncio.create_task(self.scheduler(processes, algo, i+1)) for i, algo in enumerate(algos)])
        self.gui.pause()

if __name__ == '__main__':
    m = MemoryAllocationVisualizer(memory_size=100)
    asyncio.run(m.main())
