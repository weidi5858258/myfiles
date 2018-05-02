# -*- coding:utf-8 -*-

import threading
from socket import *

HOST = "localhost"
PORT = 21567
BUFSIZE = 1024
ADDR = (HOST, PORT)



class Send(threading.Thread):

	def __init__(self, tcpCliSock_):
		threading.Thread.__init__(self)
		self.tcpCliSock = tcpCliSock_

	def run(self):
		testSend(self.tcpCliSock)

def testSend(tcpCliSock):
	print("testSend() ", tcpCliSock)
	print("\n")
	while 1:
		data = input("> ")
		if not data:
			print("testSend() break")
			break
		print("testSend2() ", tcpCliSock)
		tcpCliSock.send(bytes(data, "utf-8"))

class Recv(threading.Thread):

	def __init__(self, tcpCliSock_):
		threading.Thread.__init__(self)
		self.tcpCliSock = tcpCliSock_

	def run(self):
		testRecv(self.tcpCliSock)

def testRecv(tcpCliSock):
	while 1:
		print("testRecv() ", tcpCliSock)
		data = tcpCliSock.recv(BUFSIZE)
		if not data:
			break
		print("服务端: %s" % (str(data)))

tcpCliSock = socket(AF_INET, SOCK_STREAM)
tcpCliSock.connect(ADDR)
print(tcpCliSock)

# while 1:
# 	data = input("> ")
# 	if not data:
# 		break
# 	tcpCliSock.send(bytes(data, "utf-8"))
# 	data = tcpCliSock.recv(BUFSIZE)
# 	if not data:
# 		break
# 	print("服务端: %s" % (str(data)))

# send_ = Send(tcpCliSock)
# recv_ = Recv(tcpCliSock)
# for t_ in [send_, recv_]:
#     t_.start()

testSend = threading.Thread(target=testSend, args=(tcpCliSock,))
testRecv = threading.Thread(target=testRecv, args=(tcpCliSock,))
testSend.start()
testRecv.start()

# testSend(tcpCliSock)

tcpCliSock.close()