import tkinter as tk
from tkinter import ttk
from PIL import Image


def update_progressbar(progressbar, value):
    progressbar["value"] = value


def showOriginal(imgPath):
    img = Image.open(imgPath)
    img.show()


def toNegative(imgPath):
    if imgPath:
        root = tk.Tk()
        root.title("Changing image")

        progressbar = ttk.Progressbar(root, length=300, mode="determinate")
        progressbar.pack()

        img = Image.open(imgPath)
        width, height = img.size

        changed_img = Image.new('RGB', (width, height))

        total_pixels = width * height
        current_pixel = 0
        for i in range(width):
            for j in range(height):
                r, g, b = img.getpixel((i, j))
                changed_img.putpixel((i, j), (int(255 - r), int(255 - g), int(255 - b)))

                # Progress
                current_pixel += 1
                progress = current_pixel / total_pixels * 100
                if current_pixel % 200 == 0:
                    update_progressbar(progressbar, progress)
                    root.update()

        root.destroy()

        result_image = changed_img
        result_image.show()


def toGray(imgPath):
    if imgPath:
        root = tk.Tk()
        root.title("Changing image")

        progressbar = ttk.Progressbar(root, length=300, mode="determinate")
        progressbar.pack()

        img = Image.open(imgPath)

        width, height = img.size

        changed_img = Image.new('RGB', (width, height))

        total_pixels = width * height
        current_pixel = 0
        for i in range(width):
            for j in range(height):
                r, g, b = img.getpixel((i, j))
                avg = (r + g + b) / 3
                changed_img.putpixel((i, j), (int(avg), int(avg), int(avg)))
                current_pixel += 1
                progress = current_pixel / total_pixels * 100
                if current_pixel % 200 == 0:
                    update_progressbar(progressbar, progress)
                    root.update()
        root.destroy()

        result_image = changed_img
        result_image.show()


def toBinary(imgPath, offset):
    #try:
    #    offset = int(offset)
    #except:
    #    return
    root = tk.Tk()
    root.title("Changing image")

    progressbar = ttk.Progressbar(root, length=300, mode="determinate")
    progressbar.pack()

    img = Image.open(imgPath)

    width, height = img.size

    changed_img = Image.new('RGB', (width, height))

    total_pixels = width * height
    current_pixel = 0
    for i in range(width):
        for j in range(height):
            r, g, b = img.getpixel((i, j))
            avg = (r + g + b) / 3
            if int(avg) < 128 + offset:
                avg = 0
            else:
                avg = 255
            changed_img.putpixel((i, j), (int(avg), int(avg), int(avg)))
            current_pixel += 1
            progress = current_pixel / total_pixels * 100
            if current_pixel % 200 == 0:
                update_progressbar(progressbar, progress)
                root.update()
    root.destroy()

    result_image = changed_img
    result_image.show()
