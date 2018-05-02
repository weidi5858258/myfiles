# -*- coding:utf-8 -*-

import tkinter
from tkinter import messagebox
from functools import partial as pto


def test0():
    def resize(ev=None):
        label.config(font="Helvetica -%d bold" % (scale.get()))

    top = tkinter.Tk()
    top.geometry("250x150")

    label = tkinter.Label(top, text="Hello World!", font="Helvetica -12 bold")
    label.pack(fill=tkinter.Y, expand=1)

    scale = tkinter.Scale(top, from_=10, to=40, orient=tkinter.HORIZONTAL, command=resize)
    scale.set(12)
    scale.pack(fill=tkinter.X, expand=1)

    quit = tkinter.Button(top, text="QUIT", command=top.quit, activebackground="red", activeforeground="white")
    quit.pack()

    tkinter.mainloop()


def test1():
    WARN = "warn"
    CRIT = "crit"
    REGU = "regu"

    SIGNS = {
        "do not enter": CRIT,
        "railroad crossing": WARN,
        "55\nspeed limit": REGU,
        "wrong way": CRIT,
        "merging traffic": WARN,
        "one way": REGU,
    }

    critCB = lambda: messagebox.showerror("Error", "Error Button Pressed!")
    warnCB = lambda: messagebox.showwarning("Warning", "Warning Button Pressed!")
    infoCB = lambda: messagebox.showinfo("Info", "Info Button Pressed!")

    top = tkinter.Tk()
    top.title("Road Signs")
    tkinter.Button(top, text="QUIT", command=top.quit, bg="red", fg="white").pack()

    MyButton = pto(tkinter.Button, top)
    CritButton = pto(MyButton, command=critCB, bg="white", fg="red")
    WarnButton = pto(MyButton, command=warnCB, bg="white")
    ReguButton = pto(MyButton, command=infoCB, bg="white")

    for eachSign in SIGNS:
        signType = SIGNS[eachSign]
        cmd = "%sButton(text=%r%s).pack(fill=tkinter.X, expand=True)" % (
            signType.title(), eachSign, ".upper()" if signType == CRIT else ".title()")
        eval(cmd)

    top.mainloop()
    pass


def main():
    test0()


# test1()

if __name__ == "__main__":
    main()

'''
偏函数应用(PFA)
严重级别标志是白底红字,
警告级别标志是黄底黑字,
通知(即标准级别)标志是白底黑字

函数式编程(Functional Programming)
函数式编程的一个特点就是，允许把函数本身作为参数传入另一个函数，还允许返回一个函数！
函数(单单一个函数名)本身也可以赋值给变量，即：变量可以指向函数
函数名其实就是指向函数的变量
把函数作为参数传入，这样的函数称为高阶函数
匿名函数
list(map(lambda x: x * x, [1, 2, 3, 4, 5, 6, 7, 8, 9]))
f = lambda n: n % 2 == 1
f(15)
'''
