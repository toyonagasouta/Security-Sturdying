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

## ステップ3: マルチクライアント対応

ステップ3に進みましょう。「1つのクライアントとしか話せない」現状のサーバを、`threading`を使って複数クライアントと同時に会話できるように改造します。

## 現状の問題点

今のサーバは`accept()`を**1回しか呼んでいません**。つまり:

```python
conn, addr = serversocket.accept()  # 1回だけ
while True:
    # このconnとだけずっと会話する
```

2つ目のクライアントが接続しようとしても、サーバは最初のクライアントとの会話ループから抜けるまで、次の`accept()`を呼べません。

## 解決の方針:スレッドで分担する

考え方はシンプルです:

1. サーバの外側のループで、ひたすら`accept()`を呼び続ける(新しい接続を受け入れる専門の係)
2. 新しい接続が来るたびに、その接続とのやり取り(`recv`/`sendall`のループ)を**別スレッド**に任せる
3. メインスレッドはすぐに次の`accept()`に戻り、また新しい接続を待つ

図にするとこうです:

```
メインスレッド: accept() → accept() → accept() → ...(ひたすら待ち受け)
                    ↓           ↓           ↓
                スレッドA    スレッドB    スレッドC
              (クライアント1) (クライアント2) (クライアント3)
              各自がrecv/sendallループを担当
```

## server.py(マルチクライアント対応版)

```python
import socket
import threading

HOST = '127.0.0.1'
PORT = 5000

def handle_client(conn, addr):
    """1クライアント分の会話を担当する関数(スレッドで実行される)"""
    print(f"[+] Connected by {addr}")
    try:
        while True:
            data = conn.recv(1024)
            if not data or data == b"quit":
                print(f"[-] {addr} disconnected.")
                break
            print(f"[{addr}] Received: {data}")
            conn.sendall(data)
    finally:
        conn.close()

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
serversocket.bind((HOST, PORT))
serversocket.listen(5)  # 待機キューを5に拡大
print(f"Listening on {HOST}:{PORT}...")

try:
    while True:
        conn, addr = serversocket.accept()
        t = threading.Thread(target=handle_client, args=(conn, addr))
        t.daemon = True  # メインプログラム終了時にスレッドも道連れにする
        t.start()
except KeyboardInterrupt:
    print("\nShutting down server...")
finally:
    serversocket.close()
```

`client.py`はステップ2のものをそのまま使えます(変更不要です)。

## 変更点の解説

**`handle_client(conn, addr)`関数**
これまで`main`の中に直接書いていた「1クライアントとの会話ループ」を、独立した関数に切り出しました。これが各スレッドで実行される処理単位になります。

**`import threading`と`threading.Thread(...)`**
```python
t = threading.Thread(target=handle_client, args=(conn, addr))
t.start()
```
新しいスレッドを作り、`handle_client`関数を`(conn, addr)`という引数付きで実行させます。`t.start()`でスレッドが実際に動き出しますが、**メインスレッドはここでブロックせず、すぐ次の行に進みます**。ここがポイントです。

**`t.daemon = True`**
デーモンスレッドに設定すると、メインプログラム(サーバ本体)が終了するときに、このスレッドも強制的に終了します。これがないと、サーバを`Ctrl+C`で止めても、動いているクライアント対応スレッドが残ってプログラムが終了しない、ということが起こり得ます。

**外側の`while True:`**
```python
while True:
    conn, addr = serversocket.accept()
    ...
```
これがメインスレッドの仕事です。「新しい接続を受け入れる」ことだけに専念し、実際の会話は各スレッドに任せます。

**`serversocket.listen(5)`**
待ち受けキューのサイズを1→5に増やしました。同時に接続要求が来ても、5件までは受付順に処理を待たせられます(接続自体を拒否するわけではなく、`accept()`が呼ばれるまでの待ち行列のサイズです)。

**`try/except KeyboardInterrupt`**
`Ctrl+C`で止めたときに、Pythonの例外トレースバックがズラッと表示されるのを防ぎ、綺麗に終了メッセージを出すためです。

## 試してみてください

1. `python3 server.py`を起動
2. ターミナルを**2つ以上**開いて、それぞれで`python3 client.py`を実行
3. 両方のクライアントから同時にメッセージを送ってみて、サーバ側のログに両方の`addr`(ポート番号違い)が表示され、それぞれ独立して応答が返ってくることを確認してください

## 注意点(次に繋がる話)

このコードには、あえて触れていない問題が2つあります:

1. `print()`は複数スレッドから同時に呼ばれると、出力が入り乱れることがある(ログの取り扱いは実務では`logging`モジュール+ロックを使うのが一般的)
2. スレッド数が増えすぎると(数千クライアントなど)メモリ・OS資源を圧迫する。これが`select`/`epoll`ベースの非同期方式(ロードマップにある`select`)が使われる理由です

動かしてみて、複数クライアントが同時に会話できることを確認してください。うまくいったら、`select`を使ったノンブロッキング方式(スレッドを使わない代替アプローチ)に進むか、ステップ4(長さヘッダ方式)に進むか、どちらか選んでください。

## ステップ4: 固定長→長さヘッダ方式への発展

ステップ4に進みましょう。「メッセージの区切り」に関する問題に、より本格的に対処する方法です。

## なぜ`recv(1024)`だけでは不十分なのか

これまでのコードは`conn.recv(1024)`で「最大1024バイトまで受け取る」としてきましたが、TCPには重要な特性があります:

**TCPはストリーム(バイトの連続した流れ)であり、メッセージの境界を保持しません。**

つまり、送信側が`sendall(b"Hello")`と`sendall(b"World")`を連続で呼んでも、受信側の`recv()`では:
- 1回の`recv()`で`b"HelloWorld"`とまとめて届くかもしれない
- `b"Hel"`と`b"loWorld"`のように分割されて届くかもしれない

「1回の`recv()`=1つの意味のあるメッセージ」という保証は、TCPにはどこにもありません。これまでのコードがたまたま動いていたのは、データ量が小さくローカル通信だったからに過ぎません。

## 解決策:「長さヘッダ」方式

考え方はシンプルです:

1. **送信側**:本文を送る前に、まず「本文が何バイトか」を固定長(例:4バイト)で送る
2. **受信側**:まず固定長(4バイト)を読んで「これから何バイト来るか」を知り、次にその長さぴったりのバイト数を読み切るまで`recv`を繰り返す

```
送信データの構造:
[4バイト:本文の長さ(整数)] + [本文そのもの]
```

## `struct`モジュールとは

Pythonの整数(int)は、そのままではネットワークに送れません(bytesに変換する必要があります)。`struct`モジュールは、整数などを決まったバイト数のバイト列に変換(パック)したり、逆にバイト列から整数に戻したり(アンパック)するための標準ライブラリです。

```python
import struct

length = 20
packed = struct.pack('!I', length)  # 整数20 → 4バイトのbytes
print(packed)  # b'\x00\x00\x00\x14'

unpacked = struct.unpack('!I', packed)[0]  # 4バイトのbytes → 整数20に戻す
print(unpacked)  # 20
```

- `'!I'`というフォーマット文字列の意味:
  - `!` = ネットワークバイトオーダー(ビッグエンディアン)を使う、という指定。異なるマシン間(エンディアンが違う可能性がある)でも解釈がずれないようにするための約束事です
  - `I` = unsigned int(符号なし整数)、4バイト固定
- `struct.unpack`は**タプル**を返すので、`[0]`で最初の要素(今回は値が1つだけなので)を取り出しています

## 実装:共通の送受信関数を用意する

サーバ・クライアント両方で同じ仕組みを使うので、共通関数として書きます。

### 送信側の関数(両ファイル共通で使う)

```python
import struct

def send_msg(sock, data: bytes):
    length_prefix = struct.pack('!I', len(data))
    sock.sendall(length_prefix + data)
```

### 受信側の関数(両ファイル共通で使う)

```python
def recv_exactly(sock, n: int) -> bytes:
    """ちょうどnバイト受信するまでrecvを繰り返す"""
    buf = b''
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:
            return b''  # 相手が切断した
        buf += chunk
    return buf

def recv_msg(sock):
    length_prefix = recv_exactly(sock, 4)
    if not length_prefix:
        return None  # 切断
    (length,) = struct.unpack('!I', length_prefix)
    return recv_exactly(sock, length)
```

**`recv_exactly`がなぜ必要か**がステップ4の核心です。「4バイトのヘッダを読む」つもりで`sock.recv(4)`を1回呼んでも、TCPは3バイトしか届けてくれないかもしれません(理論上はいつでも起こり得ます)。だから「指定したバイト数ぴったり届くまでループする」処理が必須になります。これは先ほど話した「TCPはストリームで境界を保持しない」という性質への、正攻法の対処です。

## server.py(長さヘッダ方式・マルチクライアント対応)

```python
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

def recv_msg(sock):
    length_prefix = recv_exactly(sock, 4)
    if not length_prefix:
        return None
    (length,) = struct.unpack('!I', length_prefix)
    return recv_exactly(sock, length)

def send_msg(sock, data: bytes):
    sock.sendall(struct.pack('!I', len(data)) + data)

def handle_client(conn, addr):
    print(f"[+] Connected by {addr}")
    try:
        while True:
            data = recv_msg(conn)
            if data is None or data == b"quit":
                print(f"[-] {addr} disconnected.")
                break
            print(f"[{addr}] Received ({len(data)} bytes): {data}")
            send_msg(conn, data)
    finally:
        conn.close()

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
serversocket.bind((HOST, PORT))
serversocket.listen(5)
print(f"Listening on {HOST}:{PORT}...")

try:
    while True:
        conn, addr = serversocket.accept()
        t = threading.Thread(target=handle_client, args=(conn, addr))
        t.daemon = True
        t.start()
except KeyboardInterrupt:
    print("\nShutting down server...")
finally:
    serversocket.close()
```

## client.py(長さヘッダ方式)

```python
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
    length_prefix = recv_exactly(sock, 4)
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
        print("Empty message, please enter something.")
        continue

    send_msg(s, msg.encode())
    if msg == "quit":
        break

    data = recv_msg(s)
    if data is None:
        print("Server closed the connection.")
        break
    print(f"Received back ({len(data)} bytes): {data}")

s.close()
```

## 動作確認のポイント

1. サーバ・クライアントを起動して、通常のやり取りが今まで通り動くことを確認
2. 今度は**長いメッセージ**(2000文字くらいコピペした文章など)を送ってみてください。以前の`recv(1024)`固定版だと、1024バイトを超えるデータは途中で切れて壊れていたはずですが、今回は`length`ぶん正しく受信できることを確認できます

## この方式が持つ意味(Exploit開発文脈への接続)

ロードマップの最後に触れられている通り、これは`struct.pack`/`unpack`を使った**バイナリプロトコル設計**の基本形です。実際のネットワークプロトコル(HTTP/2のフレーム、多くのバイナリRPCプロトコルなど)も、根本的には「長さ+本文」という考え方の応用です。

また、脆弱性の文脈で言うと、**この「長さフィールド」自体が攻撃対象になり得る**という点も重要です。例えば:
- 送信側が主張する`length`と実際に送ってくるバイト数が矛盾している(悪意ある送信者が異常に大きい値を送り、受信側に大量のメモリ確保をさせる=DoS)
- 整数オーバーフロー(古い実装で32bit境界を超える値を細工する)

こうした「信頼できない入力に対して長さや構造を検証する」という視点は、次に学ぶバイナリプロトコル解析や脆弱性調査で継続的に出てくるテーマです。今回のコードでは`length`の妥当性チェック(上限を設けるなど)は入れていませんが、余裕があれば「異常に大きいlengthを送られた場合にどう防御するか」も考えてみると良い練習になります。

動かしてみて、疑問点があれば教えてください。