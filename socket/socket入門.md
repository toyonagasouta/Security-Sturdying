# socket入門

## ステップ0: 環境準備

- Python 3系がインストールされていればOK(標準の`socket`モジュールのみで十分)
- ターミナルを2つ開けるようにしておく(サーバ用・クライアント用)
- 全て `localhost` (`127.0.0.1`) で完結させるので、ネットワーク設定は不要です

## ステップ1: 最小のエコーサーバ(まずはこれだけ)

**目標**:クライアントが送った文字列を、そのままオウム返しするサーバを作る。

構成は2ファイル:`server.py`と`client.py`

まずサーバ側の骨組み(先ほどのHOWTOの内容そのまま):

```python
import socket

HOST = '127.0.0.1'
PORT = 5000  # 4桁のポートを使う

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((HOST, PORT))
serversocket.listen(1)
print(f"Listening on {HOST}:{PORT}...")

conn, addr = serversocket.accept()
print(f"Connected by {addr}")

data = conn.recv(1024)
print(f"Received: {data}")

conn.sendall(data)  # そのまま送り返す

conn.close()
serversocket.close()
```

クライアント側:

```python
import socket

HOST = '127.0.0.1'
PORT = 5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(b"Hello, socket world!")

data = s.recv(1024)
print(f"Received back: {data}")

s.close()
```

**やること**:
1. ターミナルAで `python server.py` を実行(待機状態になる)
2. ターミナルBで `python client.py` を実行
3. サーバ側にメッセージが届き、クライアント側にオウム返しが届くのを確認

ここで`recv(1024)`の挙動、`sendall`と`send`の違い(`sendall`は全部送り切るまでループしてくれる)などを実際に`print`デバッグしながら体感してください。

## ステップ2: ループ化して何度もやり取りする

1回きりだと味気ないので、`while True`で会話を継続させてみましょう。ここで前回説明した「メッセージの区切り」問題に自然とぶつかります(例:相手が`recv`し続けて終わらない、など)。

- クライアントが `"quit"` と送ったら双方が終了する、といったシンプルなプロトコルを自分で設計してみる

## ステップ3: マルチクライアント対応(select or threading)

1つのクライアントとしか話せないサーバを、複数クライアントと同時に会話できるように改造します。

- まずは `threading` を使う方式(`accept`のたびに新しいスレッドを立てる)の方が理解しやすいのでお勧めです
- 慣れたら前回説明した `select` を使ったノンブロッキング方式に挑戦

## ステップ4: 固定長→長さヘッダ方式への発展

- 最初は固定長メッセージ(例:常に1024バイト分待つ)で実装
- 次に「先頭4バイトでメッセージ長を送り、その後本文を送る」方式に変更してみる(`struct`モジュールを使ったパッキングの練習になります)

## つまずいたときのデバッグのコツ

- `recv`が返ってこない(ハングする)→ 送信側が`sendall`し忘れている、または受信側が期待するバイト数と送信側の実際のバイト数が食い違っている可能性が高いです
- `Address already in use`エラー→ 直前のサーバプロセスがまだポートを掴んだまま。`serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)` を`bind`前に入れると解決することが多いです

## 次の目標地点

ステップ1〜4まで一通り自分の手で動かせるようになったら、次は「バイナリデータの送受信」(`struct.pack`/`struct.unpack`)に進むと、以前お話ししたExploit開発文脈(ペイロードのパッキング)に自然につながっていきます。

まずはステップ1のエコーサーバから始めてみてください。良いスタートです。段階を踏んで、小さく動くものを積み重ねていくのが一番挫折しにくいです。具体的なロードマップを示します。

## ステップ0: 環境準備

- Python 3系がインストールされていればOK(標準の`socket`モジュールのみで十分)
- ターミナルを2つ開けるようにしておく(サーバ用・クライアント用)
- 全て `localhost` (`127.0.0.1`) で完結させるので、ネットワーク設定は不要です

## ステップ1: 最小のエコーサーバ(まずはこれだけ)

**目標**:クライアントが送った文字列を、そのままオウム返しするサーバを作る。

構成は2ファイル:`server.py`と`client.py`

まずサーバ側の骨組み(先ほどのHOWTOの内容そのまま):

```python
import socket

HOST = '127.0.0.1'
PORT = 5000  # 4桁のポートを使う

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.bind((HOST, PORT))
serversocket.listen(1)
print(f"Listening on {HOST}:{PORT}...")

conn, addr = serversocket.accept()
print(f"Connected by {addr}")

data = conn.recv(1024)
print(f"Received: {data}")

conn.sendall(data)  # そのまま送り返す

conn.close()
serversocket.close()
```

クライアント側:

```python
import socket

HOST = '127.0.0.1'
PORT = 5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.sendall(b"Hello, socket world!")

data = s.recv(1024)
print(f"Received back: {data}")

s.close()
```

**やること**:
1. ターミナルAで `python server.py` を実行(待機状態になる。実行方法はターミナルでpython3 server.pyを入力)
2. ターミナルBで `python client.py` を実行(実行方法はターミナルでpython3 server.pyを入力)   
3. サーバ側にメッセージが届き、クライアント側にオウム返しが届くのを確認

ここで`recv(1024)`の挙動、`sendall`と`send`の違い(`sendall`は全部送り切るまでループしてくれる)などを実際に`print`デバッグしながら体感してください。

## ステップ2: ループ化して何度もやり取りする

1回きりだと味気ないので、`while True`で会話を継続させてみましょう。ここで前回説明した「メッセージの区切り」問題に自然とぶつかります(例:相手が`recv`し続けて終わらない、など)。

- クライアントが `"quit"` と送ったら双方が終了する、といったシンプルなプロトコルを自分で設計してみる

## ステップ3: マルチクライアント対応(select or threading)

1つのクライアントとしか話せないサーバを、複数クライアントと同時に会話できるように改造します。

- まずは `threading` を使う方式(`accept`のたびに新しいスレッドを立てる)の方が理解しやすいのでお勧めです
- 慣れたら前回説明した `select` を使ったノンブロッキング方式に挑戦

## ステップ4: 固定長→長さヘッダ方式への発展

- 最初は固定長メッセージ(例:常に1024バイト分待つ)で実装
- 次に「先頭4バイトでメッセージ長を送り、その後本文を送る」方式に変更してみる(`struct`モジュールを使ったパッキングの練習になります)

## つまずいたときのデバッグのコツ

- `recv`が返ってこない(ハングする)→ 送信側が`sendall`し忘れている、または受信側が期待するバイト数と送信側の実際のバイト数が食い違っている可能性が高いです
- `Address already in use`エラー→ 直前のサーバプロセスがまだポートを掴んだまま。`serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)` を`bind`前に入れると解決することが多いです

## 次の目標地点

ステップ1〜4まで一通り自分の手で動かせるようになったら、次は「バイナリデータの送受信」(`struct.pack`/`struct.unpack`)に進むと、以前お話ししたExploit開発文脈(ペイロードのパッキング)に自然につながっていきます。

まずはステップ1のエコーサーバから始めてみてください。動いたら、疑問点や次に詰まった箇所を教えてもらえれば、その都度一緒に深掘りしていきましょう。