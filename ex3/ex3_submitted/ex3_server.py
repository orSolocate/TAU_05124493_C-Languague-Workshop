#!/usr/bin/python2.7 -tt
import sys
from socket import *

MAX_REQUEST_SIZE = 1024

port = int(sys.argv[1])
socket = socket()
socket.connect(('localhost', port))
numOfRequests = 0

while True:
	receivedRequest = ''
	while not receivedRequest.count('\r\n\r\n'):
		receivedRequest += socket.recv(MAX_REQUEST_SIZE)

	requestAfterGET = receivedRequest.split('GET')[1]
	requestAddress = requestAfterGET.split()[0]
	if requestAddress == '/counter':
		numOfRequests += 1
		answer = 'HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nContent-Length: {}\r\n\r\n{}\r\n\r\n'\
				.format(str(len(str(numOfRequests))), str(numOfRequests))
	else:
		answer = 'HTTP/1.1 404 Not Found\r\n'\
			 'Content-type: text/html\r\n'\
			 'Content-length: 113\r\n\r\n'\
			 '<html><head><title>Not Found</title></head><body>\r\n'\
			 'Sorry, the object you requested was not found.\r\n'\
			 '</body></html>\r\n\r\n'
	socket.send(answer)

