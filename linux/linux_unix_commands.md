# Linux/Unix でよく使うコマンドライン

## ファイル・ディレクトリ操作

```bash
ls -la          # ファイル一覧(隠しファイル含む詳細表示)
cd /path/to/dir # ディレクトリ移動
pwd             # 現在のディレクトリを表示
mkdir dirname   # ディレクトリ作成
mkdir -p a/b/c  # 親ディレクトリごと作成
rm file         # ファイル削除
rm -rf dir      # ディレクトリを強制・再帰削除(注意して使用)
cp src dst      # コピー
cp -r src dst   # ディレクトリを再帰コピー
mv src dst      # 移動・リネーム
touch file      # 空ファイル作成/タイムスタンプ更新
find . -name "*.txt"  # ファイル検索
```

## ファイル内容確認

```bash
cat file        # 内容を表示
less file       # ページャーで表示(qで終了)
head -n 20 file # 先頭20行
tail -n 20 file # 末尾20行
tail -f log     # ログをリアルタイム監視
wc -l file      # 行数カウント
```

## 検索・テキスト処理

```bash
grep "pattern" file          # 文字列検索
grep -r "pattern" dir/       # 再帰検索
grep -i "pattern" file       # 大文字小文字無視
sed 's/foo/bar/g' file       # 置換
awk '{print $1}' file        # 列抽出
sort file                    # ソート
uniq                         # 重複行削除(要sort後)
diff file1 file2             # 差分表示
```

## 権限・所有者

```bash
chmod 755 file        # パーミッション変更
chmod +x script.sh    # 実行権限付与
chown user:group file # 所有者変更
```

## プロセス管理

```bash
ps aux           # プロセス一覧
top / htop        # リアルタイム監視
kill PID          # プロセス終了
kill -9 PID       # 強制終了
jobs              # バックグラウンドジョブ一覧
bg / fg           # バックグラウンド/フォアグラウンド切替
nohup cmd &       # ログアウト後も継続実行
```

## ネットワーク

```bash
ping host              # 疎通確認
curl https://url       # HTTPリクエスト
wget https://url       # ファイルダウンロード
ssh user@host          # リモート接続
scp file user@host:/path  # ファイル転送
netstat -tulnp / ss -tulnp  # ポート使用状況確認
```

## ディスク・システム情報

```bash
df -h        # ディスク使用量
du -sh dir/  # ディレクトリサイズ
free -h      # メモリ使用量
uname -a     # システム情報
```

## 圧縮・展開

```bash
tar -czvf archive.tar.gz dir/   # 圧縮
tar -xzvf archive.tar.gz        # 展開
zip -r archive.zip dir/         # zip圧縮
unzip archive.zip               # zip展開
```

## パイプ・リダイレクト

```bash
cmd1 | cmd2       # パイプ(結果を次のコマンドへ)
cmd > file        # 標準出力をファイルへ(上書き)
cmd >> file       # 追記
cmd 2> error.log  # エラー出力をファイルへ
cmd &> all.log    # 標準出力・エラー両方
```

## その他便利系

```bash
history          # コマンド履歴
alias ll='ls -la'  # エイリアス設定
man cmd          # マニュアル表示
which cmd        # コマンドのパス確認
xargs            # 標準入力を引数に変換
env              # 環境変数一覧
export VAR=value # 環境変数設定
```
