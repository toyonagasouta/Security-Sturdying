import socket
import threading
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

def recv_message(sock):
    length_prefix = recv_exactly(sock, 4)
    if not length_prefix:
        return None
    (length,) = struct.unpack('!I', length_prefix)
    return recv_exactly(sock, length)

def send_msg(sock,data: bytes):
    sock.sendall(struct.pack('!I', len(data)) + data)
    
def handle_client(conn, addr):
    print(f"[+] Connected by {addr}")
    try:
        while True:
            data = recv_message(conn)
            if not data or data == b"quit":
                print(f"[-] Connection closed by {addr}")
                break
            print(f"[{addr}] Received: {len(data)} bytes: {data}")
            send_msg(conn, data)
    finally:
        conn.close()

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
serversocket.bind((HOST, PORT))
serversocket.listen(5)
print(f"Listening on {HOST}:{PORT}")

try:
    while True:
        conn, addr = serversocket.accept()
        t = threading.Thread(target=handle_client, args=(conn, addr))
        t.daemon = True
        t.start()
except KeyboardInterrupt:
    print("\n[!] Server shutting down.")
finally:
    serversocket.close()
        
            