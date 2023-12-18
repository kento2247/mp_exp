# 情報工学実験第 2 マイクロプロセッサ実験

## 重要なお知らせ

まずは、「実験補足資料」に含まれる「マイクロプロセッサ実験の概要」をよく読んで欲しい。実験の内容、趣旨、実験に使う FPGA ボード、製作するゲーム機の様子、ハードウェア・ソフトウェアの作業内容などが書かれている。

理想的な実験スケジュール（3 週間分）は以下の通り。5 限までは延長可。

| 週     | 時限     | 内容                                                                                                                 |
| :----- | :------- | :------------------------------------------------------------------------------------------------------------------- |
| 1 週目 | 2 時限   | 実験の概要説明。班分け。機材配布。チュートリアル 1 を実施。                                                          |
|        | 3 時限   | チュートリアル 2 を実施。                                                                                            |
|        | 4 時限   | 液晶モニタ（LCD）を使えるようにする（班ごとに演習 1、演習 2 を行う）。開発するゲームの外部仕様および内部仕様を検討。 |
| 2 週目 | 2 時限   | 先週に引き続き外部仕様および内部仕様を策定。教員とディスカッション。                                                 |
|        | 3-4 時限 | FPGA ボード上でゲーム開発を開始。LCD を使ったアニメーション、スイッチを使った当たり判定ができた班は解散。            |
| 3 週目 | 2-3 時限 | FPGA ボード上でゲームを完成させる。必要に応じて拡張ボードを追加。                                                    |
|        | 4 時限   | ゲームが完成した班は、学生、教員、TA の前でデモ発表を行う。デモ発表後、レポート作成の指示を聞いたら解散。            |

3 週目の最後に、完成したゲーム機の発表会を行う。FPGA の実機ボードを用いてゲームを実演してもらい、ゲームの機能や面白さをアピールしてもらう。教員および TA が審査委員となり、審査委員の合議によって各班の得点が決まる。

## チュートリアル 1

製品やツールのひと通りの操作方法を説明したドキュメントのことを「チュートリアル」と呼ぶ。FPGA ボード、測定機器、開発ツール、アプリケーションなどには通常のマニュアルに加え、チュートリアルが用意されている場合がある。 実際の開発現場では、まず、チュートリアルをこなして全体の流れを理解したうえで、詳細な使い方については適宜マニュアルを読むという流れが一般的である。

作業には Linux マシンを使用する。それでは実際にチュートリアルをやってみよう。

### サンプルコードとコンパイル

チュートリアル用のファイル一式を手元にコピーして解凍する。

```sh
> cp ~aa205875/2023/mips-20230828.tar.gz .
> tar zxvf mips-20230828.tar.gz
> cd mips
```

上記の aa205875 は教員のアカウント名である。同じ班のメンバのアカウント名が分かればディレクトリを覗くことができる。班の中でファイルをやり取りする際に活用しよう。

含まれているファイルは以下の通り。

| ディレクトリ名 | ファイル名  | 内容                                                                             |
| :------------- | :---------- | :------------------------------------------------------------------------------- |
| soft/          |             | プロセッサ上で動作するプログラムは soft/以下で開発する                           |
|                | Makefile    | make と打つと test.c をコンパイルし、機械語データを生成する                      |
|                | test.01.c   | プロセッサ上で動作する C 言語プログラム（LED、SWITCH のテスト）                  |
|                | test.02.c   | プロセッサ上で動作する C 言語プログラム（LED、SWITCH、タイマーのテスト）         |
|                | test.03.c   | 演習 2（ソフトウェア）を参照                                                     |
|                | crt0.c      | プロセッサの初期化用コード（プログラムをコンパイルする際に必要）                 |
|                | mips.ld     | リンカスクリプト（プログラムをリンクする際に必要）                               |
| hard/          |             | プロセッサ、メモリ、I/O 等のハードウェアモデル（Verilog HDL 言語）               |
|                | Makefile    | make と打つと論理合成、配置配線を行い、構成データを生成する                      |
|                | top.v       | FPGA 上に実装するハードウェア（mips モジュールを呼び出す）                       |
|                | mips.v      | MIPS R2000 互換プロセッサ                                                        |
|                | parts.v     | MIPS R2000 互換プロセッサで使われている部品                                      |
|                | multdiv.v   | MIPS R2000 互換プロセッサで使われている乗算器、除算器                            |
|                | fpga.xdc    | top モジュールの入出力信号と FPGA の物理ピンとの対応付け                         |
|                | fpga_z7.xdc | top モジュールの入出力信号と FPGA の物理ピンとの対応付け（**新 FPGA ボード用**） |
| hard/script/   |             | 上記の Makefile の中で使われるスクリプト類                                       |

プロセッサ上で動作するソフトウェアプログラムは soft/以下で開発する。プロセッサ、メモリ、I/O 等のハードウェアモデル（Verilog HDL 言語で書かれている）は hard/以下にある。sim/というシミュレーション用のディレクトリもあるが、最近はメニューから外している。興味があるひとは付録 E の「シミュレーション」を参照されたい。

次に C 言語サンプルプログラム soft/test.01.c の中身を確認して欲しい。

```sh
> cd soft
> cat test.01.c
```

```c
/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"
void interrupt_handler(){}

void main()
{
        volatile int *sw_ptr = (int *)0xff04;
        volatile int *led_ptr = (int *)0xff08;
        for (;;)
                *led_ptr = *sw_ptr;
}
```

先頭 3 行（正確には 2 行目と 3 行目）は消してはいけない。これらについては後述する。

0xff08 番地は LED の番地であり、ここに 4-bit の値を書き込むと値の通りに 4 個の LED が光る。

0xff04 番地は SWITCH の番地であり、4 個のスライドスイッチと 4 個の押しボタンの状態を 8-bit の値として取得できる。対応するスイッチは下位ビットから順にスライドスイッチ 4 個、押しボタン 4 個である。スイッチがオンならば 1、オフならば 0 が読み出される。押しボタン 0 と 1 を同時に押すと強制リセットがかかるようになっている（LED は光らない）。

上記のプログラムでは SWITCH の値 8-bit を LED に表示し続ける。SWITCH のオンオフによって LED の点灯パターンが変化するので、実機で試してみよう。

この test.01.c をコンパイルして機械語コード program.dat を生成したい。このための Makefile はコンパイル対象のソースファイルを test.c としているので、まず、test.01.c を test.c にコピーする。「test.01.c を test.c にコピーする」代わりに Makefile の SOURCE の部分を修正しても良い。

```sh
> cp test.01.c test.c
```

次に make コマンドによってコンパイルする。コンパイルできたら less コマンドで program.dat の中身を確認しよう。中間ファイル program.dump も参考になる。

```sh
> make
> less program.dat
> less program.dump
```

### 実機動作確認

ハードウェアおよびソフトウェアを実際の FPGA ボード上で動作させるには、まず、ハードウェア（機械語コード program.dat も含む）を FPGA 向けに論理合成、配置配線し、構成データ（ビットストリーム）を得る。PC と FPGA ボードを USB ケーブルでつなぎこの構成データを FPGA に焼き込む。

この操作には Xilinx 社の Vivado というツールを使う。機能限定ライセンス（WebPACK ライセンス）であれば Xilinx 社のホームページから無料で取得できる。今回使用する Zynq Z-7010 など比較的小規模な FPGA であれば WebPACK 版で十分である。

以下のようにして Vivado を起動する。

```sh
> vivado &
```

Vivado を用いた論理合成、配置配線、構成データの焼き込みは最初は GUI ベースで行う（CUI ベースのやり方はチュートリアル 2 で扱う）。GUI の操作方法は写真を見たほうが分かりやすいので、「実験補足資料」に含まれる「Xilinx Vivado の使い方」の通りに作業を進めて欲しい。

既有の FPGA ボードが生産中止になってしまったため、新しく調達した FPGA ボードは仕様が若干異なる。お手元のボードに黄色の「新」シールが貼ってあったら「**新 FPGA ボード**」である。

## チュートリアル 2

一通りの操作方法が分かったところで、今度は別のチュートリアルによってハードウェアとソフトウェアの詳細を見ていこう。

まず、サンプルプログラム soft/test.02.c の中身を確認して欲しい。

```c
/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"

/* interrupt_handler() is called every 100msec */
void interrupt_handler()
{
        static int cnt = 0;
        volatile int *led_ptr = (int *)0xff08;
        cnt++;
        if (cnt % 10 == 0)
                *led_ptr = cnt / 10;
}

void main()
{
        for (;;);
}
```

### 割込み

プログラム中の interrupt_handler()は 100msec（0.1 秒）に 1 回呼び出される。このような処理を割込みハンドラと呼ぶ。この割込みハンドラは 10 回に 1 回、つまり、1 秒毎に LED の値を 1 ずつインクリメントするので、実機で試してみよう。

これを実現するために、hard/top.v に以下のようなタイマー回路が実装されている。

```verilog
/* Timer module (@62.5MHz) */
timer timer (clk_62p5mhz, reset, irq);
```

100msec に 1 度だけ irq という信号の値が 1 になる。irq はプロセッサに直接つながっていて、irq が 1 になるとプロセッサは予め決められた番地（本実験環境では 0x0100 番地）の命令を実行するようになっている。0x0100 番地には上述の interrupt_handler()を呼び出すためのコードが置いてある。詳細は soft/crt0.c の**vector**:以下のコードを参照。

割込みハンドラの実行中は追加の割込みは禁止である。ユーザからの入力待ちなど時間のかかる処理を割込みハンドラの中で実行してはいけない。

### メモリ空間

上述の通り、0xff08 番地は LED の番地である。SWITCH もメモリ空間上の番地に割り当てられている。まずはメモリ空間について説明する。

FPGA 上に実現した MIPS 互換プロセッサは 65,536 bytes（16,384 words）のメインメモリを内蔵している。メインメモリには、プログラムコードとデータが区別なく格納される。

スタックもメインメモリ上に実現され、C 言語のローカル変数の使用または関数コールの度にスタック領域が使われる。スタックは上位番地（0xff00）からスタートして下位番地（0x0000）の方向へ伸びていく。

機械語コード program.dat は、以下の通り、hard/top.v にて RAM に読み込まれる。RAM のサイズは 16,384 words である。

```verilog
/* Specify your program image file (e.g., program.dat) */
initial $readmemh("program.dat", RAM, 0, 16383);
```

実験で使う FPGA ボードには、SWITCH、LED、LCD などの I/O が接続されており、プロセッサのメモリ空間にマッピングされている。つまり、プロセッサから SWITCH の番地の値をロード（lw 命令）すれば SWITCH の状態を取得できる。プロセッサから LED の番地にデータをストア（sw 命令）すればその値の通りに LED が点灯する。同様に、決められた手順で LCD の番地にコマンドをストアすれば液晶モニタに文字を表示できる。このような I/O 方式をメモリマップト I/O と呼ぶ。

メモリ空間とその使途を以下にまとめる。メモリ番地は 16 進数で書かれている。実験では 32-bit プロセッサを使っているため、32-bit のメモリ空間（4,294,967,296 bytes、つまり 4 Gbytes）を扱えるが、本実験環境のメモリサイズは 64 kbytes である。

| 番地                  | 極性       | 内容                                                                                                                                                                                                                                                                                                                          |
| :-------------------- | :--------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 0x00000000-0x0000ff00 | Read/Write | メインメモリ（プログラムコード、データ、スタック領域）。サイズは 16,384 words 弱。                                                                                                                                                                                                                                            |
| 0x0000ff04            | Read Only  | SWITCH の番地。この番地の値をロードすると押しボタンおよびスライドスイッチの値を取得できる。下位 8-bit の値は、0-bit 目から順にスライドスイッチ 4 個、押しボタン 4 個に対応しており、スイッチがオンならば 1、オフならば 0。上位 24-bit は常に 0 を返す。押しボタン 0 と 1 を同時に押すと強制リセットがかかるようになっている。 |
| 0x0000ff08            | Write Only | LED の番地。この番地に 4-bit の値をストアすると、その 2 進数表記の通りに 4 個の LED が点灯する。上位 28-bit の値は無視される。                                                                                                                                                                                                |
| 0x0000ff0c            | Write Only | 液晶モニタ（LCD）の番地。この番地に 11-bit の制御コマンドをストアすると LCD に文字が表示される。0 から 7-bit 目までが 8-bit データ、8-bit 目が RW 信号、9-bit 目が RS 信号、10-bit 目が E 信号である（各信号の意味は「LCD 表示の基礎知識」で述べる）。上位 21-bit は無視される。                                              |
| 0x00010000-0xffffffff | N/A        | 未使用                                                                                                                                                                                                                                                                                                                        |

### Vivado の CUI モード

メモリ空間を理解したところ、さっそく test.02.c を FPGA ボード上で動かしてみたい。

チュートリアル 2 では CUI（コマンドライン）ベースの開発方法を覚えよう。チュートリアル 1 で使用した GUI ベースのツールは操作が直感的で分かりやすいという利点があるものの、ツールに慣れてくるとマウス操作を煩わしく感じることもある。操作に慣れたら CUI ベースのツールが便利である。

まず、test.02.c をコンパイルして新しい program.dat を作る。make の前に前回のコンパイル結果を削除するために make clean しておく。

```sh
> cd soft
> cp test.02.c test.c
> make clean
> make
```

次に論理合成、配置配線を行って、構成データを生成する。

**新 FPGA ボード**の場合は hard/Makefile を以下のように書き換える。つまり、bitgen.tcl の代わりに bitgen_z7.tcl が呼ばれるように修正する。make や vivado コマンドの前の空白はスペースではなく、タブなので注意。

```
all:
	make clean
	#vivado -mode batch -source script/bitgen.tcl
	vivado -mode batch -source script/bitgen_z7.tcl
```

ここも make の前に make clean しておく。実行内容の詳細は hard/Makefile および hard/script/bitgen.tcl（**新 FPGA ボード**の場合は bitgen_z7.tcl）を参照。

```sh
> cd hard
> make clean
> make
```

最後に PC と FPGA ボードを USB ケーブルでつなぎ、FPGA ボードの電源を On にしたうえでこの構成データを FPGA に焼き込む。実行内容の詳細は hard/Makefile および hard/script/program.tcl を参照。

```sh
> make program
```

プログラムを開始するために、押しボタン 0 と 1 を同時に押してプロセッサをリセットしよう。

**注意: test.c を書き換える度に soft/以下で make clean、make して program.dat を作り直し、そのあと hard/以下で make clean、make、make program する必要がある。**

## 拡張 I/O ボード

FPGA ボード上には、すでに SWITCH、LED、液晶モニタ（LCD）などの I/O が載っている。これだけでゲーム機を作ることもできるが、さらに I/O を追加するとゲーム機っぽくなる。モグラ叩きの例ならば、モグラを叩いたらブザーが鳴る、ライフ（残基）数を追加の LED に表示する、回転スイッチ（ロータリーエンコーダ）で速度を調整するなど。以下の Pmod ボードを貸与する。

- Pmod KYPD: 16 ボタンのキーパッド
- Pmod BB: ブレッドボード（ブザーを実装済）
- Pmod 8LD: 8 個の追加 LED
- Pmod SWT: 4 個の追加スライドスイッチ
- Pmod BTN: 4 個の追加押しボタン
- Pmod ENC: ロータリーエンコーダ（スライドスイッチ、押しボタン付き）

## ソフトウェア部分の基礎知識

本実験環境におけるソフトウェアの動作の仕組みを紹介する。各自、目を通しておいて欲しい。

### プログラム動作の仕組み

本実験環境では、メインメモリの 0 番地目に格納されている命令からプログラムの実行が開始される。C 言語の main()関数からではないことに注意されたい。

サンプルプログラム（test.01.c など）では先頭で crt0.c をインクルードしていたことを思い出して欲しい。crt0.c には main()が呼び出される前のスタートアップ処理がインラインアセンブラで記述されている。下記は crt0.c を簡略化したもの。

```nasm
__start__:
        lui $sp, 0
        ori $sp, 0xff00
        li $gp, 0
        li $k0, 0x02000101
        mtc0 $k0, $12
	/* Initialize .data */
	/* Initialize .bss */
        j main
```

上記の lui や ori などは MIPS プロセッサの命令である。詳細は Wikipedia の[MIPS アーキテクチャ](https://ja.wikipedia.org/wiki/MIPSアーキテクチャ)を参照されたい。命令は 32-bit 長なので、0 番地目、4 番地目、8 番地目、12 番地目、...の順に命令が格納される。

まず、スタックポインタ（sp レジスタ）の初期化（命令 1 と 2）、gp レジスタの初期化（命令 3）、CPU の動作モード設定（命令 4 と 5）を行う。CPU の動作モード設定では割込みを許可している。

具体的なコードは省略しているが、その後、初期値を持つデータ領域や初期値を持たないデータ領域を初期化している（Initialize .data/.bss の部分）。最後に、j 命令によって C 言語の main()関数へジャンプしている。

このような main の前に呼ばれる初期化ルーチンは「C Run Time start up file」（別名 crt0）と呼ばれる。組込みシステムでは、一般的に、1) スタックポインタ、キャッシュ、メモリなど各種ハードウェアの初期化、2) 初期値を持つデータ領域の定数値での初期化、3) 初期値を持たないデータ領域の 0 初期化、4) main 関数の呼び出しなどを行う。詳細は crt0.c を見て欲しい。

### メモリアロケーション

本実験環境ではメインメモリは 16,384 words 分実装されている。ただし、実際にメモリとして利用可能な領域は 0x0000 番地から 0xff00 番地までなので 16,384 words より若干少ない。メインメモリ上に以下の領域が確保される。

- text 領域: 機械語命令。
- data 領域: 初期値を持つ global 変数、static 変数。
- rodata 領域: read-only なデータ。つまり定数。
- bss 領域: 初期値を持たない global 変数、static 変数（プログラム起動時に 0 に初期化される）。
- stack 領域: local 変数。関数コールの度にメモリ空間末尾から先頭方向へ伸びる。

プログラムのリンク時に、リンカスクリプトで指定された通りにメモリアロケーションが行われる。下記は soft/mips.ld を簡略化したもの（実物はもう少し複雑）。0x0000 番地目から text 領域が始まり、その直後に rodata、data、bss 領域が置かれる。スタックポインタはプログラム冒頭のインラインアセンブラでメインメモリの末尾に設定済みである。

```c
ENTRY(__start__)
SECTIONS
{
        .text   : { *(.text); }
        .rodata : { *(.rodata); . = ALIGN(4); }
        .data   : { *(.data); . = ALIGN(4); }
        .bss    : { *(.*bss); . = ALIGN(4); }
}
```

### クロス開発ツールの使い方

C 言語プログラム（test.c）がどのようにして機械語コード（program.dat）に変換されるか説明する。ここでは soft/Makefile に沿って説明していく。なお、本 soft/Makefile では SOURCE=test.c としている。ファイル名を変更したければ SOURCE の値を書き換えれば良い。

まず、C 言語プログラム（test.c）をコンパイルして、アセンブリ言語プログラム（program.asm）に変換する。コンパイルオプションは CFLAGS で定義している。最適化レベルは-O0（オー・ゼロ）としているので一切の最適化は行わない。

```make
        $(CC) $(CFLAGS) -c -S $(SOURCE) -o program.asm
```

次に、アセンブリ言語プログラム（program.asm）をオブジェクファイル（program.o）に変換する。オブジェクファイルは機械語で書かれた中間ファイルである。

```make
        $(AS) program.asm -o program.o
```

上述のリンカスクリプト（mips.ld）を用いてオブジェクファイル（program.o）をリンクし、実行ファイル（program.bin）を生成する。

```make
        $(LD) -T mips.ld program.o -o program.bin
```

通常、実行ファイル（program.bin）はプロセッサ上でそのまま実行できる。ただし、今回はこの実行ファイルを Verilog HDL で書かれた mips モジュールに組み込むという追加ステップが必要である。そこで、soft/Makefile では実行ファイル（program.bin）を逆アセンブルして、text、data、rodata、bss などの領域を 16 進数で書かれたテキストファイル（program.dump）に保存している。

このために逆アセンブル（objdump）や簡単なテキスト処理（grep、tr、awk）を行っている。逆アセンブルの部分を以下に示すが、残りのテキスト処理の部分は少々込み入っているので、興味がある人は soft/Makefile を確認されたい。

```make
        $(DUMP) -D --disassemble-zeroes program.bin > program.dump
```

ここまで読んだら test.c、program.asm、program.dump、program.dat をよく確認し、機械語コード生成の過程を理解して欲しい。

## ハードウェア部分の基礎知識

本実験環境において各種 I/O を制御するための周辺回路の仕組みを紹介する。各自、目を通しておいて欲しい。

### SWITCH と LED の制御回路の解説

FPGA チップから SWITCH、LED、LCD などの I/O を制御するには、FPGA チップの物理ピン番号と各種 I/O の対応付けが必要である。この対応付けを管理するのが制約ファイル（Xilinx Design Constraints File、略して XDC）である。さっそく hard/fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）を確認してみよう。

```sh
> cd hard
> less fpga.xdc
```

```tcl
##LEDs
set_property PACKAGE_PIN M14 [get_ports {led[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[0]}]
set_property PACKAGE_PIN M15 [get_ports {led[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[1]}]
set_property PACKAGE_PIN G14 [get_ports {led[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[2]}]
set_property PACKAGE_PIN D18 [get_ports {led[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {led[3]}]
...
```

上記は 4 個の LED と FPGA の物理ピンの対応付けを定義している。FPGA の各物理ピンは「M14」のように、アルファベットと数字を組み合わせた名前が付いている。例えば、FPGA の G14 ピンは LED の 3 つ目（led[2]）につながっている。FPGA の各物理ピンの先に何がつながっているかは FPGA ボードのマニュアルに書いてある。

同様に、4 個のスライドスイッチ（sw[3:0]）、4 個の押しボタン（btn[3:0]）、LCD 制御用信号（lcd[10:0]）、クロック（clk_125mhz）についても fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）を確認しよう。他にも、8-bit 汎用入出力ポート A、B、C（ioa[7:0]、iob[7:0]、ioc[7:0]）も実装されているが、これらについては後述する。

次に、fpga_top モジュールの入出力ポートを確認しよう。fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）に書かれていた信号名（led など）が fpga_top の入出力ポートとして宣言されていることが確認できる。

```sh
> less top.v
```

```verilog
module fpga_top (
        input                   clk_125mhz,
        input           [3:0]   sw,
        input           [3:0]   btn,
        output  reg     [3:0]   led,
        output  reg     [10:0]  lcd,
        output  reg     [7:0]   ioa,
        output  reg     [7:0]   iob
);
```

SWITCH と LED の制御回路を紹介する前に、まず、mips プロセッサの入出力ポートを説明する。hard/top.v の続きを見て欲しい。

```verilog
/* CPU module (@62.5MHz) */
mips mips (clk_62p5mhz, reset, pc, instr, {7'b0000000, irq}, memwrite,
        memtoregM, swc, byteen, dataadr, writedata, readdata, 1'b1, 1'b1);
```

主要な入出力ポートの使途を以下にまとめる。

- clk_62p5mhz: 入力ポート（1-bit）。FPGA ボードから供給される 125MHz クロック（clk_125mhz）を半分に分周してプロセッサに与える。
- pc: 出力ポート（32-bit）。プログラムカウンタ。命令を読み出すためのメモリ番地を出力。
- instr: 入力ポート（32-bit）。pc で指定したメモリ番地から読み出した MIPS I 命令。
- irq: 入力ポート（1-bit）。タイマー回路からの割込み信号。100msec に 1 回だけ 1 になる。
- memwrite: 出力ポート（1-bit）。メインメモリもしくは I/O に値をストアする際は 1 を出力。それ以外は 0 を出力。
- dataadr: 出力ポート（32-bit）。ストア対象のメモリ番地を出力。
- writedata: 出力ポート（32-bit）。ストアする書き込みデータ。
- readdata: 入力ポート（32-bit）。メインメモリもしくは I/O から読み出したデータの値。

メモリマップト I/O では、dataadr を基にロード/ストア対象がメインメモリなのか、SWITCH なのか、LED なのか、LCD なのか判別する必要がある。そのために、ここでは、ロード/ストア対象がメインメモリの場合は cs0 信号を 1 に、SWITCH の場合は cs1 信号を 1 に、LED の場合は cs2 信号を 1 に、LCD の場合は cs3 信号を 1 にしようと思う。このような判定を行う回路をアドレスデコーダと呼ぶ。以下にアドレスデコーダの例を示す。

```verilog
/* Memory(cs0), Switch(cs1), LED(cs2), LCD(cs3), and more ... */
assign  cs0     = dataadr <  32'hff00;
assign  cs1     = dataadr == 32'hff04;
assign  cs2     = dataadr == 32'hff08;
...
```

SWITCH（cs1）の値をロードするための回路は以下の通りである。8-bit の SWITCH の値（sw と btn）が readdata1 に継続代入されている（readata1 の上位 24-bit は 0 に固定）。cs1 が 1 のとき、readdata1 の値がプロセッサに与えられる。なお、readdata0 はメインメモリから読み出した値であり、cs0 が 1 のときは readdata0 の値がプロセッサに与えられる。

```verilog
assign  readdata        = cs0 ? readdata0 : cs1 ? readdata1 : 0;
...
/* cs1 */
assign  readdata1       = {24'h0, btn, sw};
```

LED（cs2）に値をストアするための回路は以下の通りである。プロセッサからの書き込み信号（memwrite）が 1、かつ、cs2 が 1 のとき、プロセッサからの書き込みデータの下位 4-bit（writedata[3:0]）を led に書き込む。

```verilog
/* cs2 */
always @ (posedge clk_62p5mhz or posedge reset)
        if (reset)                      led     <= 0;
        else if (cs2 && memwrite)       led     <= writedata[3:0];
```

## LCD 表示の基礎知識

本実験環境において LCD に文字等を表示するための方法を紹介する。各自、目を通しておいて欲しい。

### LCD の制御方法の概要

チュートリアル 2 で示した通り、LCD のメモリ番地は 0xff0c である。この番地は書き込み専用であり、この番地に 11-bit の制御コマンドやデータをストアすると LCD に文字が表示される。0 から 7-bit 目までが 8-bit データ、8-bit 目が RW 信号、9-bit 目が RS 信号、10-bit 目が E 信号である。上位 21-bit は無視される。

各信号の意味を以下にまとめる。

| 信号     | 説明                                                                                                                                                                                                                                                                                                |
| :------- | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| lcd[7:0] | LCD に送る 8-bit データ。この 8-bit データは、RS が 0 ならばコマンドとして扱われ、RS が 1 ならばデータとして扱われる。データは LCD に表示する ASCII コード（文字コード）である。例えば、0x41 は大文字の「A」に対応する。コマンドは LCD の制御コードである。例えば、0x01 は「Clear Display」である。 |
| lcd[8]   | LCD の RW 信号。書き込み時は 0、読み込み時は 1 に設定する。本実験では常に 0 とする。                                                                                                                                                                                                                |
| lcd[9]   | LCD の RS 信号。コマンドを送る場合は 0、文字コードを送る場合は 1。                                                                                                                                                                                                                                  |
| lcd[10]  | LCD の E 信号。この信号を 0 にして 40nsec 以上待ち、1 にして 230nsec 以上待ち、0 に戻して 10nsec 以上待つと、lcd[7:0]で指定した 8-bit データがボード上の LCD に取り込まれる。                                                                                                                       |

LCD の利用手順は下記の通り。起動時に LCD の初期化を行わなければならない。

- LCD の初期化。詳細は後述。
- 制御コマンドの書き込み。Function Set、Clear Display、Return Cursor Home などの制御を行う。
- ASCII コードの書き込み。画面に表示したい文字「0-9」「A-Z」「a-z」、各種記号。実はカタカナも出せる。
- 必要に応じて Step ii に戻る。例えば、改行してカーソルを 2 行目に移動させたり、Clear Display する場合など。

### LCD の制御方法の詳細

ボード上の LCD は、FPGA 上のプロセッサよりもずっと低い動作周波数で動いている。プロセッサの動作が速すぎるので適切に「待ち」を入れないと LCD がデータやコマンドを正しく受け取れない。

このような「待ち」を作るにはいろいろな方法があるが、精度は要らないのでここでは「空の for ループ」を使って時間を稼ぐ。プロセッサの動作周波数は 62.5MHz であるから 1 命令当たり 16nsec かかる。例えば、以下の for ループでは、ループ 1 回あたり 9 命令の処理（lw から nop まで）に翻訳される。つまり for ループ 1 回あたり 144nsec である。この特徴を利用して「待ち」を作ろう。

```c
/* C言語による空のforループ */
for (i = 0; i < 100; i++);
```

まずは LCD に制御コマンド（0x01、Clear Display）を送る例を紹介しよう。

1. LCD の番地（0xff0c）に対し値を書き込む。この値の下位 8-bit は 0x01、8-bit 目（RW 信号）は 0、9-bit 目（RS 信号）は 0、10-bit 目（E 信号）は 0 である。
2. 40nsec 待つ。空の for ループのループ回数は 1 回で良い。
3. LCD の番地（0xff0c）に対し値を書き込む。この値の下位 8-bit は 0x01、8-bit 目（RW 信号）は 0、9-bit 目（RS 信号）は 0、10-bit 目（E 信号）は 1 である。
4. 230nsec 待つ。空の for ループのループ回数は 2 回で良い。
5. LCD の番地（0xff0c）に対し値を書き込む。この値の下位 8-bit は 0x01、8-bit 目（RW 信号）は 0、9-bit 目（RS 信号）は 0、10-bit 目（E 信号）は 0 である。
6. 次のデータもしくはコマンドを送信するには 1.64msec 待つ必要がある。ループ回数は 11,389 回以上必要だ。

続いて、LCD に ASCII コード（0x41、大文字の「A」）を送る例を紹介しよう。

1. LCD の番地（0xff0c）に対し値を書き込む。この値の下位 8-bit は 0x41、8-bit 目（RW 信号）は 0、9-bit 目（RS 信号）は 1、10-bit 目（E 信号）は 0 である。
2. 40nsec 待つ。空の for ループのループ回数は 1 回で良い。
3. LCD の番地（0xff0c）に対し値を書き込む。この値の下位 8-bit は 0x41、8-bit 目（RW 信号）は 0、9-bit 目（RS 信号）は 1、10-bit 目（E 信号）は 1 である。
4. 230nsec 待つ。空の for ループのループ回数は 2 回で良い。
5. LCD の番地（0xff0c）に対し値を書き込む。この値の下位 8-bit は 0x41、8-bit 目（RW 信号）は 0、9-bit 目（RS 信号）は 1、10-bit 目（E 信号）は 0 である。
6. 次のデータもしくはコマンドを送信するには 40usec 待つ必要がある。ループ回数は 278 回以上必要だ。

LCD に表示する文字の文字コードを調べるには`man ascii`と打つ。例えば、「A」の 16 進数（Hex）表記は 0x41 である。「b」ならば 0x62 である。

主な制御コードは以下の通り。

- 0x01: Clear Display
- 0x02: Return Cursor Home
- 0x06: カーソル自動インクリメントモード（Entry Mode Set）
- 0x0c: Display On
- 0x38: 8-bit モード、2 行表示モード（Function Set。8-bit モードなら 4-bit 目を 1。2 行表示なら 3-bit 目を 1）

LCD の初期化シーケンスは以下の通り。これをプログラムの冒頭で 1 度だけ実行すること。初期化シーケンスが完了すると LCD が利用可能になる。

1. 15msec 待つ。空の for ループのループ回数は 104,167 回以上あれば良い。
1. Function Set コマンド（8-bit モード、2 行表示モード=0x38）を発行。
1. Entry Mode Set コマンド（カーソル自動インクリメント=0x06）を発行。
1. Display On コマンド（0x0c）を発行。
1. Clear Display コマンド（0x01）を発行。
1. 1.5msec 待つ。ループ回数は 10,417 回必要。

以上で初期化は終了。以降、LCD に ASCII コードを表示できる。

### LCD の制御関数

LCD にコマンドを送るための C 言語関数は下記の通り。

- lcd_init(): LCD の初期化を行う。上述の「LCD の初期化シーケンス」を参考に「待ち」を入れたり、コマンド（0x38、0x06、0x0c、0x01）を発行する。
- lcd_cmd(int cmd): 8-bit の LCD 制御コマンドを LCD の番地に書き込む。
- lcd_data(int data): 8-bit の ASCII コードを LCD の番地に書き込む。
- lcd_str(unsigned char \*str): 文字列 str から 1 文字ずつ連続して LCD の番地に書き込む。

上記の解説とソースコードを見比べよう。例えば、lcd_data()内の「E=0,RS=1,RW=0」の行において、なぜ 0x00000200 と論理和を取っているのかよく考えよう。2 進数の 010 は 16 進数では 2 である。2 進数の 110 は 16 進数では 6 である。

```c
void lcd_wait(int n)
{
        int i;
        for (i = 0; i < n; i++);
}
void lcd_cmd(unsigned char cmd)
{
        /* E, RS, RW, DB[7:0] */
        volatile int *lcd_ptr = (int *)0xff0c;
        *lcd_ptr = (0x000000ff & cmd) | 0x00000000; /* E=0,RS=0,RW=0 */
        lcd_wait(1);
        *lcd_ptr = (0x000000ff & cmd) | 0x00000400; /* E=1,RS=0,RW=0 */
        lcd_wait(2);
        *lcd_ptr = (0x000000ff & cmd) | 0x00000000; /* E=0,RS=0,RW=0 */
        lcd_wait(11389);
}
void lcd_data(unsigned char data)
{
        /* E, RS, RW, DB[7:0] */
        volatile int *lcd_ptr = (int *)0xff0c;
        *lcd_ptr = (0x000000ff & data) | 0x00000200; /* E=0,RS=1,RW=0 */
        lcd_wait(1);
        *lcd_ptr = (0x000000ff & data) | 0x00000600; /* E=1,RS=1,RW=0 */
        lcd_wait(2);
        *lcd_ptr = (0x000000ff & data) | 0x00000200; /* E=0,RS=1,RW=0 */
        lcd_wait(278);
}
void lcd_init()
{
        lcd_wait(104167);
        lcd_cmd(0x38);          /* 8-bit, 2-line mode */
        lcd_cmd(0x06);          /* Cursor auto increment */
        lcd_cmd(0x0c);          /* Display ON */
        lcd_cmd(0x01);          /* Clear display */
        lcd_wait(10417);
}
void lcd_str(unsigned char *str)
{
        while (*str != 0x00)
                lcd_data(*str++);
}
void main()
{
        lcd_init();
        lcd_str("mog mog");
}
```

## 演習 1（ハードウェア）

「ハードウェア部分の基礎知識」で紹介した SWITCH および LED 回路を参考に、LCD 制御回路を作ろう。

LCD を制御するための物理ピンは 11 本（11-bit）である。0 から 7-bit 目までが 8-bit データ、8-bit 目が RW 信号、9-bit 目が RS 信号、10-bit 目が E 信号である。各信号の意味は「LCD 表示の基礎知識」を参照。

LCD を制御するための物理ピン番号は、すでに hard/fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）に書かれているので修正は不要である。参考までに fpga.xdc の該当箇所を以下に示す。**新 FPGA ボード**の fpga_z7.xdc はピン名が若干違うがどちらにせよ修正する必要は無い。

```tcl
set_property PACKAGE_PIN T20 [get_ports {lcd[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[0]}]
set_property PACKAGE_PIN U20 [get_ports {lcd[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[1]}]
set_property PACKAGE_PIN V20 [get_ports {lcd[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[2]}]
set_property PACKAGE_PIN W20 [get_ports {lcd[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[3]}]
set_property PACKAGE_PIN Y18 [get_ports {lcd[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[4]}]
set_property PACKAGE_PIN Y19 [get_ports {lcd[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[5]}]
set_property PACKAGE_PIN W18 [get_ports {lcd[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[6]}]
set_property PACKAGE_PIN W19 [get_ports {lcd[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[7]}]

set_property PACKAGE_PIN Y14 [get_ports {lcd[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[8]}]
set_property PACKAGE_PIN W14 [get_ports {lcd[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[9]}]
set_property PACKAGE_PIN T12 [get_ports {lcd[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd[10]}]
```

演習 1（ハードウェア）では hard/top.v を修正する。具体的には、1) fpga_top の中で cs3 のためにアドレスデコーダを修正し、2) cs3 と memwrite が有効であるときに lcd に writedata（ビット幅に注意）を書き込むようにする。

拡張ボードは 8-bit 汎用入出力ポート A、B、C に挿入して使うことになる。fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）については ioa[7:0]、iob[7:0]、ioc[7:0]はすでに定義されているので修正は不要である。hard/top.v については適宜修正する必要がある。サンプルコードでは ioa[7:0]と iob[7:0]は出力ポート扱いとなっているので、入力ポートとして使用したければ「output reg」を「input」に変更する。必要に応じて、入力もしくは出力ポートとして ioc[7:0]も追加する。アドレスデコーダとしては cs4、cs5、cs6 を割り当てると良い。入力ポートとして使用する場合は cs1、出力ポートとして使用する場合は cs2 の回路を参考にすると良い。以下に参考例を示す。

```verilog
module fpga_top (
        input                   clk_125mhz,
        input           [3:0]   sw,
        input           [3:0]   btn,
        output  reg     [3:0]   led,
        output  reg     [10:0]  lcd,
        //output reg    [7:0]   ioa,
        input           [7:0]   ioa,
        output  reg     [7:0]   iob
);
wire    [31:0]  pc, instr, readdata, readdata0, readdata1, readdata4, writedata, dataadr;
...
assign  readdata        = cs0 ? readdata0 : cs1 ? readdata1 : cs4 ? readdata4 : 0;
...
/* cs4 */
//always @ (posedge clk_62p5mhz or posedge reset)
//      if (reset)                      ioa     <= 0;
//      else if (cs4 && memwrite)       ioa     <= writedata[7:0];
assign  readdata4       = {24'h0, ioa};
```

## 演習 2（ソフトウェア）

押しボタン、LED、LCD を使うゲームプログラムを実機上で動かしてみよう。ソースコードを読みながらこれはどのようなゲームなのか考えてみよう。

LCD 関連の関数は空欄になっているが、ここは「LCD 表示の基礎知識」で解説したコードを適宜コピーしてこよう。
実機で上記のコードを試すには、演習 1（ハードウェア）の通り hard/top.v を修正したうえで、Xilinx Vivado を用いた論理合成からやり直す必要がある。

実機に焼き込んだ後は押しボタン 0 と 1 を同時に押してプロセッサをリセットしよう。実機で動作を確認できたら、自分たちが作るゲームの構想を練って欲しい。

```c
/* Do not remove the following line. Do not remove interrupt_handler(). */
#include "crt0.c"

void show_ball(int pos);
void play();
int  btn_check_0();
int  btn_check_1();
int  btn_check_3();
void led_set(int data);
void led_blink();
void lcd_wait(int n);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init();

#define INIT	0
#define OPENING	1
#define PLAY	2
#define ENDING	3

int state = INIT, pos = 0;

/* interrupt_handler() is called every 100msec */
void interrupt_handler()
{
	static int cnt;
	if (state == INIT) {
	} else if (state == OPENING) {
		cnt = 0;
	} else if (state == PLAY) {
		/* Display a ball */
		pos = (cnt < 16) ? cnt : 31 - cnt;
		show_ball(pos);
		if (++cnt >= 32) {
			cnt = 0;
		}
	} else if (state == ENDING) {
	}
}
void main()
{
	while (1) {
		if (state == INIT) {
			lcd_init();
			state = OPENING;
		} else if (state == OPENING) {
			state = PLAY;
		} else if (state == PLAY) {
			play();
			state = ENDING;
		} else if (state == ENDING) {
			state = OPENING;
		}
	}
}
void play()
{
	while (1) {
		/* Button0 is pushed when the ball is in the left edge */
		if (pos == 0 && btn_check_0()) {
			led_blink();	/* Blink LEDs when hit */
		/* Button3 is pushed when the ball is in the right edge */
		} else if (pos == 15 && btn_check_3()) {
			led_blink();	/* Blink LEDs when hit */
		} else if (btn_check_1()) {
			break;		/* Stop the game */
		}
	}
}
void show_ball(int pos)
{
	lcd_cmd(0x01);		/* Clear display */
	lcd_cmd(0x80 + pos);	/* Set cursor position */
	lcd_data('*');
}
/*
 * Switch functions
 */
int btn_check_0()
{
	volatile int *sw_ptr = (int *)0xff04;;
	return (*sw_ptr & 0x10) ? 1 : 0;
}
int btn_check_1()
{
	volatile int *sw_ptr = (int *)0xff04;;
	return (*sw_ptr & 0x20) ? 1 : 0;
}
int btn_check_3()
{
	volatile int *sw_ptr = (int *)0xff04;;
	return (*sw_ptr & 0x80) ? 1 : 0;
}
/*
 * LED functions
 */
void led_set(int data)
{
	volatile int *led_ptr = (int *)0xff08;
	*led_ptr = data;
}
void led_blink()
{
	int i;
	led_set(0xf);			/* Turn on */
	for (i = 0; i < 300000; i++);	/* Wait */
	led_set(0x0);			/* Turn off */
	for (i = 0; i < 300000; i++);	/* Wait */
	led_set(0xf);			/* Turn on */
	for (i = 0; i < 300000; i++);	/* Wait */
	led_set(0x0);			/* Turn off */
}
/*
 * LCD functions
 */
void lcd_wait(int n)
{
	/* Not implemented yet */
}
void lcd_cmd(unsigned char cmd)
{
	/* Not implemented yet */
}
void lcd_data(unsigned char data)
{
	/* Not implemented yet */
}
void lcd_init()
{
	/* Not implemented yet */
}
```

以下にいくつか追加のヒントを紹介する。

- lcd_data() を使って LCD に日本語カタカナも表示できる。LCD に表示可能な文字一覧は「Digilent PmodCLP Reference Manual」の 2.4 節に書いてある。
- 自分で定義したドット絵（カスタム文字）を LCD に表示することもできる。詳しくは付録 C の「便利なサンプルコード集」を参照されたい。
- LCD の制御コマンドとして「LCD の任意の位置にカーソルを移動する」コマンドもある。詳しくは付録 C の「便利なサンプルコード集」を参照されたい。

## ディスカッション

教員とのディスカッションは 2 週目の午前中に行う。班の中でゲーム機の構想を練ったうえで、適当な紙に外部仕様と内部仕様をまとめる。ディスカッションでは、まず外部仕様と内部仕様が書かれた紙を提出し、計画について口頭でプレゼンする。

### 内部仕様と外部仕様

外部仕様: ユーザ向けの仕様。ゲームのルールや開発する製品がどのような操作に対してどのように動くかを詳細に決める。設計者の間で動き方の理解に違いがあるとバグの原因になる。例えば、「〇〇ボタンを押すとゲームがスタートする」「〇〇ボタンを押すとモグラを叩く」「LCD の上段にモグラを表示する」「LED に残機数を表示する」「残機が 0 になるとゲームオーバー」「ヒットすると振動する」等々。箇条書きで良い。

内部仕様: どのように実装するか、中身の仕様。データ構造やプログラムの流れなどソフトウェアの仕様を書く。データ構造については、どのような C 言語変数（例えば、スコア、残機数、速度）を使うか決める。プログラムの流れについては、下記のリンクを参考に疑似コード、もしくは、フローチャート（流れ図）を描いて説明する。例えば、「スタートボタンの入力待ち」→「モグラを表示」→「叩くボタンの入力待ち」→「ヒットしたらスコアをインクリメント、ミスしたら残機をデクリメント」→「残機が 0 ならばゲームオーバー、そうでなければモグラ表示に戻る」等々。計画した機能が実装できるかどうかの自己確認にもなるので、プログラムの流れ図は可能な限り詳細に描くこと。

### 理解度を確認するための質問

実験で使っているシステムには OS もスレッドライブラリもない。このようなシステムにおいて、例えばテニスゲームならば、「画面のボールを動かしつつ、ユーザからの入力を待ち、もし入力があればラケットを振る」という同時処理が必要である。

これを実現するにはどのようなプログラムを書けば良いだろうか？自分の言葉で説明して欲しい。

### ヒント

以下のプログラムを考えて欲しい。

```c
/* interrupt_handler() is called every 100msec */
void interrupt_handler()
{
        ボールの位置を動かす処理;
        画面を更新する処理;
}
void main()
{
        初期化;
        while (1) {
                if (ボタン0が押された) {
                        ボタンが0が押されたときの処理;
                } else if (ボタン1が押された) {
                        ボタンが1が押されたときの処理;
                }
        }
}
```

ここでは画面として 2 行 16 文字の LCD を使う。画面の表示パターンは配列 buf[2][16]で管理し、値が 1 のマスにボールを表示するものとする。

「ボールの位置を動かす処理」は時間に応じて buf の値を更新する。例えば、buf[0][5]=1 だったものを 100msec 後に buf[0][6]=1 にすればボールが右に 1 マス移動したことになる。

「画面を更新する処理」は buf のパターンに応じて画面を更新する。例えば、buf の値が 1 のマスに「\*」を表示し、それ以外のマスには「 」を表示する。

「ボタン 0 が押されたときの処理」は buf のパターンに応じてラケットにボールが当たったかどうかの当たり判定を行う。「ボタン 1 が押されたときの処理」も大体一緒である。

必ずしもこのように作る必要はないが、このようにすることで「画面のボールを動かしつつ、ユーザからの入力を待ち、もし入力があればラケットを振る」という同時処理を簡単に実現できる。詳細は「演習 2（ソフトウェア）」を読んで欲しい。

## 最終レポートに関する注意

最終レポートの内容は内部仕様と外部仕様である。2 週目のディスカッションのときよりも詳細に書くこと。分量の目安は A4 サイズ 4 ページ以上。内容が少ないとプロジェクトへの貢献が少ないと判断され、不当に低い点数になってしまうので注意。自分の貢献を積極的にアピールして欲しい。

外部仕様は A4 サイズ 1 ページ以上。ゲームのルール（スコアの加点条件、ゲームの終了条件等）、画面の見方（スコア、残機数等）、スイッチ/ボタンの意味（スタートボタン、叩くボタン等）など。画面の見方やスイッチ/ボタンの意味は図や実機の写真を使って説明すると良い。内容は班の中で共通化して良い。

内部仕様の 1 ページ目には詳細なフローチャートを入れること。大ざっぱなフローチャートは減点する。このフローチャートはプログラム中のすべての関数（すべての処理）を網羅すること。フローチャート自体は班の中で共通化して良いが、自分が貢献した部分についてはフローチャートに色付けするなどして強調して欲しい。色付けする箇所が班の中で被っても良い。実際、複数人で協力して 1 つのモジュールを実装することはよくある事である。

内部仕様の 2 ページ目以降は、自分が貢献した部分（フローチャートに色付けした箇所）を詳細に解説して欲しい（A4 サイズ 2 ページ以上）。ソースコードもしくは疑似コードを適宜含めて、自分の貢献を積極的にアピールして欲しい。ハードウェア実装を頑張った場合は Verilog HDL コードを含めて説明すると良い。拡張ボードの実装に力を注いだ場合は拡張ボードの写真もレポートに含めると良い。繰り返しになるが、内容が少ないとプロジェクトへの貢献が少ないと判断され、不当に低い点数になってしまうので注意。

締切までにレポートを提出すること。遅れると減点もしくは受け取ってもらえない。実験アンケートには必ず回答すること。

## 付録 A: ハードウェア記述言語 Verilog HDL

ハードウェア記述言語 Verilog HDL は、天野先生の計算機構成同演習（B2 秋学期）、コンピュータアーキテクチャ（B3 春学期）で扱っている。

実験中に Verilog HDL の文法を確認したくなったら、授業のホームページにある「Verilog HDL 入門」（VLSI 設計演習、B4 春学期、担当:松谷）を適宜参照して欲しい。Verilog HDL に触れるのは今回が初めてという人は一通り読んでおいたほうが良い。

Verilog HDL シミュレーションには Icarus Verilog を使用する。テストベクタのファイル名を add_test.v、回路のファイル名を add.v とするとき、以下のように打つと Verilog HDL ソースがコンパイルされる。Verilog HDL ファイルが複数個ある場合はすべて指定すること。テストベクタはファイルリストの最初に指定する。

```sh
> iverilog test_add.v add.v
```

実際のシミュレーションは以下のように実行する。

```sh
> vvp a.out
```

iverilog コマンドのオプションは以下の通り。

- -h : ヘルプを表示。
- -o <ファイル名> : 出力ファイル名を指定。デフォルトでは a.out。
- -I <パス名> : インクルードパスに追加するディレクトリを指定。
- -D <マクロ名>(=<値>) : define を追加。値はなくても良い。

テストベクタに以下のような記述を入れておくと、シミュレーション中の信号変化を波形として生成できる。例えば、sim/test.v などを参照。この場合、test.fpga というインスタンスの信号変化を、dump.vcd という名前のファイルに記録する。

```verilog
initial begin
       $dumpfile("dump.vcd");
       $dumpvars(0, test.fpga);
end
```

波形ビューワ（GTKWave）を起動するには以下のように打つ。

```sh
> gtkwave dump.vcd &
```

## 付録 B: プログラミング言語 C

演算子とその優先順位を以下にまとめる。文法については、市販の教科書もしくはウェブ等を参照。

| 優先順位 | 演算子                                                                                                                             |
| :------- | :--------------------------------------------------------------------------------------------------------------------------------- | --------------------- |
| 1        | ( )　　[ ]　　間接メンバアクセス ->　　直接メンバアクセス .　　後置型インクリメント ++　　後置型デクリメント --                    |
| 2        | 論理否定 !　　ビット否定 ~　　前置型インクリメント ++　　前置型デクリメント --　　ポインタ演算子 \*　　ポインタ演算子 &　　 sizeof |
| 3        | キャスト (型)                                                                                                                      |
| 4        | 乗算 \*　　除算 /　　剰余 %                                                                                                        |
| 5        | 足し算 +　　引き算 -                                                                                                               |
| 6        | 左シフト <<　　右シフト >>                                                                                                         |
| 7        | 関係演算子 <　　<=　　>　　>=                                                                                                      |
| 8        | 等値演算子 ==　　!=                                                                                                                |
| 9        | ビット積 &                                                                                                                         |
| 10       | ビット排他的論理和 ^                                                                                                               |
| 11       | ビット和 \|                                                                                                                        |
| 12       | 論理積 &&                                                                                                                          |
| 13       | 論理和 \|\|                                                                                                                        |
| 14       | 条件演算子 ?:                                                                                                                      |
| 15       | 代入演算子 =　　+=　　-=　　\*=　　/=　　%=　　&=　　                                                                              | =　　^=　　<<=　　>>= |
| 16       | カンマ演算子 ,                                                                                                                     |

## 付録 C: 便利なサンプルコード集

以下のサンプルコードでは省略しているが、プログラムの先頭には crt0.c のインクルード、及び、interrupt_handler()が必要。

### LCD（16 文字 2 段）の任意の位置に文字を表示したい

この LCD では上から 1 段目に書き続けても自動的に 2 段目に改行されない。例えば、2 段目左端に文字を表示したければ、次に示す方法でカーソルを 2 段目左端に移動させたうえで、lcd_str()もしくは lcd_data()を用いて文字を表示すれば良い。

```c
/* 1段目左から0番目にカーソルを移動 */
lcd_cmd(0x80);
/* 1段目左から1番目にカーソルを移動 */
lcd_cmd(0x81);
...
/* 1段目左から15番目にカーソルを移動 */
lcd_cmd(0x8f);

/* 2段目左から0番目にカーソルを移動 */
lcd_cmd(0xc0);
/* 2段目左から1番目にカーソルを移動 */
lcd_cmd(0xc1);
...
/* 2段目左から15番目にカーソルを移動 */
lcd_cmd(0xcf);
```

ちなみに、上記 LCD の上位互換品として「20 文字 4 段」バージョンもある。3 段目と 4 段目にカーソルを移動するには下記のようにする。

```c
/* 1段目左から0番目にカーソルを移動 */
lcd_cmd(0x80);
/* 2段目左から0番目にカーソルを移動 */
lcd_cmd(0xc0);
/* 3段目左から0番目にカーソルを移動 */
lcd_cmd(0x94);
/* 4段目左から0番目にカーソルを移動 */
lcd_cmd(0xd4);
```

### 数値（スコア等）を LCD に表示したい

以下の lcd_digit3()は引数で与えた 3 桁の数字を LCD に表示する関数である。

```c
void lcd_digit3(unsigned int val)
{
       int digit3, digit2, digit1;
       digit3 = (val < 100) ? ' ' : ((val % 1000) / 100) + '0';
       digit2 = (val <  10) ? ' ' : ((val %  100) /  10) + '0';
       digit1 = (val %  10) + '0';
       lcd_data(digit3);
       lcd_data(digit2);
       lcd_data(digit1);
}
void main()
{
       lcd_init();

       lcd_digit3(9876);
       lcd_data(' ');
       lcd_digit3(432);
       lcd_data(' ');
       lcd_digit3(65);
       lcd_data(' ');
       lcd_digit3(7);
}
```

### LCD を使ってアニメーションを表示したい

LCD の左から右に「abcdefghijklmnop」という文字列が流れる。

```c
void main()
{
       int i, j;
       unsigned char data[16] = "abcdefghijklmnop";

       lcd_init();

       for (i = 0; i < 16; i++) {
               /* Return Cursor Home */
               lcd_cmd(0x02);
               /* Display 16 characters in data */
               for (j = 0; j < 16; j++)
                       lcd_data(data[j]);
               /* Shift */
               for (j = 0; j < 16 - 1; j++)
                       data[j] = data[j + 1];
               data[j] = ' ';
               /* Wait for a while */
               lcd_wait(1000000);
       }
}
```

### LCD に自分で定義したドット絵を表示したい

横 5 ドット、縦 7 ドットの「ドット絵」を使ってカスタム文字を表現できる。

カスタム文字のパターンは CGRAM というメモリ領域に保存する。CGRAM は 0 番地から 7 番地まで利用できるので、最大 8 個のカスタム文字を定義できることになる。

下記の例では、bitmap で示すカスタム文字のパターン（横 5 ドット、縦 7 ドット）を CGRAM の 3 番地に保存している。次に lcd_data(CGRAM の番地)を呼び出すことでこのカスタム文字を表示している。

```c
void lcd_customchar(unsigned int addr, unsigned int *bitmap)
{
       lcd_cmd((addr << 3) | 0x40); /* Set CGRAM address */
       lcd_data(bitmap[0]);
       lcd_data(bitmap[1]);
       lcd_data(bitmap[2]);
       lcd_data(bitmap[3]);
       lcd_data(bitmap[4]);
       lcd_data(bitmap[5]);
       lcd_data(bitmap[6]);
       lcd_data(0x00);              /* Last line is used by cursor */
       lcd_cmd(0x80);               /* Set DDRAM address (write to display) */
}
void main()
{
       unsigned int bitmap[7] = {      0x15, /* 10101 */
                                       0x0a, /* 01010 */
                                       0x15, /* 10101 */
                                       0x0a, /* 01010 */
                                       0x15, /* 10101 */
                                       0x0a, /* 01010 */
                                       0x15, /* 10101 */
                                };
       lcd_init();
       /* New character is defined and stored in CGRAM address 0x03 */
       lcd_customchar(0x03, bitmap);
       /* Display the character stored in CGRAM address 0x03 */
       lcd_data(0x03);
}
```

### ブザーで音階を作りたい（ソフトウェア版）

8 音から成る音階をソフトウェアで作ってみた。出来はあまり良くないので各自で調整して欲しい。

ここではブザーは汎用入出力ポート B につながっているものとする。入出力ポート B の番地は 0xff14 としているが、班によっては違う番地を使っている可能性がある。必要に応じて修正して欲しい。

```c
void beep(int mode)
{
        int len;
        volatile int *iob_ptr = (int *)0xff14;
        switch (mode) {
        case 1: len = 13304; break;
        case 2: len = 11851; break;
        case 3: len = 10554; break;
        case 4: len =  9949; break;
        case 5: len =  8880; break;
        case 6: len =  7891; break;
        case 7: len =  7029; break;
        case 8: len =  6639; break;
        }
        *iob_ptr = 1;
        lcd_wait(len);
        *iob_ptr = 0;
        lcd_wait(len);
}
void main()
{
        int i;
        while (1) {
                for (i = 0; i < 250; i++) beep(1);
                for (i = 0; i < 250; i++) beep(2);
                for (i = 0; i < 250; i++) beep(3);
                for (i = 0; i < 250; i++) beep(4);
                for (i = 0; i < 250; i++) beep(5);
                for (i = 0; i < 250; i++) beep(6);
                for (i = 0; i < 250; i++) beep(7);
                for (i = 0; i < 250; i++) beep(8);
        }
}
```

しかし、このソフトウェア版には音を鳴らしている最中に他の処理ができないという致命的な欠点がある。例えば、音を鳴らしている最中に画面を更新するようなことはできない。この問題は次のハードウェア版で解決する。

### ブザーで音階を作りたい（ハードウェア版）

13 音から成る音階のハードウェア版である。top.v に修正が必要であるが、先にプログラムがどのようになるかを示しておく。汎用入出力ポート B の番地（ここでは 0xff14）に音階を書き込んだ後は別の処理ができるという点が利点である。

```c
void main()
{
        volatile int *iob_ptr = (int *)0xff14;
        *iob_ptr = 1; lcd_wait(7000000);
        *iob_ptr = 2; lcd_wait(7000000);
        *iob_ptr = 3; lcd_wait(7000000);
        *iob_ptr = 4; lcd_wait(7000000);
        *iob_ptr = 5; lcd_wait(7000000);
        *iob_ptr = 6; lcd_wait(7000000);
        *iob_ptr = 7; lcd_wait(7000000);
        *iob_ptr = 8; lcd_wait(7000000);
        *iob_ptr = 9; lcd_wait(7000000);
        *iob_ptr = 10; lcd_wait(7000000);
        *iob_ptr = 11; lcd_wait(7000000);
        *iob_ptr = 12; lcd_wait(7000000);
        *iob_ptr = 13; lcd_wait(7000000);
}
```

このために hard/top.v にブザーを制御するための beep モジュールを追加する。beep モジュールは fpga_top モジュールの endmodule の後に追加すると良い。assign 文中の定数は、TA さんが何度もブザー音を聞きながらドレミファソラシドに近づけたものである。

```verilog
module beep
(
       input clk_62p5mhz,
       input reset,
       input [7:0] mode,
       output buzz
);
reg  [31:0] count;
wire [31:0] interval;
assign interval =      (mode ==  1) ? 14931 * 2: /* C  */
                       (mode ==  2) ? 14093 * 2: /* C# */
                       (mode ==  3) ? 13302 * 2: /* D  */
                       (mode ==  4) ? 12555 * 2: /* D# */
                       (mode ==  5) ? 11850 * 2: /* E  */
                       (mode ==  6) ? 11185 * 2: /* F  */
                       (mode ==  7) ? 10558 * 2: /* F# */
                       (mode ==  8) ?  9965 * 2: /* G  */
                       (mode ==  9) ?  9406 * 2: /* G# */
                       (mode == 10) ?  8878 * 2: /* A  */
                       (mode == 11) ?  8380 * 2: /* A# */
                       (mode == 12) ?  7909 * 2: /* B  */
                       (mode == 13) ?  7465 * 2: /* C  */
		       0;
assign buzz = (mode > 0) && (count < interval / 2) ? 1 : 0;
always @ (posedge clk_62p5mhz or posedge reset)
       if (reset)
               count   <= 0;
       else if (mode > 0)
               if (count < interval)
                       count   <= count + 1;
               else
                       count   <= 0;
endmodule
```

fpga_top モジュール中からこの beep モジュールにアクセスしたい。そのために fpga_top モジュール中に beep モジュールのインスタンスを追加する。さらに beep モジュールが使用する reg や wire も追加する。

```verilog
reg     [7:0]   mode;
wire            buzz;
beep beep (clk_62p5mhz, reset, mode, buzz);
```

汎用入出力ポート B が cs4 空間に割り当てられている場合、mode および iob の制御は次のようになる。

```verilog
always @ (posedge clk_62p5mhz or posedge reset)
        if (reset)                      mode    <= 0;
        else if (cs4 && memwrite)       mode    <= writedata[7:0];
always @ (posedge clk_62p5mhz or posedge reset)
        if (reset)                      iob     <= 0;
        else                            iob[0]  <= buzz;
```

### キーパッドを使いたい

キーパッドには 4 行 4 列の 16 個のボタンがある。1 行目は左から 1、2、3、A。2 行目は左から 4、5、6、B。3 行目は左から 7、8、9、C。4 行目は左から 0、F、E、D に対応している。

キーパッドの使い方はやや特殊である。8-bit ピンのうち、下位 4-bit を出力、上位 4-bit を入力ポートとして使う。
ここでは、汎用入出力ポート B にキーパッドがつながっていると仮定し、その下位を iob_lo、上位を iob_hi とする（汎用入出力ポート A を使う場合は ioa_lo、ioa_hi になる）。

まず、ボタンが押されているか知りたい列を 0、そうでない列を 1 として、4-bit の値を作る。例えば、ボタン 1、4、7、0 のどれかが押されているか知りたい場合は 0111 となる。ボタン 3、6、9、E のどれかがが押されているか知りたい場合は 1101 となる。これを iob_lo に出力し、数サイクル待つ。

次に、iob_hi の値を読み取る。読み取った 4-bit の値は「指定した列」の行の値に対応している。例えば、値が 0111 ならば 1 行目、値が 1101 ならば 3 行目が押されているということになる。

以下にサンプルコードを示す。kypd_scan()は押されているボタンの数字に対応した値が戻り値として返される。

```c
int kypd_scan() {
        int i;
        volatile int *iob_ptr = (int *)0xff14;

        *iob_ptr = 0x07;                /* 0111 */
        for (i = 0; i < 1; i++);        /* Wait */
        if ((*iob_ptr & 0x80) == 0)
                return 0x1;
        if ((*iob_ptr & 0x40) == 0)
                return 0x4;
        if ((*iob_ptr & 0x20) == 0)
                return 0x7;
        if ((*iob_ptr & 0x10) == 0)
                return 0x0;

        *iob_ptr = 0x0b;                /* 1011 */
        for (i = 0; i < 1; i++);        /* Wait */
        if ((*iob_ptr & 0x80) == 0)
                return 0x2;
        if ((*iob_ptr & 0x40) == 0)
                return 0x5;
        if ((*iob_ptr & 0x20) == 0)
                return 0x8;
        if ((*iob_ptr & 0x10) == 0)
                return 0xf;

        *iob_ptr = 0x0d;                /* 1101 */
        for (i = 0; i < 1; i++);        /* Wait */
        if ((*iob_ptr & 0x80) == 0)
                return 0x3;
        if ((*iob_ptr & 0x40) == 0)
                return 0x6;
        if ((*iob_ptr & 0x20) == 0)
                return 0x9;
        if ((*iob_ptr & 0x10) == 0)
                return 0xe;

        *iob_ptr = 0x0e;                /* 1110 */
        for (i = 0; i < 1; i++);        /* Wait */
        if ((*iob_ptr & 0x80) == 0)
                return 0xa;
        if ((*iob_ptr & 0x40) == 0)
                return 0xb;
        if ((*iob_ptr & 0x20) == 0)
                return 0xc;
        if ((*iob_ptr & 0x10) == 0)
                return 0xd;

        return 0;
}
void main()
{
        volatile int *led_ptr = (int *)0xff08;
        for (;;)
		*led_ptr = kypd_scan();
}
```

これを実現するには、汎用入出力ポート iob[7:0]を iob_lo[3:0]と iob_hi[3:0]に分ける必要がある。これには hard/fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）の該当箇所を以下のように修正する。

```tcl
##Pmod Header JE
set_property PACKAGE_PIN V12 [get_ports {iob_lo[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_lo[0]}]
set_property PACKAGE_PIN W16 [get_ports {iob_lo[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_lo[1]}]
set_property PACKAGE_PIN J15 [get_ports {iob_lo[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_lo[2]}]
set_property PACKAGE_PIN H15 [get_ports {iob_lo[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_lo[3]}]

set_property PACKAGE_PIN V13 [get_ports {iob_hi[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_hi[0]}]
set_property PACKAGE_PIN U17 [get_ports {iob_hi[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_hi[1]}]
set_property PACKAGE_PIN T17 [get_ports {iob_hi[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_hi[2]}]
set_property PACKAGE_PIN Y17 [get_ports {iob_hi[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {iob_hi[3]}]
```

hard/top.v の入出力ポート宣言は下記のように修正する。

```verilog
module fpga_top (
        input                   clk_125mhz,
        input           [3:0]   sw,
        input           [3:0]   btn,
        output  reg     [3:0]   led,
        output  reg     [10:0]  lcd,
        output  reg     [7:0]   ioa,
        //output reg    [7:0]   iob
        output  reg     [3:0]   iob_lo,
        input           [3:0]   iob_hi
);
```

汎用入出力ポート B が cs4 空間に割り当てられている場合、iob_lo と iob_hi の制御は下記の通り。これに加え、cs4 と readdata4 による readdata の制御も必要になるので忘れずに。

```verilog
/* cs4 */
assign  readdata4       = {24'h0, iob_hi, iob_lo};
always @ (posedge clk_62p5mhz or posedge reset)
        if (reset)                      iob_lo  <= 0;
        else if (cs4 && memwrite)       iob_lo  <= writedata[3:0];
```

### ロータリーエンコーダを使いたい

ロータリーエンコーダには回転スイッチとスライドスイッチが実装されていて、回転スイッチのシャフトは押しボタンにもなっている。回転スイッチのシャフトは時計回り、反時計回りに回る。

ロータリーエンコーダ自体の出力は 4-bit で、ここでは汎用入出力ポート C（つまり、ioc）の上段 4-bit にロータリーエンコーダがつながっていると仮定する。
4-bit のうち、最初の 2-bit（A と B）の位相差によって回転スイッチのシャフトが時計回りか反時計回りかを判定し、次の 2-bit にはシャフトの押しボタンとスライドスイッチがつながっている。

シャフトの回転方向を判定する部分は専用回路として作る。
hard/top.v にロータリーエンコーダを制御するための rotary_enc モジュールを追加する。次のような rotary_enc モジュールを fpga_top モジュールの endmodule の後に追加すると良い。

```verilog
module rotary_enc
(
	input clk_62p5mhz,
	input reset,
	input [3:0] rte_in,
	output [9:0] rte_out
);
reg	[7:0]	count;
wire		A, B;
reg		prevA, prevB;
assign	{B, A} = rte_in[1:0];
assign	rte_out	= {count, rte_in[3:2]};
always @ (posedge clk_62p5mhz or posedge reset)
	if (reset) begin
		count	<= 128;
		prevA	<= 0;
		prevB	<= 0;
	end else
		case ({prevA, A, prevB, B})
		4'b0100: begin
			count <= count + 1;
			prevA <= A;
		end
		4'b1101: begin
			count <= count + 1;
			prevB <= B;
		end
		4'b1011: begin
			count <= count + 1;
			prevA <= A;
		end
		4'b0010: begin
			count <= count + 1;
			prevB <= B;
		end
		4'b0001: begin
			count <= count - 1;
			prevB <= B;
		end
		4'b0111: begin
			count <= count - 1;
			prevA <= A;
		end
		4'b1110: begin
			count <= count - 1;
			prevB <= B;
		end
		4'b1000: begin
			count <= count - 1;
			prevA <= A;
		end
		endcase
endmodule
```

rotary_enc モジュールの入力（rte_in）はロータリーエンコーダ本体の 4-bit で、rotary_enc モジュールの出力（rte_out）は 10-bit である。

rte_out[9:0]のうち最初の 2-bit は、シャフトの押しボタン（rte_in[3]）とスライドスイッチ（rte_in[2]）がそのままつながっている。
次の 8-bit はカウンタ出力になっていて、初期値が 128 で、シャフトが反時計回りに回されるとカウンタの値が増え、時計回りに回されるとカウンタの値が減る。
なお、カウンタは 8-bit なので、カウンタの値が 255 のときに反時計回りに回すとカウンタの値は 0 に戻り、カウンタの値が 0 のときに時計回りに回すとカウンタの値は 255 になってしまう点に注意。

fpga_top モジュール中からこの rotary_enc モジュールにアクセスしたい。
汎用入出力ポート C（つまり、ioc）にロータリーエンコーダがつながっていると仮定するので、hard/top.v の入出力ポート宣言は下記のように修正する。

```verilog
module fpga_top (
	input			clk_125mhz,
	input		[3:0]	sw,
	input		[3:0]	btn,
	output 	reg	[3:0]	led,
	output 	reg	[10:0]	lcd,
＜中略＞
	input		[7:0]	ioc
);
```

次に fpga_top モジュール中に rotary_enc モジュールのインスタンスを追加する。さらに rotary_enc モジュールが使用する wire も追加する。
ここでは汎用入出力ポート C が cs6 空間に割り当てられているものとする。

```verilog
wire	[31:0]	readdata6;
wire		cs6;
wire	[9:0]	rte;
rotary_enc rotary_enc (clk_62p5mhz, reset, ioc, rte);
```

rotary_enc モジュールの出力（rte_out）は 10-bit なので、これを readdata6 につなぐ。これに加え、cs6 と readdata6 による readdata の制御も必要になるので忘れずに。

```verilog
/* cs6 */
assign	readdata6	= {22'h0, rte};
```

以下にソフトウェア側のサンプルコードを示す。
ここでは入出力ポート C の番地は 0xff18 としているが、ここは必要に応じて修正して欲しい。

```c
void main()
{
	volatile int *led_ptr = (int *)0xff08;
	volatile int *rte_ptr  = (int *)0xff18;
	lcd_init();
	for (;;) {
		*led_ptr = (*rte_ptr) & 0x3;
		lcd_cmd(0x80);	/* Cursor first line */
		lcd_digit3((*rte_ptr) >> 2);
	}
}
```

シャフトの押しボタンとスライドスイッチのオンオフ状態を LED の下位 2-bit に出している。
rotary_enc モジュールの 8-bit カウンタの値は LCD に表示している。値の表示には上述の lcd_digit3()関数を使っている。

### タイマー割込みの頻度を変えたい

hard/top.v 中の timer モジュールのカウンタの最大値を変更すれば良い。
動作周波数は 62.5MHz（1 クロックは 16nsec）である。
デフォルトでは 100msec 間隔で irq を 1 にするため、カウンタの最大値は 6250000 になっている。

```verilog
module timer (
        input                   clk, reset,
        output                  irq
);
reg     [22:0]  counter;

assign  irq = (counter == 23'd6250000);

always @ (posedge clk or posedge reset)
        if (reset)                      counter <= 0;
        else if (counter < 23'd6250000) counter <= counter + 1;
        else                            counter <= 0;
endmodule
```

### LCD を 2 つ使いたい

ここでは ioa と iob に 2 つ目の LCD を接続する。fpga_top モジュールのポート名は lcd2 とする。

この場合、fpga_top モジュールの汎用入出力ポート ioa と iob を削除して、出力ポート lcd2 を追加する。
また、hard/fpga.xdc（**新 FPGA ボード**の場合は fpga_z7.xdc）の該当箇所を以下のように修正する。

```tcl
##Pmod Header JD
set_property PACKAGE_PIN T14 [get_ports {lcd2[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[0]}]
set_property PACKAGE_PIN T15 [get_ports {lcd2[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[1]}]
set_property PACKAGE_PIN P14 [get_ports {lcd2[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[2]}]
set_property PACKAGE_PIN R14 [get_ports {lcd2[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[3]}]
set_property PACKAGE_PIN U14 [get_ports {lcd2[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[4]}]
set_property PACKAGE_PIN U15 [get_ports {lcd2[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[5]}]
set_property PACKAGE_PIN V17 [get_ports {lcd2[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[6]}]
set_property PACKAGE_PIN V18 [get_ports {lcd2[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[7]}]

##Pmod Header JE
set_property PACKAGE_PIN U17 [get_ports {lcd2[8]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[8]}]
set_property PACKAGE_PIN V13 [get_ports {lcd2[9]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[9]}]
set_property PACKAGE_PIN T17 [get_ports {lcd2[10]}]
set_property IOSTANDARD LVCMOS33 [get_ports {lcd2[10]}]
```

### RGB LED を使いたい（新 FPGA ボード限定）

**新 FPGA ボード**のひとは他と若干の違いがあり恐縮だが、良い点としてはボード上に RGB LED が 2 個実装されている。

ポート名は led5 と led6。それぞれ 3-bit 幅で、下位から順に R、G、B に対応する。3'b000 は消灯なので計 8 種類の色で光らせることができる。

hard/top.v の入出力ポート宣言は下記のように修正する。

```verilog
module fpga_top (
        input                   clk_125mhz,
        input           [3:0]   sw,
        input           [3:0]   btn,
        output  reg     [3:0]   led,
        output  reg     [2:0]   led5,
        output  reg     [2:0]   led6,
＜中略＞
);
```

### 浮動小数点数を使いたい

float 型や double 型変数のこと。結論から言うと現状では使えない。

本実験で使用しているプロセッサは教育用の簡易版であり、浮動小数点演算ユニットを装備していない。ソフトウェアによる浮動小数点ライブラリをリンクするという代案もあるが、現状ではまだ移植できていない。

## 付録 D: 実験環境の基礎知識

本実験では FPGA ボードとして Digilent 社の Zybo Zynq-7000 ARM/FPGA SoC トレーナーボードを使用している。以下は、このボードの公式マニュアルである。

- https://digilent.com/reference/programmable-logic/zybo/reference-manual

本実験で使用する LCD の高度な使い方に興味がある場合は以下のマニュアルを読むと良い。

- https://digilent.com/reference/pmod/pmodclp/reference-manual

16 ボタンキーパッドのマニュアルは以下にある。付録 C「便利なサンプルコード集」の kypd_scan()関数はこれを見て実装した。

- https://digilent.com/reference/pmod/pmodkypd/reference-manual

ロータリーエンコーダのマニュアルは以下にある。付録 C「便利なサンプルコード集」の rotary_enc モジュールはこれを見て実装した。

- https://digilent.com/reference/pmod/pmodenc/reference-manual

MIPS R2000 互換プロセッサの命令セットについては下記のリンクが参考になる。アセンブリ言語コードを見ながらのデバッグで、もし知らない命令が出てきたら適宜参照しよう。「7 MIPS アセンブリ言語」および「8 コンパイラのレジスタ使用規則」は一度目を通しておくと良い。

- https://ja.wikipedia.org/wiki/MIPSアーキテクチャ

本実験では Xilinx 社の Zynq Z-7010 という FPGA を使っている。Zynq 7000 シリーズについては下記のリンクをたどると良い。

- https://japan.xilinx.com/products/silicon-devices/soc/zynq-7000.html

## 付録 E: シミュレーション

実機を用いて動作確認するには、毎回、論理合成、配置配線、FPGA の再構成が必要であり、どうしても時間がかかってしまう。実は、実機を使わずにシミュレータ上でプログラムの動作確認を行うこともできる。ここではその方法を紹介する。

シミュレーションのために必要なファイルは以下の通り。

| ディレクトリ名 | ファイル名 | 内容                                                           |
| :------------- | :--------- | :------------------------------------------------------------- |
| sim/           |            | Verilog HDL のシミュレーションは sim/以下で行う                |
|                | Makefile   | make と打つとシミュレーションを行う                            |
|                | test.v     | シミュレーション用のテストベクタ（シミュレーション内容を記述） |

### チュートリアル

ここでは、「チュートリアル 1」で生成した機械語コード program.dat 使って Verilog HDL シミュレータの使い方を紹介する。sim/ディレクトリに移動して make と打つと、シミュレーションが開始する。

```sh
> cd sim
> make
```

```verilog
Clk     0  SWITCH value changed to      0000
Clk     0  BUTTON value changed to      0000
Clk     0  BUTTON value changed to      1111
Clk     0  LCD    value changed to      000_0000_0000
Clk     0  LED    value changed to      0000
Clk     6  BUTTON value changed to      0000
Clk    32  Writing value        x to address 0x0000fefc
Clk    38  Writing value    65284 to address 0x0000fef0
Clk    42  Writing value    65288 to address 0x0000fef4
Clk    54  Writing value        0 to address 0x0000ff08
Clk    56  SWITCH value changed to      0101
Clk    56  BUTTON value changed to      0110
Clk    70  Writing value      101 to address 0x0000ff08
Clk    70  LED    value changed to      0101
Clk    86  Writing value      101 to address 0x0000ff08
Clk   102  Writing value      101 to address 0x0000ff08
Clk   106  SWITCH value changed to      0000
Clk   106  BUTTON value changed to      0000
Clk   118  Writing value        0 to address 0x0000ff08
Clk   118  LED    value changed to      0000
...
```

シミュレーションのテストベクタは sim/test.v である。ここでは、メモリや I/O への書き込み（sw 命令）を監視していて、書き込みがあればログに表示する。

test.01.c では、SWITCH の番地（0xff04）から読み込んだ値を LED の番地（0xff08）に書き込み続けている。ログを見て欲しい。例えば、56 サイクル目で SWITCH と BUTTON の番地にそれぞれ 0101 と 0110 という値を書き込み、70 サイクル目で LED の値が 0101 に変化している。

ログとにらめっこすることでプログラムの挙動をある程度把握できるが、この例ではストア命令（sw）以外のイベントを捉えられないので内部の挙動を把握することはできない。余程自信があるときを除き、通常は波形を見ながらデバッグしたほうが効率が良い。以下のようにして波形ビューワ（GTKWave）を立ち上げよう。引数の dump.vcd は信号の変化を記録したファイルであり、シミュレーションを実行する度に生成される。

```sh
> gtkwave dump.vcd &
```

### モジュールおよびファイル構成

Verilog HDL シミュレーション、および、FPGA ボードを使った実機動作確認では使用するファイルが若干異なるのでここで整理しておこう。

- まず、hard/以下の top.v、mips.v、parts.v、multdiv.v は共通。soft/以下の program.dat も共通である。
- シミュレーションでは、fpga_top モジュール（FPGA 上の回路）への SWITCH や BUTTON 操作、および、LED や LCD の監視は sim/test.v に記述されている通りに実行される。
- 実機動作確認では、fpga_top モジュール（FPGA 上の回路）への SWITCH や BUTTON 操作、および、LED や LCD の監視は人間が行う。

シミュレータ上で動作確認するときは、まず sim/test.v を修正することでゲーム機に対して SWITCH や BUTTON 操作を与え、波形を見ながら動作確認を行う。

### シミュレータを用いたデバッグの実際

シミュレータ上で動作確認するには、まず sim/test.v を修正して SWITCH や BUTTON 操作のイベントを発生させ、正しく動作しているか確かめる。

```sh
> less sim/test.v
```

```verilog
＜中略＞
`timescale 1ns/1ps
module test;
parameter       STEP = 8;       /* System clock is 125MHz (8nsec) */
＜中略＞
/* Generate clock to sequence tests */
always #(STEP/2)
       clk <= ~clk;
...
```

シミュレーションの時間単位は 1nsec とした。STEP には 8 を代入しているので、4nsec ごとに clk の値が反転する。clk の 0 から 1 への立ち上がりイベントは 8nsec 周期で発生するから、動作クロックは 125MHz である。ちなみに、hard/top.v を見れば分かる通り、メインメモリ（FPGA の内蔵 BlockRAM）には 125MHz クロックを与え、mips プロセッサやそれ以外の周辺回路にはその半分の 62.5MHz クロックを与えている。FPGA の内蔵 BlockRAM とプロセッサの動作タイミングの都合によりこのような実装にした。

```verilog
initial begin
        clk     <= 0;
        counter <= 0;
        sw      <= 4'b0000;
        btn     <= 4'b0000;
        #(STEP/4);
        btn     <= 4'b1111;             /* Reset ON */
        #(STEP*6);
        btn     <= 4'b0000;             /* Reset OFF */

        #(STEP*50);
        sw      <= 4'b0101;
        btn     <= 4'b0110;
        #(STEP*50);
        sw      <= 4'b0000;
        btn     <= 4'b0000;

        /* Specify the simulation time. Default value is (STEP*1000) nsec.
         * If it is not enough, longer value should be specified. */
        #(STEP*1000);
        $finish;
end
```

上記の記述によって、決められた時間に SWITCH 操作を発生させている。具体的には、

- まず reset（ここでは 4 個の btn 同時押し）してから、6 STEP 待ち、reset を解除している。これによって mips プロセッサを含むすべての周辺回路が初期化され、一斉に動作を再開する。
- さらに 50 STEP 経った時点で、sw に 0101、btn に 0110 という値が書き込まれる。これはスライドスイッチの 0-bit 目と 2-bit 目、押しボタンの 1-bit 目と 2-bit 目が押されたという意味である。
- さらに 50 STEP 経った時点で、sw と btn に 0 が書き込まれる。これは全スイッチがオフになったという意味である。
- その後、1,000 STEP 進めてからシミュレーションを finish している。もしシミュレーション時間が 1,000 STEP では足りない場合は「#(STEP\*1000);」の値をもっとずっと大きくすれば良い。

この挙動はシミュレーション結果のログと一致するはずである。チュートリアル 1 のシミュレーション結果と見比べて欲しい。

```sh
> cd sim
> make
```

```verilog
Clk     0  SWITCH value changed to      0000
Clk     0  BUTTON value changed to      0000
＜中略＞
Clk    56  SWITCH value changed to      0101
Clk    56  BUTTON value changed to      0110
＜中略＞
Clk   106  SWITCH value changed to      0000
Clk   106  BUTTON value changed to      0000
...
```

開発中のゲームにおいて、例えば、1 つ目の押しボタンを「スタートボタン」として使うとする。シミュレーションによる動作確認のために「100 STEP 目にスタートボタンが押され、150 STEP 目に離される」などのイベントを発生させたくなると思う。このような場合は test.v に SWITCH の操作イベントを追記していけば良い。

デバッグの役に立つように、メインメモリ、SWITCH、BUTTON、LED、LCD の値が変わるとシミュレーション時にログが出るようにしている。詳しくは以下の display 文を参照。

```verilog
/* Monitoring memory write operations */
always @ (posedge fpga.clk_62p5mhz)
        if (fpga.memwrite) begin
                $display("Clk %5d  Writing value %8d to address 0x%h",
                                counter, fpga.writedata, fpga.dataadr);
        end
/* Monitoring Switches */
always @ (sw)
        $display("Clk %5d  SWITCH value changed to\t%4b", counter, sw);
/* Monitoring Buttons */
always @ (btn)
        $display("Clk %5d  BUTTON value changed to\t%4b", counter, btn);
/* Monitoring LED */
always @ (led)
        $display("Clk %5d  LED    value changed to\t%4b", counter, led);
/* Monitoring LCD */
always @ (lcd)
        $display("Clk %5d  LCD    value changed to\t%3b_%4b_%4b", counter,
                        lcd[10:8], lcd[7:4], lcd[3:0]);
```

このような「printf デバッグ」はお手軽ではあるが、外部への書き込み以外の挙動はまったく見えないのでとんだ勘違いを起こすことがある。波形を見ながらデバッグするほうが良いだろう。

## 付録 F: 環境構築

本実験で使用するツールは、MIPS 用クロス開発ツール（gcc、binutils）、FPGA 用合成・配置配線ツール（Xilinx Vivado）などである。シミュレーションを行うには Verilog HDL シミュレータ（Icarus Verilog）、波形ビューワ（GTKWave）も必要になる。

これらはすでに ITC で利用可能であるため、新しくインストールする必要はない。参考までにクロス開発環境のインストール方法を以下に書き記す。

### クロス開発環境の構築

MIPS I 命令セットアーキテクチャ用の C コンパイラ（gcc）、バイナリツール（as、ld、objdump）をインストールする。バイトオーダは big endian とする。
この例では、インストール先は/home/md401/aa205875/usr/としている。ここの部分は各自の環境に合わせて適宜修正すること。

まずは、バイナリツール（binutils）をインストールする。

```sh
> wget https://ftp.gnu.org/gnu/binutils/binutils-2.30.tar.gz
> tar zxvf binutils-2.30.tar.gz
> cd binutils-2.30
> mkdir build_mips && cd build_mips
> ../configure --target=mips --prefix=/home/md401/aa205875/usr
> make
> make install
```

次に任意精度数演算ライブラリ（gmp）をインストールする。これは C コンパイラ（gcc）をコンパイルするために必要である。

```sh
> wget https://ftp.gnu.org/gnu/gmp/gmp-4.3.2.tar.gz
> tar zxvf gmp-4.3.2.tar.gz
> cd gmp-4.3.2
> ./configure --prefix=/home/md401/aa205875/usr
> make
> make install
```

高品質多倍長浮動小数点ライブラリ（mpfr）をインストールする。こちらも gcc をコンパイルするために必要である。

```sh
> wget https://ftp.gnu.org/gnu/mpfr/mpfr-3.1.2.tar.gz
> tar zxvf mpfr-3.1.2.tar.gz
> ./configure --prefix=/home/md401/aa205875/usr \
	--with-gmp=/home/md401/aa205875/usr
> make
> make install
```

複素数ライブラリ（mpc）をインストールする。こちらも gcc をコンパイルするために必要である。

```sh
> wget https://ftp.gnu.org/gnu/mpc/mpc-1.0.1.tar.gz
> tar zxvf mpc-1.0.1.tar.gz
> ./configure --prefix=/home/md401/aa205875/usr \
	--with-gmp=/home/md401/aa205875/usr \
	--with-mpfr=/home/md401/aa205875/usr
> make
> make install
```

ここで、やっと C コンパイラ（gcc）のインストールに入る。configure スクリプトの引数として gmp、mpfr、mpc ライブラリのパスを指定する必要がある。マシンによってはコンパイルには 1 時間以上かかる。

```sh
> wget https://ftp.gnu.org/gnu/gcc/gcc-7.4.0/gcc-7.4.0.tar.gz
> tar zxvf gcc-7.4.0.tar.gz
> cd gcc-7.4.0
> mkdir build_mips && cd build_mips
> ../configure --target=mips --without-fp --enable-languages="c" \
       --disable-libssp --prefix=/home/md401/aa205875/usr \
	--with-gmp=/home/md401/aa205875/usr \
	--with-mpfr=/home/md401/aa205875/usr \
	--with-mpc=/home/md401/aa205875/usr
> make -j4
> make install
```

### 環境変数の設定

開発ツールを使えるようにするために、シェルの設定ファイル（.bashrc）を修正し、環境変数 PATH を設定する。

```sh
> emacs .bashrc
＜末尾に以下を追加＞
export PATH=/home/md401/aa205875/usr/bin/:${PATH}
> source .bashrc
```
