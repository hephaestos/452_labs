"""
CIS 452 Project 2: Dynamic Memory Allocation
Winter 2022

Authors:
Nick Biesbrock
Daniel Floyd
Caleb Poe
"""
class Process:
    """ Represents a process to be allocated with num, size, and runtime """
    def __init__(self, num, size, runTime):
        self.num = num
        self.size = size
        self.runTime = runTime

    def __str__(self):
        return f'Process #:{self.num} Memory size:{self.size} Runtime remaining:{self.runTime}'
