import socket

HOST = '127.0.0.1'
PORT = 5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    msg = input("Enter message (or 'quit' to exit):")
    s.sendall(msg.encode())
    if msg == "quit":
        break
    data = s.recv(1024)
    print(f"Received back: {data}")

s.close()