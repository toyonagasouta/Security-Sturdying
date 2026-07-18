import socket
import threading

HOST = '127.0.0.1'
PORT = 5000

def handle_client(conn, addr):
    """1つのクライアント接続を処理する関数"""
    print(f"[+] Connected by {addr}")
    try:
        while True:
            data = conn.recv(1024)
            if not data or data == b"quit":
                print(f"[-] {addr} disconnected.")
                break
            print(f"[{addr}] Received:{data}")
            conn.sendall(data)
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
        t =threading.Thread(target=handle_client, args=(conn, addr))
        t.daemon = True #メインプログラム終了時にスレッドも終了するようにする。
        t.start()
except KeyboardInterrupt:
    print("\nShutting down server.")
finally:
    serversocket.close()