"""
CIS 452 Project 2: Dynamic Memory Allocation
Winter 2022

Authors:
Nick Biesbrock
Daniel Floyd
Caleb Poe
"""
from tkinter import *

class GUI:
    """
    GUI class which handles all the drawing of the boxes and text,
    and also handles the creation of the window.
    """
    def __init__(self, memsize):
        self.memsize = memsize
        # generating GUI and canvas
        self.root = Tk()
        self.root.title('CIS 452 Dynamic Memory Allocation Project')
        # 16x9 aspect ratio
        self.root.geometry("1200x675")
        self.grey = '#bebebe'
        self.lightblue = '#ADD8E6'
        self.canvas = Canvas(self.root, width=1150, height=625, bg=self.grey)
        self.canvas.pack()
        self.width = 4

        self.canvas.create_rectangle(350, 85, 485, 585, fill="blue", width=self.width, outline="blue")
        self.canvas.create_rectangle(620, 85, 755, 585, fill="blue", width=self.width, outline="blue")
        self.canvas.create_rectangle(890, 85, 1025, 585, fill="blue", width=self.width, outline="blue")

        # label for each bar
        self.canvas.create_text(415, 605, text="First Fit", fill="black", font=('Helvetica 13'))
        self.canvas.create_text(685, 605, text="Best Fit", fill="black", font=('Helvetica 13'))
        self.canvas.create_text(955, 605, text="Worst Fit", fill="black", font=('Helvetica 13'))

        # left legend
        self.canvas.create_rectangle(50, 85, 200, 435, fill="blue",width=self.width, outline="blue")
        self.canvas.create_rectangle(55, 90, 195, 235, fill="white",width=self.width, outline="white")

        self.canvas.create_text(120, 100, text="Next Process Size", fill="black", font=('Helvetica 11'))
        self.canvas.create_text(109, 120, text="to be allocated:", fill="black", font=('Helvetica 11'))

        # title bar
        self.canvas.create_text(575, 25, text="CIS 452 Dynamic Memory Allocation Project", fill="black", font=('Helvetica 15'))

        self.root.update_idletasks()
        self.root.update()

    def add(self, allocation, column):
        """ Add processes to the GUI """
        malloc = allocation.start
        size = allocation.process.size
        num = allocation.process.num
        column = column

        self.canvas.create_rectangle(80 + (270*column), 85 + ((500/self.memsize) * malloc), 215 + (270*column), 85 + ((500/self.memsize) * (malloc + size)), fill='red', width=self.width)
        printstring = "Proc " + str(num) + " Size " + str(size)
        self.canvas.create_text(145 + (270*column), 95 + ((500/self.memsize) * malloc + size), text=printstring, fill="white", font=('Helvetica 13'))

        self.root.update_idletasks()
        self.root.update()


    def remove(self, allocation, column):
        """ Remove processes from the GUI """
        malloc = allocation.start
        size = allocation.process.size
        column = column

        self.canvas.create_rectangle(80 + (270*column), 85 + ((500 / self.memsize) * malloc), 215 + (270*column), 85 + ((500 / self.memsize) * (malloc + size)), fill="blue", width=self.width, outline="blue")

        self.root.update_idletasks()
        self.root.update()

    def updateTime(self, timeUnitsPassed, column):
        """ Updates the time units passed for each algorithm """
        printstring2 = f"Time Passed: {timeUnitsPassed}"
        text = self.canvas.create_text(145+(column*270), 620, text=printstring2, fill="black", font=('Helvetica 12'))
        box = self.canvas.create_rectangle(self.canvas.bbox(text),fill=self.grey, width=0)
        self.canvas.tag_lower(box,text)

        
        self.root.update_idletasks()
        self.root.update()
    
    def updateNextProcess(self, process, column):
        """" Updates the next process size to be allocated for each algorithm """
        if column == 1:
            firstString = f"First: {process.size:02d}"
            text = self.canvas.create_text(89, 165, text=firstString, fill="black", font=('Helvetica 12'))
            box = self.canvas.create_rectangle(self.canvas.bbox(text),fill="white", width=0)
            self.canvas.tag_lower(box,text)
        elif column == 2:
            bestString = f"Best: {process.size:02d}"
            text = self.canvas.create_text(90, 190, text=bestString, fill="black", font=('Helvetica 12'))
            box = self.canvas.create_rectangle(self.canvas.bbox(text),fill="white", width=0)
            self.canvas.tag_lower(box,text)
        elif column == 3:
            worstString = f"Worst: {process.size:02d}"
            text = self.canvas.create_text(94, 215, text=worstString, fill="black", font=('Helvetica 12'))
            box = self.canvas.create_rectangle(self.canvas.bbox(text),fill="white", width=0)
            self.canvas.tag_lower(box,text)

        self.root.update_idletasks()
        self.root.update()

    def pause(self):
        self.root.mainloop()