# -*- coding:utf-8 -*-

import threading
import time
import queue
import _thread

beforeUnderlineCount = 25
afterUnderlineCount = 25

print("-" * beforeUnderlineCount + "多线程" + "-" * afterUnderlineCount)

'''
threading.currentThread()
返回当前的线程变量。
threading.enumerate()
返回一个包含正在运行的线程的list。
正在运行指线程启动后、结束前，不包含启动前和终止后的线程。
threading.activeCount()
返回正在运行的线程数量，与len(threading.enumerate())有相同的结果。
run()
表示线程活动的方法。
start()
启动线程的方法。
join([time])
等待至线程中止。
isAlive()
返回线程是否活动的。
getName()
返回线程名。
setName()
设置线程名。

Queue
FIFO先入先出

LifoQueue
LIFO后入先出

PriorityQueue
优先级队列

Queue.qsize()
返回队列的大小。
Queue.empty()
如果队列为空，返回True，反之False。
Queue.full()
如果队列满了，返回True,反之False。
Queue.full()与maxsize大小对应。
Queue.get([block[, timeout]])获取队列，timeout等待时间。
Queue.get_nowait()相当于Queue.get(False)。
Queue.put(item)
写入队列。
Queue.put_nowait(item)
Queue.put(item, False)。
Queue.task_done()
在完成一项工作之后，Queue.task_done()函数向任务已经完成的队列发送一个信号。
Queue.join()
实际上意味着等到队列为空，再执行别的操作。

'''
exitFlag = 0


# 第一个例子
class myThread(threading.Thread):

    def __init__(self, threadID, name, counter):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.counter = counter

    def run(self):
        threadLock.acquire()
        print('开始线程：' + self.name)
        print_time(self.name, self.counter, 5)
        print('退出线程：' + self.name)
        threadLock.release()


def print_time(threadName, delay, counter):
    while counter:
        if exitFlag:
            print('exitFlag = ' + str(exitFlag))
            return
        time.sleep(delay)
        print('%s: %s' % (threadName, time.ctime(time.time())))
        counter -= 1


threadLock = threading.Lock()
threads = []

# 创建新线程
thread1 = myThread(1, 'Thread-1', 1)
thread2 = myThread(2, 'Thread-2', 2)

# 开启新线程
# thread1.start()
# thread2.start()

# 添加线程到线程列表
threads.append(thread1)
threads.append(thread2)


# 第一种方式
# thread1.join()
# thread2.join()
# 第二种方式
# for t in threads:
#     t.join()

# 第二个例子
class testThread(threading.Thread):
    '''
    测试线程的用法
    '''

    def __init__(self, threadID, name, q):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.q = q

    def run(self):
        print('开启线程：' + self.name)
        process_data(self.name, self.q)
        print('退出线程：' + self.name)


def process_data(threadName, q):
    while not exitFlag:
        queueLock.acquire()
        if not workQueue.empty():
            data = q.get()
            print('%s processing %s' % (threadName, data))
            queueLock.release()
        else:
            queueLock.release()
        time.sleep(1)


workQueue = queue.Queue(10)
queueLock = threading.Lock()
threadList = ['Thread-1', 'Thread-2', 'Thread-3']
nameList = ['One', 'Two', 'Three', 'Four', 'Five']
# 存放线程
threads = []
threadID = 1

# 填充队列(放到这里执行，那么就不需要加锁了)
queueLock.acquire()
for word in nameList:
    workQueue.put(word)
print(workQueue.maxsize, workQueue.qsize(), workQueue.full())
queueLock.release()

# 创建新线程并把线程加入列表中
for tName in threadList:
    thread = testThread(threadID, tName, workQueue)
    thread.start()
    threads.append(thread)
    threadID += 1

# 等待队列清空
while not workQueue.empty():
    # print('workQueue.qsize(): %d' % workQueue.qsize())
    # print('workQueue.qsize()' + str(workQueue.qsize()))
    pass

exitFlag = 1

# 等待所有线程完成，所有的线程执行完成后才执行后面的代码
for t in threads:
    t.join()

print('退出主线程')


def testa():
    # time.sleep(1)
    print("a")


def testb():
    # time.sleep(1)
    print("b")


ta = threading.Thread(target=testa)
tb = threading.Thread(target=testb)
for t in [ta, tb]:
    t.start()
# for t in [ta, tb]:
#     t.join()
print("DONE")

# def printTime(threadName, delay):
#     count = 0
#     while count < 5:
#         time.sleep(delay)
#         count += 1
#         print("%s: %s" %(threadName, time.ctime(time.time())))
#
# try:
#     _thread.start_new_thread(printTime, ("Thread-1", 2, ))
#     _thread.start_new_thread(printTime, ("Thread-2", 4, ))
# except:
#     print("Error: ubable to start thread")

# while 1:
#     pass

# time.sleep(30)

from atexit import register
from re import compile
from threading import Thread
from time import ctime
from urllib2 import urlopen as uopen

REGEX = compile("#([\d,]+) in Books")
AMZN = "http://amazon.com/dp/"
ISBNs = {
    "0132269937":"Core Python Programming",
    "0132356139":"Python web Development with Django",
    "0137143419":"Python Fundamentals",
}

def getRanking(isbn):
    page = uopen("%s%s"%(AMZN, isbn))
    data = page.read()
    page.close()
    return REGEX.findall(data)[0]

def _showRanking(isbn):
    print("- %r ranked %s"%(ISBNs[isbn], getRanking(isbn)))

def _main():
    print("at: ", ctime(), "on Amazon...")
    for isbn in ISBNs:
        _showRanking(isbn)

@register
def _atexit():
    print("all DONE at: ", ctime())

if __name__ == "__main__":
    _main()



























