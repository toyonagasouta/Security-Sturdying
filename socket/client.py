import socket

HOST = '127.0.0.1'
PORT =5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(b"hello.socket world!")

data = s.recv(1024)
print(f"Received back: {data}")

s.close()