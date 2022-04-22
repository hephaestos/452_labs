class Process:
    def __init__(self, num, size, runTime):
        self.num = num
        self.size = size
        self.runTime = runTime

    def __str__(self):
        return f'Process #:{self.num} Memory size:{self.size} Runtime remaining:{self.runTime}'
