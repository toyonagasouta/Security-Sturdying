import socket

HOST = '127.0.0.1'
PORT = 5000

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((HOST, PORT))
serversocket.listen(1)
print(f"Listening on {HOST}:{PORT}")

conn, addr = serversocket.accept()
print(f"Connected by {addr}")

data = conn.recv(1024)
print(f"Received: {data}")

conn.sendall(data)

conn.close()
serversocket.close()
