import socket

HOST = '127.0.0.1'
PORT = 5000

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) #デバック時の再起動が楽になる。
serversocket.bind((HOST, PORT))
serversocket.listen(1)

print(f"Listening on {HOST}:{PORT}")

conn, addr = serversocket.accept()
print(f"Connected by {addr}")

while True:
    data = conn.recv(1024)
    if not data or data == b"quit":
        print("Closing connection.")
        break
    print(f"Received: {data}")
    conn.sendall(data)

conn.close()
serversocket.close()
