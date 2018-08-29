# -*- coding:utf-8 -*-

import os
import sys
import platform
import keyword
import math
import random
import time
import urllib
import tkinter
import operator
import pygeoip
import dpkt
import socket
import optparse
import pygame
from pygame.locals import *
from urllib.request import urlopen
from urllib.request import Request
from urllib.parse import urlencode

# reload(sys)
# sys.setdefaultencoding('utf-8')
# from PyQt4 import QtGui

'''
三个单引号注释
http://code.google.com/p/android-scripting/58d4e639cae7e70d3d00199d

两种编码方式（前面有#）：
1. #coding=utf-8 
2. #-*- coding:utf-8 -*- （python推荐使用的）
如果代码中有中文，那么不加“#coding=utf-8”时
用python2运行时应用crash，
用python3运行没有问题，也不需要加上“#coding=utf-8”。

变量
变量可以存储任何类型的内容
字符串必须用双引号或者单引号括起来
%s      字符串
%c      单个字符
%d      整数
%f      小数
%.2f    保留小数点后两位
%lf     小数
%.3lf   保留小数点后三位

import platform
print(platform.uname())
uname_result(system='Linux', node='wangliwei', release='4.4.0-119-generic', version='#143-Ubuntu SMP Mon Apr 2 16:08:24 UTC 2018', machine='x86_64', processor='x86_64')

# 用python2时会出错
name=input("please input your name: ") 
print(name)
# 用python3时会出错
name=raw_input("Please your name: ") 
print(name)
用input让用户输入的即使是数字，用变量保存后就是一个字符串

在python3中，用input输入的内容都是字符串
在python2中，用input输入的内容如果是一个表达式时也能完成操作
func_new = input("...")
# 把字符串转成表达式
func_new = eval(func_new)

交换两个变量的值
a = 4 
b = 5

1.
a = a + b
b = a - b
a = a - b

2.
a,b = b,a

比较运算符
== 
!= 两边的值不相等时为真
<> 两边的值不相等时为真
> 
< 
>=
<=
逻辑运算符
and 与
or  或
not 非

1个字节能存储的最大值是255
1个字节能存储一个字符

如果要使用unicode编码,
则在字符之前使用字符u进行标识,
如u"magedu"

dir(str)
help(str.find)

酒神曲
九月九酿新酒 
好酒出在咱的手好酒 
喝了咱的酒 
上下通气不咳嗽 
喝了咱的酒 
滋阴壮阳嘴不臭 
喝了咱的酒 
一人敢走青刹口 
喝了咱的酒 
见了皇帝不磕头 
一四七三六九 
九九归一跟我走 
好酒好酒好酒

var_name = open(file_name, [mode,[bufsize]])
mode:	
	r
	w
	a
	r+
	w+
	a+
b表示以二进制模式打开文件:
	rb
	wb
	ab
	rb+
	wb+
	ab+
缓冲:
	负数:使用系统默认缓冲
	0:禁用
	1:使用缓冲,只缓冲一行数据
	2+:指定缓冲空间大小

file1 = open("/root/temp/", "r")
type(file1)
id(file1)
file1.next()
文件调用close()后就不能再调用其他函数进行操作了
file1.readline()
file1.readlines()
file1.tell()返回当前指针在文件中的位置(字节数)
file1.seek(100, 0)从文件开始处进行偏移(默认)
file1.seek(100, 1)从文件当前位置进行偏移
file1.seek(100, 2)从文件结尾处进行偏移
file1.seek(0)
file1.name属性,不是函数
list1 = [ i+"\n" for i in os.listdir("\n") ]
file1.writelines(list1)

可以判断目录或者文件是否存在 
os.path.exists(path or filename) 

创建(文件或者目录):
创建一个空文件(不是目录)
os.mknod(filename)
1.创建一个空目录(不是文件)
2.如果system目录已存在,则抛异常
os.mkdir("system")
1.如果500px目录已存在,则抛异常(system, app目录存在没关系)
2.如果system,app,500px都不存在,则全部创建
os.makedirs("system/app/500px")

删除(文件或者目录):
只能删除文件,不能删除目录
os.remove(filename)
1.如果system目录不存在,则抛异常
2.如果system目录不为空,则抛异常
os.rmdir("system")
删除空目录或者非空目录(最终路径只能是目录)
import shutil
shutil.rmtree(path)

关于文件和目录的总结:
创建文件或者目录要先判断一下
是否已经存在,如果存在了还创建则抛异常
删除文件或者目录要先判断一下
是否已经存在,如果不存在还删除则抛异常

tuple = ('abc',('def','ghi'))
tuple[1]
# ('def', 'ghi')
tuple[1][0]
# 'def'

元组因为是不可变的数据,所以是不能直接调用函数
进行排序的,要先转换为列表,然后把这个列表进行排序,
最后把这个列表转换为元组

列表,元组,集合,字典之间的区别
		列表		元组		集合		字典
英文		list		tuple		set		dict
可否读写	读写		只读		读写		读写
可否重复	是		是		否		是
存储方式	值		值		键(不能重复)	键值对(键不能重复)
是否有序	有序		有序		无序		无序,自动正序
初始化		[1, 'a']	('a', 1)	set([1,2])或{1,2}	{'a':1, 'b':2}
添加		append		只读		add		d['key'] = 'value'
读元素		l[2:]		t[0]		无		d['a']

# 列表转集合(去重)
list1 = [6, 7, 7, 8, 8, 9]
set(list1)
# {6, 7, 8, 9}

# 两个列表转字典
list1 = ['key1','key2','key3']
list2 = ['1','2','3']
dict(zip(list1,list2))
# {'key1': '1', 'key2': '2', 'key3': '3'}

# 嵌套列表转字典
list3 = [['key1','value1'],['key2','value2'],['key3','value3']]
dict(list3)
# {'key1': 'value1', 'key2': 'value2', 'key3': 'value3'}

# 字典key和value互转
dic2 = {'a': 1, 'b': 2, 'c': 3}
{value:key for key, value in a_dict.items()}
# {1: 'a', 2: 'b', 3: 'c'}

# 列表、元组、集合、字典转字符串
list2 = ['a', 'a', 'b']
''.join(list2)
# 'aab'
tup1 = ('a', 'a', 'b')
''.join(tup1)
# 'aab'
set1 = {6, 7, 8, 9}
str(set1)
# '{8, 9, 6, 7}'
dic1 = {'a':1, 'b':2}
str(dic1)
# "{'a': 1, 'b': 2}"

# 字符串转列表、元组、集合、字典
s = 'aabbcc'
list(s)
# ['a', 'a', 'b', 'b', 'c', 'c']
tuple(s)
# ('a', 'a', 'b', 'b', 'c', 'c')
set(s)
# {'a', 'b', 'c'}
s = "{'name':'ljq', 'age':24}"
dic2 = eval(s)

# 切分字符串
a = 'a b c'
a.split(' ')
# ['a', 'b', 'c']

元组转换为列表
1.T = ('cc', 'aa', 'dd', 'bb')
  tmp = list(T)
2.tmp = [x for x in T]
元组转换为列表并排序
T = ('cc', 'aa', 'dd', 'bb')
sorted(T)
列表转换为元组
tmp = ['cc', 'aa', 'dd', 'bb']
T = tuple(tmp)

nums = [1123, 12, 3, 12, 42, 55, 445, 200]
nums.sort()
nums.sort(reverse=True)
nums.reverse()

result = lambda a,b:a+b

infos = [
{"name":"laowang", "age":10},
{"name":"laowang", "age":10}, 
{"name":"laowang", "age":10}]
infos.sort(key=lambda x:x["name"])

a = [100]
# 1.
def test(num):
    # 先检查num这个值是否可变，如果可变，那么对num直接进行操作
    # 如果num是个不可变的值，那么num指向了新生成的值 
    num += num
    print(num)

test(a)
print(a)

def test(num):
    # num指向了新生成的值 
    num = num + num
    print(num)
# 2.
test(a)
print(a)

LOCAL_INIT_RC := xxx.rc

exec内置表达式
sorted()
dir()

模块的发布和安装：
1.在与包同级处新建一个文件setup.py
2.往setup.py中写入下面内容
	from distutils.core import setup
	set(name="weidi", version="1.0", dexcription="weidi modules", author="weidi", py_modules=['包名.模块名', '', ''])
3.python3 setup.py build
4.python3 setup.py sdist
最终会生成一个dist目录，里面会生成一个XXX.tar.gz压缩包

别人怎么用：
1.tar -zxvf XXX.tar.gz
2.cd XXX
3.python3 setup.py install 

接受程序输入参数：
import sys
print(sys.argv)

元组、列表可以有相同的元素，集合不会有相同的元素，相同的元素只会输出一个
list(set) list(tuple)
set(list) set(tuple)
tuple(list) tuple(set)

# from xxx import * 时,只有写在__all__里面的东西在外面才能用函数名直接调用
__all__ = ['test1', 'test2']

def test1():
	pass

def test2():
	pass

class Test(object):
	pass

# 一个Test目录下如果有__init__.py这个文件,
# 那么python解释器会把这个Test目录当成一个Test包.
# 当在外面import这个Test包时解释器会自动执行__init__.py里面的代码
# Test包下面的py文件都是一个个的模块

# __init__.py
# 影响from Test包下面的模块能否被导入,
# 没有被写进去的模块在外面是使用不了的
__all__ = ['模块名']

# python2
import sendmsg

# python2 python3
# 从当前路径下导入
from . import sendmsg


pip3 install Django==1.11.4

如果要在任何一个地方都要使用自己或者别人写的一个包,
那么需要经过这样的操作:
1.在与这个包几级的目录下创建setup.py文件
2.在里面写上这样的内容
  from distutils.core import setup
  setup(name='dongGe', version='1.0', description='dongGe's module', author='dongGe', py_modules=['suba.aa', 'suba.bb', 'subb.cc', 'subb.dd'])
  
tcpdump -i eth0 -A 'port 80'

AttributeError: people instance has no attribute 'country'
SyntaxError: invalid syntax

Python3迭代器
it=iter(字符串or列表or元组or字典)
print(next(it))
使用next(it)时如果下一个元素没有值了,那么会发生异常
import sys
while True:
    try:
        print(next(it), end=" ")
    except StopIteration:
        sys.exit()














'''

"""
三个双引号注释


"""

underlineCount = 50
beforeUnderlineCount = 25
afterUnderlineCount = 25

print("-" * beforeUnderlineCount + "sys库" + "-" * afterUnderlineCount)

# sys.path.extend(['/root/mydev/myfiles/python/PycharmProjects/StudyPython'])

print('Python version: %s' % (sys.version))
print('Python platform: %s' % (sys.platform))

print("-" * beforeUnderlineCount + "基本语法" + "-" * afterUnderlineCount)

print(operator.gt(1, 2))  # greater than（大于）
print(operator.ge(1, 2))  # greater and equal（大于等于）
print(operator.eq(1, 2))  # equal（等于）
print(operator.le(1, 2))  # less and equal（小于等于）
print(operator.lt(1, 2))  # less than（小于）

# 打印字符串
print("我是伟哥！")
# 加法运算
print(3 + 4)
score = 90
score += 100
print(score)
# %s的用法
name = "weidi"
type(name)
id(name)
print(name)
print("My name is %s" % name)
# %c的用法
name = "w"
# 用%c时变量存储的内容必须是一个字符
# 否则应用出现异常
print("My name is %c" % name)

test = name * 3
print(test)
test = name * 5
print(test)
a = 3.4
b = 5.6
c = a + b
print(c)
print("c = %f" % c)
print("c = %.3f" % c)
print("c = %lf" % c)
print("c = %.4lf" % c)
print("c = %d" % c)
c = a - b
print(c)
c = a * b
print(c)
c = a / b
print(c)
c = a % b
print(c)
# print(d) # 必须先定义再使用，不定义就使用的话报语法错误
# test=score+name # error
# print(test)
e = "e"
f = "f"
g = e + f
print(g)
e = 10
print(e)
# e="100"
# print("e = %d"%e) # error
# height=input("Please your height: ")
# print("Your height is %d"%height) # error
d1 = {"a": 31, "b": 66}
# print "The float is %(0)f." %d1
print("The float is %(a)f" % (d1))
print("The float is %(b)f" % (d1))

# def printName(name):
# 	print name

# printName("weidi")
print(sys.platform)
print(2 ** 100)
# x = 'Spam!'
x = "Spam!"
print(x)
print(x * 10)
# 打印关键字
print(keyword.kwlist)

print(5 + 3)
print(5 - 3)
print(5 * 3)
print(5 / 3)
# 商
print(5 // 3)
# 余数
print(5 % 3)

print("=" * 10 + "-" * 10)

name = "weidi"
age = 33
address = "下岩贝村"
print("%s, %d, %s" % (name, age, address))
print("%s %d %s" % (name, age, address))
# 没有任何内容被输出
# print("%s, %d, %s" %(name) %(age) %(address))

print("-" * beforeUnderlineCount + "数字(Number)" + "-" * afterUnderlineCount)

print("-" * beforeUnderlineCount + "字符串" + "-" * afterUnderlineCount)

print(len("weidi"))
name = "abcdefgABCDEFG"
print(name[0::])
print(name[::1])
print(name[-1::-1])
print(name[::-1])

print("abcd".islower())
print("abcdE".islower())

print("-" * beforeUnderlineCount + "函数" + "-" * afterUnderlineCount)


def myNeme():
    print("weidi")


# 函数调用
myNeme()

random.uniform(1, 10)
random.randint(1, 3)
int(random.uniform(1, 7))

print("-" * beforeUnderlineCount + "循环语句" + "-" * afterUnderlineCount)

for letter in 'Python':
    print(letter)

print("-" * beforeUnderlineCount + "列表" + "-" * afterUnderlineCount)

# 列表生成式
a = [i for i in range(1, 18)]
b = [11 for i in range(1, 18)]
c = [i for i in range(10) if i % 2 == 0]
d = [i for i in range(3) for j in range(2)]
d = [(i, j) for i in range(3) for j in range(2)]

fruits = ['banana', 'apple', 'mango']
for fruit in fruits:
    print(fruit)

fruits = ['banana', 'apple', 'mango']
for index in range(len(fruits)):
    print('当前水果 :', fruits[index])

range(3)
# [0, 1, 2]

for num in range(10, 20):  # 迭代 10 到 20 之间的数字
    for i in range(2, num):  # 根据因子迭代
        if num % i == 0:  # 确定第一个因子
            j = num / i  # 计算第二个因子
            print('%d 等于 %d * %d' % (num, i, j))
            break  # 跳出当前循环
    else:  # 循环的 else 部分
        print(num, '是一个质数')

sequence = [12, 34, 34, 23, 45, 76, 89]
for i, j in enumerate(sequence):
    print(i, j)

s = 'qazxswedcvfr'
for i in range(0, len(s), 2):
    print(s[i])
for (index, char) in enumerate(s):
    print("index=%s ,char=%s" % (index, char))

# 冒泡排序# 定义列表 list
arays = [1, 8, 2, 6, 3, 9, 4]
for i in range(len(arays)):
    for j in range(i + 1):
        if arays[i] < arays[j]:
            # 实现连个变量的互换
            arays[i], arays[j] = arays[j], arays[i]
print(arays)

print("-" * underlineCount)

print("-" * beforeUnderlineCount + "元组" + "-" * afterUnderlineCount)
print("-" * beforeUnderlineCount + "字典" + "-" * afterUnderlineCount)

print("-" * beforeUnderlineCount + "条件控制" + "-" * afterUnderlineCount)

age = "100"
age = int(age)
if age > 10:
    print("big")
    print("big")
    print("big")
elif age == 10:
    print("")
else:
    print("small")

print("-" * beforeUnderlineCount + "面向对象" + "-" * afterUnderlineCount)

'''
1.
self如果指向子类对象,那么打印出子类;
self如果指向父类,那么打印出父类
self.__class__.__name__
2.
子类构造方法中调用父类构造方法
Parent.__init__(self)
3.
打印Parent类的说明信息
Parent.__doc__
4.
如果要定义一个类的私有属性,
那么在这个属性前面加上两个下线线就行了,即"__",
定义私有方法也是一样的
5.
如果要访问类的私有属性,那么用"对象名._类名__属性名",
使用"类名._类名__属性名"这样也可以,只是可能得不到想要的结果
6.
__new__(cls,*args,**kwd)
__new__()在__init__()之前被调用，用于生成实例对象。
利用这个方法和类属性的特性可以实现设计模式中的单例模式。
单例模式是指创建唯一对象吗，单例模式设计的类只能实例化一个对象。
7.
__init__(self,...)
__init__方法在类的一个对象被建立时，马上运行。
这个方法可以用来对你的对象做一些你希望的初始化。
注意，这个名称的开始和结尾都是双下划线。
8.
__getattr__() __setattr__() __getattribute__()
当读取对象的某个属性时，python会自动调用__getattr__()方法。
例如，fruit.color将转换为fruit.__getattr__(color)。
当使用赋值语句对属性进行设置时，
python会自动调用__setattr__()方法。
__getattribute__()的功能与__getattr__()类似，
用于获取属性的值。但是__getattribute__()
能提供更好的控制，代码更健壮。
注意，python中并不存在__setattribute__()方法。
Python不允许实例化的类访问私有数据，
但你可以使用object._className__attrName访问这些私有属性。
9.
__getitem__()
如果类把某个属性定义为序列，可以使用__getitem__()
输出序列属性中的某个元素.假设水果店中销售多钟水果，
可以通过__getitem__()方法获取水果店中的没种水果。
10.
__str__()
__str__()用于表示对象代表的含义，返回一个字符串.
实现了__str__()方法后，可以直接使用print语句
输出对象，也可以通过函数str()触发__str__()的执行。
这样就把对象和字符串关联起来，便于某些程序的实现，
可以用这个字符串来表示某个类。
11.
__call__()
在类中实现__call__()方法，可以在对象创建时
直接返回__call__()的内容。使用该方法可以模拟静态方法。

 内置方法
 __new__(cls, *args, **kwd)	 
 实例的生成操作
 __init__(self,...)	 
 初始化对象，在创建新对象时调用
 __del__(self)	 
 释放对象，在对象被删除之前调用
 __str__(self)	 
 在使用print语句时被调用
 __getitem__(self, key)	 
 获取序列的索引key对应的值，等价于seq[key]
 __len__(self)	 
 在调用内联函数len()时被调用
 __cmp__(stc, dst)	 
 比较两个对象src和dst
 __getattr__(s, name)	 
 获取属性的值
 __setattr__(s, name,value)	 
 设置属性的值
 __delattr__(s, name)	 
 删除name属性
 __getattribute__()	 
 __getattribute__()功能与__getattr__()类似
 __call__(self,*args)	 
 把实例对象作为函数调用
 __gt__(self, other)	 判断self对象是否大于other对象
 __lt__(slef, other)	 判断self对象是否小于other对象
 __ge__(slef, other)	 判断self对象是否大于或者等于other对象
 __le__(slef, other)	 判断self对象是否小于或者等于other对象
 __eq__(slef, other)	 判断self对象是否等于other对象

类属性
就是类对象所拥有的属性，
它被所有类对象的实例对象所共有，
在内存中只存在一个副本。
对于公有的类属性，在类外可以通过类对象和实例对象访问。
总结：一句话，只要这个类属性是公有的，那么类对象和实例对象都可以访问。
class people:  
    name = 'jack'  #公有的类属性  
    __age = 12     #私有的类属性

p = people()
print p.name             #正确  
print people.name        #正确  
print p.__age            #错误，不能在类外通过实例对象访问私有的类属性  
print people.__age       #错误，不能在类外通过类对象访问私有的类属性 

实例属性
是不需要在类中显示定义的。
总结：一句话，只能由实例对象创建，并且调用。
第一种方式：
class people:  
    name = 'jack' 
 
p = people()  
p.age =12  
print p.name         #正确  
print p.age          #正确  
print people.name    #正确  
print people.age     #错误

第二种方式：
class people:  
    name = 'jack'  
    # __init__()是内置的构造方法，在实例化对象时自动调用  
    def __init__(self, age):  
        self.age = age  
  
p = people(12)  
print p.name         #正确  
print p.age          #正确  
print people.name    #正确  
print people.age     #错误

如果需要在类外修改类属性，
必须通过类对象去引用然后进行修改。
如果通过实例对象去引用，会产生一个同名的实例属性，
这种方式修改的是实例属性，不会影响到类属性，
并且之后如果通过实例对象去引用该名称的属性，
实例属性会强制屏蔽掉类属性，即引用的是实例属性，
除非删除了该实例属性。
class people:  
    country = 'china'

print people.country  
p = people()  
print p.country  
p.country = 'japan'   
print p.country      #实例属性会屏蔽掉同名的类属性  
print people.country  
del p.country        #删除实例属性  
print p.country 

类方法
是类对象所拥有的方法，
需要用修饰器"@classmethod"来标识其为类方法，
对于类方法，第一个参数必须是类对象，
一般以"cls"作为第一个参数
（当然可以用其他名称的变量作为其第一个参数，
但是大部分人都习惯以'cls'作为第一个参数的名字，
就最好用'cls'了），
能够通过实例对象和类对象去访问。
class people:  
    country = 'china'  
    # 类方法，用classmethod来进行修饰  
    @classmethod  
    def getCountry(cls):  
        return cls.country   
    @classmethod  
    def setCountry(cls, country): 
        # 用类对象去修改类属性时，以后由这个类对象或者所有其实例对象
        # 调用那个属性时，值都会发生变化。除非实例对象对这个属性重新赋值。
        cls.country = country 

p = people()  
print p.getCountry()         #可以用过实例对象引用  
print people.getCountry()    #可以通过类对象引用  
p.setCountry('japan')     
print p.getCountry()     
print people.getCountry() 

实例方法
在类中最常定义的成员方法，
它至少有一个参数并且必须以实例对象作为其第一个参数，
一般以名为'self'的变量作为第一个参数
（当然可以以其他名称的变量作为第一个参数）。
在类外实例方法只能通过实例对象去调用，不能通过其他方式去调用。
总结：一句话，只能由实例对象调用。
class people:  
    country = 'china'  
    # 实例方法  
    def getCountry(self):  
        return self.country  

p = people()  
print p.getCountry()         #正确，可以用过实例对象引用  
print people.getCountry()    #错误，不能通过类对象引用实例方法

静态方法
需要通过修饰器"@staticmethod"来进行修饰，静态方法不需要多定义参数。
总结：一句话，类对象和实例对象都可以调用。
class people:  
    country = 'china'  
    # 静态方法  
    @staticmethod  
    def getCountry():  
        return people.country  

p = people()  
print p.getCountry()
print people.getCountry() 

对于类属性和实例属性，
如果在类方法中引用某个属性，
该属性必定是类属性，
而如果在实例方法中引用某个属性（不作更改），
并且存在同名的类属性，
此时若实例对象有该名称的实例属性，
则实例属性会屏蔽类属性，
即引用的是实例属性，
若实例对象没有该名称的实例属性，
则引用的是类属性；
如果在实例方法更改某个属性，
并且存在同名的类属性，
此时若实例对象有该名称的实例属性，
则修改的是实例属性，
若实例对象没有该名称的实例属性，
则会创建一个同名称的实例属性。
想要修改类属性，如果在类外，
可以通过类对象修改，如果在类里面，
只有在类方法中进行修改。

从类方法和实例方法以及静态方法的定义形式就可以看出来，
类方法的第一个参数是类对象cls，
那么通过cls引用的必定是类对象的属性和方法；
而实例方法的第一个参数是实例对象self，
那么通过self引用的可能是类属性、
也有可能是实例属性（这个需要具体分析），
不过在存在相同名称的类属性和实例属性的情况下，
实例属性优先级更高。
静态方法中不需要额外定义参数，
因此在静态方法中引用类属性的话，
必须通过类对象来引用。
'''


class Parent:
    """
    Parent类
    """
    parentAttr = 100

    def __init__(self):
        print(self.__class__.__name__ + " invoke Parent class Constructor")

    def parentMethod(self):
        print(self.__class__.__name__ + " invoke Parent class Method")

    def setAttr(self, attr):
        self.parentAttr = attr
        print(self.__class__.__name__ + " invoke Parent class Method")

    def getAttr(self):
        print("Parent class Attr:", self.parentAttr)


class Child(Parent):
    '''
    Child类
    '''

    def __init__(self):
        Parent.__init__(self)
        print("invoke Child class Constructor")

    def childMethod(self):
        print("invoke Child class Method")


c = Child()
c.childMethod()
c.parentMethod()
c.setAttr(200)
c.getAttr()
print(Parent.__doc__)
print(Child.__doc__)

print(issubclass(Child, Parent))
print(isinstance(c, Parent))
print(isinstance(c, Child))


class Vector:

    def __init__(self, a, b):
        print(self.__class__.__name__ + " __init__: a = %d, b = %d" % (a, b))
        self.a = a
        self.b = b

    def __del__(self):
        # print("__del__: ", self.__class__.__name__)
        pass

    def __str__(self):
        return "Vector (%d, %d)" % (self.a, self.b)

    def __add__(self, other):
        print(self.__class__.__name__ + " __add__")
        return Vector(self.a + other.a, self.b + other.b)


v1 = Vector(2, 10)
v2 = Vector(5, -2)
# 1.__add__
# 2.__init__
# 3.__str__
print(v1 + v2)


class JustCounter:
    __secretCount = 0
    publicCount = 0

    def count(self):
        self.__privateCount()
        self.__secretCount += 1
        self.publicCount += 1
        print(self.__secretCount)

    def __privateCount(self):
        self.__secretCount = 100


counter = JustCounter()
counter.count()
counter.count()
print(counter.publicCount)
# 这样还是可以读取私有属性的值
print(counter._JustCounter__secretCount)
print(JustCounter._JustCounter__secretCount)


class A:
    def foo(self):
        print(self.__class__.__name__ + " invoke A class foo()")


class B(A):
    def foo(self):
        print(self.__class__.__name__ + " invoke B class foo()")

    pass


class C(A):
    def foo(self):
        print(self.__class__.__name__ + " invoke C class foo()")


# B与C及其父类中都有同名方法且同参数时,
# 子类对象调用这个方法时会调用排在最前面的那个类的方法
class D(C, B, object):
    pass


class E(C, B):
    pass


class F(C, object):
    pass


d = D()
d.foo()
e = E()
e.foo()
f = F()
f.foo()

print("-" * beforeUnderlineCount + "Gui" + "-" * afterUnderlineCount)

# 弹出一个窗口
# top = tkinter.Tk()
# top.mainloop()


print("-" * beforeUnderlineCount + "列表" + "-" * afterUnderlineCount)

'''
序列是Python中最基本的数据结构。
列表是最常用的Python数据类型，它可以作为一个方括号内的逗号分隔值出现。
列表的数据项不需要具有相同的类型。
list1 = ['Google', 'Runoob', 1997, 2000];
list2 = [1, 2, 3, 4, 5 ];
list3 = ["a", "b", "c", "d"];
列表对 + 和 * 的操作符与字符串相似。+ 号用于组合列表，* 号用于重复列表。
访问列表中的元素时下标不能越界，否则抛异常
'''
# 访问列表元素
list1 = ["Google", "Runoob", 1997, 2000]
list2 = [1, 2, 3, 4, 5, 6, 7]
print("list1[0]: ", list1[0])
print("list2[1:5]: ", list2[1:5])
# 更新列表元素
print(list1)
list1[2] = 2001
print(list1)
# 删除列表元素
del list1[2]
print(list1)
# del一个对象后，就不能再对这个对象进行任何的操作了
# del list1
# print(list1)
print(["Hi!" * 2] * 5)
print(3 in [1, 2, 3])
print(0 in [1, 2, 3])
for x in [1, 2, 3]: print(x, end="!\n")
# print("\n")
tempList = [1, 4, 9, 16, 25] + [36, 49, 64, 81, 100]
print(tempList)
print(len(tempList))
print(max(tempList))
print(min(tempList))
print(list(("1", 2, 1.8)))
# 切片，不包括下标为5的那个元素
print(tempList[1:5])
'''
list[start:end:step]
1.step不能为0，必须是整数
2.step大于0时从左往右取值（默认为1，取值方向为从左往右）
3.step小于0时从右往左取值，-1表示倒数第一个数，-2表示倒数第二个数
4.start与end的范围之间有值才能输出元素，但不包括下标为end的这个元素
如tempList[5:-4] 下标为5的元素是36，下标为-4的元素是49，因此输出为36
5.当step大于0时，start的取值范围是从start这个位置开始,
从左往右取到最后的数,end的取值范围是从左往右取到end这个元素
的前一个数,然后把两者的取值进行交集运算,就是结果.
      tempList[5:-1:1]
  5-1.tempList[5::1]  ---> [36, 49, 64, 81, 100]
  5-2.tempList[:-1:1] ---> [1, 4, 9, 16, 25, 36, 49, 64, 81]
  5-3.tempList[5:-1:1]---> [36, 49, 64, 81]
6.当step小于0时,start的取值范围是从start这个位置开始,
从右往左取到第一个数,end的取值范围是从右往左取到end这个元素
的后一个数,然后把两者的取值进行交集运算,就是结果.
      tempList[5:-2:-1]
  6-1.tempList[5::-1]  ---> [36, 25, 16, 9, 4, 1]
  6-2.tempList[:-2:-1] ---> [100]
  6-3.tempList[5:-2:-1]---> []
'''

print("-" * beforeUnderlineCount + "range" + "-" * afterUnderlineCount)

'''
Python3 range() 函数返回的是一个可迭代对象（类型是对象），
而不是列表类型， 所以打印的时候不会打印列表。
Python3 list() 函数是对象迭代器，可以把range()
返回的可迭代对象转为一个列表，返回的变量类型为列表。

range(stop)
range(start, stop[, step])
start: 计数从 start 开始。默认是从 0 开始。
stop: 计数到 stop 结束，但不包括 stop。
step：步长，默认为1，可以为正整数或者负整数，不能为0。
'''
print(range(10))
print(range(0, 10))
print(range(0, 10, 2))
print(list(range(10)))
print(list(range(0, 10)))
print(list(range(0, 10, 2)))
print(list(range(0, -10, -2)))
print(list(range(1, -1, -1)))

print("-" * beforeUnderlineCount + "os库" + "-" * afterUnderlineCount)

'''
import os

os.environ 一个dictionary 包含环境变量的映射关系   
os.environ["HOME"] 可以得到环境变量HOME的值     
注意windows下用到转义     
os.chdir(dir) 改变当前目录 os.chdir('d:\\outlook')   
os.getcwd() 得到当前目录     
os.getegid() 得到有效组id os.getgid() 得到组id     
os.getuid() 得到用户id os.geteuid() 得到有效用户id     
os.setegid os.setegid() os.seteuid() os.setuid()     
os.getgruops() 得到用户组名称列表     
os.getlogin() 得到用户登录名称     
os.getenv 得到环境变量     
os.putenv 设置环境变量     
os.umask 设置umask     
os.system(cmd) 利用系统调用，运行cmd命令 
'''

print("-" * beforeUnderlineCount + "shutil库" + "-" * afterUnderlineCount)

'''
针对日常的文件和目录管理任务
import shutil

shutil.copyfile('source_file_path', 'target_file_path')
shutil.move('source_file_path', 'target_file_path')
'''

print("-" * beforeUnderlineCount + "sys库" + "-" * afterUnderlineCount)

'''
import sys

sys.argv 是一个 list,包含所有的命令行参数.    
sys.stdout sys.stdin sys.stderr 分别表示标准输入输出,错误输出的文件对象.    
sys.stdin.readline() 从标准输入读一行 sys.stdout.write("a") 屏幕输出a    
sys.exit(exit_code) 退出程序    
sys.modules 是一个dictionary，表示系统中所有可用的module    
sys.platform 得到运行的操作系统环境    
sys.path 是一个list,指明所有查找module，package的路径.
'''

print("-" * beforeUnderlineCount + "sys库" + "-" * afterUnderlineCount)

'''
用于从目录通配符搜索中生成文件列表
import glob

glob.glob('*.py')
['primes.py', 'random.py', 'quote.py']
'''

print("-" * beforeUnderlineCount + "内置函数" + "-" * afterUnderlineCount)

'''
help(obj) 在线帮助, obj可是任何类型    
callable(obj) 查看一个obj是不是可以像函数一样调用    
repr(obj) 得到obj的表示字符串，可以利用这个字符串eval重建该对象的一个拷贝    
eval_r(str) 表示合法的python表达式，返回这个表达式    
dir(obj) 查看obj的name space中可见的name    
hasattr(obj,name) 查看一个obj的name space中是否有name    
getattr(obj,name) 得到一个obj的name space中的一个name    
setattr(obj,name,value) 为一个obj的name   
space中的一个name指向vale这个object    
delattr(obj,name) 从obj的name space中删除一个name    
vars(obj) 返回一个object的name space。用dictionary表示    
locals() 返回一个局部name space,用dictionary表示    
globals() 返回一个全局name space,用dictionary表示    
type(obj) 查看一个obj的类型    
isinstance(obj,cls) 查看obj是不是cls的instance    
issubclass(subcls,supcls) 查看subcls是不是supcls的子类 

类型转换
chr(i) 把一个ASCII数值,变成字符    
ord(i) 把一个字符或者unicode字符,变成ASCII数值    
oct(x) 把整数x变成八进制表示的字符串    
hex(x) 把整数x变成十六进制表示的字符串    
str(obj) 得到obj的字符串描述    
list(seq) 把一个sequence转换成一个list    
tuple(seq) 把一个sequence转换成一个tuple    
dict(),dict(list) 转换成一个dictionary    
int(x) 转换成一个integer    
long(x) 转换成一个long interger    
float(x) 转换成一个浮点数    
complex(x) 转换成复数    
max(...) 求最大值    
min(...) 求最小值
'''

print("-" * beforeUnderlineCount + "飞机大战" + "-" * afterUnderlineCount)

pygame.init()

# 创建一个窗口，用来显示内容
screen = pygame.display.set_mode((480, 852), 0, 32)
# 创建一个和窗口大小一样的图片，用来充当背景
background = pygame.image.load('/root/Pictures/500px/images/249358865.jpg')

hero = pygame.image.load('/root/Pictures/drawable-hdpi/actionbar_dlna_connected.png')

x = 210
y = 700

while 1:
    # 把图片加载到内存中
    screen.blit(background, (0, 0))

    screen.blit(hero, (x, y))

    # 把内存里的东西显示到屏幕上去
    pygame.display.update()

    for event in pygame.event.get():

        if event.type == QUIT:
            print('exit')
            exit(0)
        elif event.type == KEYDOWN:
            print(str(event.key))
            # if event.key == K_a or event.key == K_LEFT:
            #     print('left')
            # elif event.key == K_d or event.key == K_RIGHT:
            #     print('right')
            # elif event.key == K_SPACE:
            #     print('space')

    time.sleep(0.01)
    pass

print("-" * beforeUnderlineCount + "urlopen" + "-" * afterUnderlineCount)

'''
# 处理get请求，不传data，则为get请求
url = 'http://www.xxx.com/login'
data = {"username": "admin", "password": 123456}
# 将字典类型的请求数据转变为url编码
req_data = urlencode(data)
# 通过urlopen方法访问拼接好的url
res = urlopen(url + '?' + req_data)
# read()方法是读取返回数据内容，decode是转换返回数据的bytes格式为str
res = res.read().decode()
print(res)

# 处理post请求,如果传了data，则为post请求
# 将字典类型的请求数据转变为url编码
data = urlencode(data)
# 将url编码类型的请求数据转变为bytes类型
data = data.encode('ascii')
# 将url和请求数据处理为一个Request对象，供urlopen调用
req_data = Request(url, data)
with urlopen(req_data) as res:
    # read()方法是读取返回数据内容，decode是转换返回数据的bytes格式为str
    res = res.read().decode()
print(res)
'''

print("-" * beforeUnderlineCount + "GeoLiteCity" + "-" * afterUnderlineCount)

gi = pygeoip.GeoIP('/root/mydev/tools/geolitecity/geolite_city/GeoLiteCity.dat')


def printRecord(tgt):
    rec = gi.record_by_name(tgt)
    '''
    {
    'country_name': 'United States', 
    'city': 'Newark', 
    'metro_code': 'New York, NY', 
    'dma_code': 501, 
    'time_zone': 'America/New_York', 
    'country_code3': 'USA',
    'postal_code': '07175', 
    'area_code': 973, 
    'country_code': 'US', 
    'region_code': 'NJ', 
    'longitude': -74.1724, 
    'latitude': 40.73570000000001, 
    'continent': 'NA'
    }
    '''
    city = rec['city']
    country_name = rec['country_name']
    # 经度
    longitude = rec['longitude']
    # 纬度
    latitude = rec['latitude']
    print(rec)
    print('[*] Target: ' + tgt + ' Geo-located.')
    print('[+] ' + str(city) + ', ' + str(country_name))
    print('[+] Longitude: ' + str(longitude) + ', Latitude: ' + str(latitude))


def retGeoStr(ip):
    try:
        rec = gi.record_by_name(ip)
        city = rec['city']
        country = rec['country_code3']
        if (city != ''):
            geoLoc = city + ', ' + country
        else:
            geoLoc = country
        return geoLoc
    except:
        return 'Unregistered'


# tgt = '173.255.226.98'
'''
{
'area_code': 0, 
'continent': 'AS', 
'country_code': 'JP', 
'city': 'Tokyo', 
'country_code3': 'JPN', 
'postal_code': '102-0082', 
'metro_code': None, 
'time_zone': 'Asia/Tokyo', 
'longitude': 139.7514, 
'dma_code': 0, 
'region_code': '40', 
'country_name': 'Japan', 
'latitude': 35.685
}
'''
tgt = '43.82.112.120'
printRecord(tgt)

print('\n')

# def printPcap(pcap):
#     for (ts, buf) in pcap:
#         try:
#             eth = dpkt.ethernet.Ethernet(buf)
#             ip = eth.data
#             src = socket.inet_ntoa(ip.src)
#             dst = socket.inet_ntoa(ip.dst)
#             print('[+] Src: ' + src + ' ---> Dst: ' + dst)
#         except:
#             pass
#
#
# f = open('geotest.pcap')
# pcap = dpkt.pcap.Reader(f)
# printPcap(pcap)

print("-" * beforeUnderlineCount + "网络编程" + "-" * afterUnderlineCount)

