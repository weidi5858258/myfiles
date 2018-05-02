# -*- coding:utf-8 -*-

import threading
from socket import *
from time import ctime

# 表示可以使用任何可用的地址
HOST = ""
PORT = 21567
BUFSIZE = 1024
ADDR = (HOST, PORT)
MAX_LISTEN = 5

# AF_INET(IPv4) AF_INET6(IPv6)
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
# 在连接被转接或拒绝之前,传入连接请求的最大数
tcpSerSock.listen(MAX_LISTEN)

# class Send(threading.Thread):

# 	def __init__(self, tcpCliSock):
# 		threading.Thread.__init__(self)
# 		self.tcpCliSock = tcpCliSock

# 	def run(self):
# 		testSend(self.tcpCliSock)

# def testSend(tcpCliSock):
# 	print("testSend() ", tcpCliSock)
# 	while 1:
# 		data = input("> ")
# 		if not data:
# 			break
# 		tcpCliSock.send(bytes(data, "utf-8"))

# class Recv(threading.Thread):

# 	def __init__(self, tcpCliSock):
# 		threading.Thread.__init__(self)
# 		self.tcpCliSock = tcpCliSock

# 	def run(self):
# 		testRecv(self.tcpCliSock)

# def testRecv(tcpCliSock):
# 	print("testRecv() ", tcpCliSock)
# 	while 1:
# 		data = tcpCliSock.recv(BUFSIZE)
# 		if not data:
# 			break
# 		print("客户端: %s" % (str(data)))

while 1:
	print("waiting for connection...")
	tcpCliSock, addr = tcpSerSock.accept()
	print("...connected from: ", tcpCliSock)
	print("...connected from: ", addr)

	# recv_ = Recv(tcpCliSock)
	# send_ = Send(tcpCliSock)
	# for t_ in [recv_, send_]:
	#     t_.start()

	while 1:
		data = tcpCliSock.recv(BUFSIZE)
		if not data:
			print("break")
			break
		print("客户端: %s" %(str(data)))
		# tcpCliSock.send("[%s] %s" % (bytes(ctime(), "utf-8"), data))
		tcpCliSock.send(bytes(ctime(), "utf-8"))
	
	tcpCliSock.close()

tcpSerSock.close()