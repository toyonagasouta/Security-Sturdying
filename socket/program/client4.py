import socket
import struct

HOST = '127.0.0.1'
PORT = 5000

def recv_exactly(sock, n):
    buf = b''
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:
            return b''
        buf += chunk
    return buf

def recv_msg(sock):
    length_prefix = recv_exactly(sock,4)
    if not length_prefix:
        return None
    (length,) = struct.unpack('!I', length_prefix)
    return recv_exactly(sock, length)

def send_msg(sock, data: bytes):
    sock.sendall(struct.pack('!I', len(data)) + data)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    msg = input("Enter message (or 'quit' to exit): ")
    if msg == "":
        print("Empty message, plese enter something.")
        continue
    
    send_msg(s, msg.encode())
    if msg == "quit":
        break
    
    data = recv_msg(s)
    if data is None:
        print("Server closed the connection.")
        break
    print(f"Received back {len(data)} bytes:{data}")

s.close()
