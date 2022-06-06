import socket
import sys

port = int(sys.argv[1])                   # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', port))
s.sendall(b'Hello, world')
data = s.recv(1024)
s.close()
print('Received', repr(data))