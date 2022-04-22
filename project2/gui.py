from tkinter import *
import random
import time

# generating GUI and canvas
root = Tk()
root.title('Project 2')
root.geometry("1200x675")
grey = '#bebebe'
canvas = Canvas(root, width=1150, height=625, bg=grey)
canvas.pack()

global totalmem
totalmem = 50

#global firstfitmalloc
firstfitmalloc = 0

# gui boxes for first fit list
firstfitguilist = []
firstfitguilist = [None] * 20




class Process:

    def __init__(self, num, size):
        self.num = num
        self.size = size
        runtime = 3

    def tostring(self):
        print('Process #:', self.num, 'Process size:', self.size)


def addtogui(process, guilist, malloc):
    size = process.size
    num = process.num

    guilist[num] = canvas.create_rectangle(x11, ytop + (10 * malloc), x12, ytop + (10 * (malloc + size)) ,fill="black")
    printstring = "Proc " + str(num) + " Size " + str(size)
    canvas.create_text(x11+65, ytop + (10*malloc + size)+10, text=printstring, fill="white", font=('Helvetica 13'))

    root.update_idletasks()
    root.update()

    return firstfitmalloc + size

# my_canvas.create_rectangle(x1, y1, x2, y2 fill ="color")
# x1, y1: top left
# x2, y2: bottom right

ybottom = 550
ytop = ybottom - 500
xwidth = 135
x11 = 350

x12 = x11 + xwidth
x21 = x11 + 2 * xwidth
x22 = x21 + xwidth
x31 = x21 + 2 * xwidth
x32 = x31 + xwidth

# three empty bars
canvas.create_rectangle(x11, ytop, x12, ybottom, fill="blue")
canvas.create_rectangle(x21, ytop, x22, ybottom, fill="blue")
canvas.create_rectangle(x31, ytop, x32, ybottom, fill="blue")

# label for each bar
canvas.create_text(x11 + 65, ybottom + 20, text="First Fit", fill="black", font=('Helvetica 13'))
canvas.create_text(x21 + 65, ybottom + 20, text="Best Fit", fill="black", font=('Helvetica 13'))
canvas.create_text(x31 + 65, ybottom + 20, text="Worst Fit", fill="black", font=('Helvetica 13'))

ProcArray = []
ProcArray.append(Process(0, 5))
ProcArray.append(Process(1, 7))
ProcArray.append(Process(2, 8))
ProcArray.append(Process(3, 10))
ProcArray.append(Process(4, 4))


time.sleep(1)

firstfitmalloc = addtogui(ProcArray[0], firstfitguilist, firstfitmalloc)
print(firstfitmalloc)
time.sleep(1)

firstfitmalloc = addtogui(ProcArray[1], firstfitguilist, firstfitmalloc)
print(firstfitmalloc)
time.sleep(5)
