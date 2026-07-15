# Findコマンドについて

`find` はファイルやディレクトリを条件を指定して検索する、非常に柔軟なコマンドです。基本構文と主要なオプションを解説します。

## 基本構文

```bash
find [検索開始パス] [検索条件] [アクション]
```

```bash
find .              # カレントディレクトリ以下の全ファイル・ディレクトリを表示
find /home           # /home以下を検索
```

## 名前で検索

```bash
find . -name "*.txt"        # 拡張子.txtのファイルを検索(大文字小文字を区別)
find . -iname "*.txt"       # 大文字小文字を区別しない検索
find . -name "test*"        # testで始まるファイル
find . -not -name "*.log"   # .log以外を検索
```

## 種類で絞り込む(`-type`)

```bash
find . -type f      # ファイルのみ
find . -type d      # ディレクトリのみ
find . -type l       # シンボリックリンクのみ
```

例:カレント以下のディレクトリだけ一覧表示

```bash
find . -type d -name "src"
```

## サイズで検索(`-size`)

```bash
find . -size +100M    # 100MBより大きいファイル
find . -size -1k       # 1KBより小さいファイル
find . -size 10M       # ちょうど10MBのファイル
```

単位:`c`(バイト) `k`(KB) `M`(MB) `G`(GB)。`+`は「より大きい」、`-`は「より小さい」。

## 更新日時で検索(`-mtime`, `-mmin`)

```bash
find . -mtime -1      # 1日以内に更新されたファイル
find . -mtime +7      # 7日以上前に更新されたファイル
find . -mmin -30       # 30分以内に更新されたファイル
```

- `-mtime`:日数単位
- `-mmin`:分単位
- `+`は「より前」、`-`は「より後(以内)」

その他に `-atime`(アクセス日時)、`-ctime`(属性変更日時)もあります。

## 権限で検索(`-perm`)

```bash
find . -perm 755         # パーミッションが755のファイル
find . -perm -u+x         # 所有者に実行権限があるファイル
```

## 深さを指定する(`-maxdepth`, `-mindepth`)

```bash
find . -maxdepth 1 -type f   # カレント直下のファイルのみ(サブディレクトリは見ない)
find . -mindepth 2            # 2階層より深いものだけ
```

`-maxdepth` はディレクトリを深く探索しすぎたくないときによく使います。

## 検索結果に対してアクションを実行

### `-exec`:見つけたファイルに対してコマンドを実行

```bash
find . -name "*.tmp" -exec rm {} \;
```

- `{}` は見つかったファイル名に置き換わる
- `\;` は各ファイルごとに1回コマンドを実行(セミコロンのエスケープが必要)

まとめて一度に処理したい場合は `\+` を使うと効率的です:

```bash
find . -name "*.tmp" -exec rm {} \+
```

### `-delete`:見つけたファイルを直接削除

```bash
find . -name "*.tmp" -delete
```

`-exec rm {} \;` よりシンプルですが、削除前によく確認してから実行してください。

## 複数条件の組み合わせ

```bash
find . -name "*.log" -a -size +10M     # AND条件(-aは省略可能)
find . -name "*.log" -o -name "*.tmp"  # OR条件
find . -not -name "*.log"               # NOT条件
```

括弧を使う場合はエスケープが必要です:

```bash
find . \( -name "*.log" -o -name "*.tmp" \) -delete
```

## 空のファイル・ディレクトリを検索

```bash
find . -empty          # 空のファイル・ディレクトリ
find . -type f -empty   # 空のファイルのみ
```

## 所有者・グループで検索

```bash
find . -user username    # 特定ユーザーが所有するファイル
find . -group groupname  # 特定グループが所有するファイル
```

## よく使う実践例

```bash
# 7日以上前のログファイルを削除
find /var/log -name "*.log" -mtime +7 -delete

# カレント以下の.txtファイルの内容をすべてgrep検索と組み合わせる
find . -name "*.txt" -exec grep -l "error" {} \;

# 特定ディレクトリを検索対象から除外する
find . -path "./node_modules" -prune -o -name "*.js" -print

# 100MB以上のファイルを一覧表示してサイズ順にソート
find . -type f -size +100M -exec ls -lh {} \; | sort -k5 -h
```

## ポイントまとめ

| オプション | 意味 |
|---|---|
| `-name` / `-iname` | ファイル名で検索 |
| `-type f/d/l` | 種類で絞り込み |
| `-size` | サイズで検索 |
| `-mtime` / `-mmin` | 更新日時で検索 |
| `-perm` | パーミッションで検索 |
| `-maxdepth` / `-mindepth` | 探索する階層を制限 |
| `-exec` | 見つけたファイルにコマンド実行 |
| `-delete` | 見つけたファイルを削除 |

`find` は組み合わせ次第で非常に強力なので、特に `-delete` や `-exec rm` を使う際は、まず `-print`(デフォルト動作)で対象ファイルを確認してから実行することをおすすめします。