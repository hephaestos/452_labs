class Allocation:
    def __init__(self, process, start, end):
        self.process = process
        self.start = start
        self.end = end

    def __str__(self):
        return f'Process #:{self.process.num} Runtime remaining:{self.process.runTime} Start:{self.start} End:{self.end}'
