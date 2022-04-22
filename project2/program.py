# very basic
# right now, it only does 1 process at a time
# need to figure out how to allocate new process every second

import time

class Process:
  def __init__(self, num, size, runTime):
    self.num = num # 1-30
    self.size = size # 1-10
    self.runTime = runTime # 1-5

  def toString(self):
    print("Process #:", self.num, "Process size:", self.size)

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

if __name__ == '__main__':
  process1 = Process(1, 20, 3)
  process2 = Process(2, 10, 4)
  process3 = Process(3, 30, 2)
  process4 = Process(4, 25, 5)
  processes = [process1, process2, process3, process4]
  firstFit(processes)