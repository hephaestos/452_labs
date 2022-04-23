from tkinter import *

# generating GUI and canvas
root = Tk()
root.title('CIS 452 Dynamic Memory Allocation Project')
# 16x9 aspect ratio
root.geometry("1200x675")
grey = '#bebebe'
lightblue = '#ADD8E6'
canvas = Canvas(root, width=1150, height=625, bg=grey)
canvas.pack()
width = 4

def addToGUI(allocation, memsize):
    malloc = allocation.start
    size = allocation.process.size
    num = allocation.process.num

    canvas.create_rectangle(350, 85 + ((500/memsize) * malloc), 485, 85 + ((500/memsize) * (malloc + size)), fill='red', width=width)
    printstring = "Proc " + str(num) + " Size " + str(size)
    canvas.create_text(415, 95 + ((500/memsize) * malloc + size), text=printstring, fill="white", font=('Helvetica 13'))

    root.update_idletasks()
    root.update()


def removeFromGUI(allocation, memsize):
    malloc = allocation.start
    size = allocation.process.size
    canvas.create_rectangle(350, 85 + ((500 / memsize) * malloc), 485, 85 + ((500 / memsize) * (malloc + size)), fill="blue", width=width, outline="blue")

    root.update_idletasks()
    root.update()


def InitGUI():
    # my_canvas.create_rectangle(x1, y1, x2, y2 fill ="color")
    # x1, y1: top left
    # x2, y2: bottom right

    # three empty bars
    canvas.create_rectangle(350, 85, 485, 585, fill="blue", width=width, outline="blue")
    canvas.create_rectangle(620, 85, 755, 585, fill="blue", width=width, outline="blue")
    canvas.create_rectangle(890, 85, 1025, 585, fill="blue", width=width, outline="blue")

    # label for each bar
    canvas.create_text(415, 605, text="First Fit", fill="black", font=('Helvetica 13'))
    canvas.create_text(685, 605, text="Best Fit", fill="black", font=('Helvetica 13'))
    canvas.create_text(955, 605, text="Worst Fit", fill="black", font=('Helvetica 13'))

    # left legend
    canvas.create_rectangle(50, 85, 200, 435, fill="blue",width=width, outline="blue")
    canvas.create_rectangle(55, 90, 195, 235, fill="white",width=width, outline="white")
    canvas.create_text(120, 100, text="Next Process Size", fill="black", font=('Helvetica 11'))
    canvas.create_text(109, 120, text="to be allocated:", fill="black", font=('Helvetica 11'))

    canvas.create_text(89, 165, text="First: 50", fill="black", font=('Helvetica 12'))
    canvas.create_text(90, 190, text="Best: 00", fill="black", font=('Helvetica 12'))
    canvas.create_text(94, 215, text="Worst: 00", fill="black", font=('Helvetica 12'))

    # title bar
    canvas.create_text(575, 25, text="CIS 452 Dynamic Memory Allocation Project", fill="black", font=('Helvetica 15'))

    root.update_idletasks()
    root.update()

