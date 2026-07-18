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

もちろん手伝います。このロードマップは、Pythonの標準的なソケットプログラミングを段階的に学ぶ、良い構成になっています。すでにステップ1(基本のエコーサーバ/クライアント)は理解できているので、次はステップ2「ループ化」に進みましょう。

## ステップ2:会話を継続させる

現状は「1回送って1回返す」で終わりですが、これを`while True`でループさせ、複数回やり取りできるようにします。ポイントは以下の2つです。

1. **サーバ側**:`recv`→`sendall`を繰り返す。クライアントが特定のメッセージ(例:`"quit"`)を送ってきたらループを抜けて接続を閉じる
2. **クライアント側**:ユーザーに`input()`でメッセージを入力させ、それをサーバに送る。`"quit"`と入力したら送信後にループを抜ける

### server.py(ループ版)

```python
import socket

HOST = '127.0.0.1'
PORT = 5000

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # デバッグ時の再起動が楽になる
serversocket.bind((HOST, PORT))
serversocket.listen(1)
print(f"Listening on {HOST}:{PORT}...")

conn, addr = serversocket.accept()
print(f"Connected by {addr}")

while True:
    data = conn.recv(1024)
    if not data or data == b"quit":
        print("Client requested to quit, or disconnected.")
        break
    print(f"Received: {data}")
    conn.sendall(data)

conn.close()
serversocket.close()
```

### client.py(ループ版)

```python
import socket

HOST = '127.0.0.1'
PORT = 5000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    msg = input("Enter message (or 'quit' to exit): ")
    s.sendall(msg.encode())
    if msg == "quit":
        break
    data = s.recv(1024)
    print(f"Received back: {data}")

s.close()
```

## 変更点の解説

- `while True:` でサーバ・クライアントともにループさせています
- `if not data or data == b"quit":` の`not data`は、クライアントが`close()`した場合に`recv()`が空バイト列`b""`を返すことへの対処です。これがないと、切断後もサーバが無限ループでCPUを消費し続けます(ビジーループ)
- クライアント側の`msg.encode()`は、`input()`で得た文字列(str型)をbytes型に変換しています。以前の`b"hello.socket world!"`と同じ効果ですが、動的な文字列を送る場合は`.encode()`を使うのが一般的です
- `SO_REUSEADDR`を追加したのは、テスト中にサーバを再起動するときの`Address already in use`エラー対策です

## 試してみてください

1. ターミナルAで`python3 server.py`
2. ターミナルBで`python3 client.py`
3. 何度かメッセージを送ってみて、オウム返しされることを確認
4. `quit`と入力して、両方が正常終了することを確認

動かしてみて、疑問点(例:`not data`が何を意味するか、`quit`のタイミングでサーバ側が本当に抜けるか、など)があれば教えてください。うまく動いたら、ステップ3(マルチクライアント対応)に進みます。

## Q:空の文字列を入力した時、どちらも反応しなくなるのは何故か？

良い質問です。これは初心者が必ず一度ハマるポイントで、TCP/socketの理解が一段深まる良い経験です。原因を説明します。

## 何が起きているか

問題の箇所はここです:

```python
msg = input("Enter message (or 'quit' to exit): ")
s.sendall(msg.encode())
```

`input()`でEnterキーだけ押す(何も入力しない)と、`msg`は空文字列`""`になります。これを`.encode()`すると空バイト列`b""`になります。

**ここが重要**:`s.sendall(b"")`は、**実際には何もネットワーク上に送信しません**。TCPソケットにおいて、0バイトの送信は「送るデータがない」という扱いになり、相手側には何も届きません(接続が切れるわけでもなく、ただ何も起こらない)。

## デッドロックの構造

1. クライアント:`sendall(b"")` → 実質何も送られない
2. クライアント:次に`s.recv(1024)`でサーバからの応答を待つ → **ブロック(待機)**
3. サーバ:何も受信していないので、`conn.recv(1024)`は待ち続ける → **ブロック(待機)**

両者が「相手が何か送ってくるはず」と思ってお互いに`recv()`で待ち続ける状態になります。これが**デッドロック**です。`Ctrl+C`で強制終了するしかありません。

## 修正方法

空文字列を送信しないように、送信前にチェックを入れます。

```python
while True:
    msg = input("Enter message (or 'quit' to exit): ")
    if msg == "":
        print("Empty message, please enter something.")
        continue  # sendせずにループの最初に戻る

    s.sendall(msg.encode())
    if msg == "quit":
        break
    data = s.recv(1024)
    print(f"Received back: {data}")
```

`continue`を使うことで、空文字列の場合は送信も受信もスキップして、もう一度`input()`に戻ります。

## より本質的な学び

これは実は、ロードマップのステップ2で触れられている**「メッセージの区切り」問題**の一種です。「何も送っていないのに、受信側は何か来ることを期待して待ち続ける」というのは、プロトコル設計(通信のルール)を自分で決めていないと必ず起きる問題です。

今回のケースでは「空文字列は送らない」という単純なルールで回避しましたが、実際のアプリケーションでは以下のような設計もよく使われます:

- 空メッセージも「有効なメッセージ」として扱い、必ず何か(改行やヘッダ)を付けて送る
- 「メッセージの終わり」を示すデリミタ(区切り文字、例:`\n`)を必ず付ける

これは後のステップ4「固定長→長さヘッダ方式」にもつながる話です。「受信側がどうやって『これで1メッセージ分届いた』と判断するか」は、ソケットプログラミングの核心的なテーマの一つです。

修正版を試して、動作を確認してみてください。