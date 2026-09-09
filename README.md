# felica-reader-rcs660s-pico

<img width="600" src="https://dl.dropboxusercontent.com/scl/fi/2i863wezc1q5qlecbgah5/felica_rcs660s.jpg?rlkey=0jb1o9xmfampcucey2dbe89b8&st=mkuer817&raw=1">

FeliCa リーダー・ライターを使用したSuica/PASMO履歴リーダです。

USBケーブルでPaspberry Pi Picoとパソコンと接続し、履歴データを表示させることができます。I2C接続LCDを付ければ、現在の残高を表示します。残高表示は、以下のFelica対応ICカードに対応しています。

* 交通系ICカード: Suica / PASMO / ICOCA / TOICA / Kitaca / SUGOCA / nimoca / はやかけん / manaca / PiTaPa / SAPICA / PASPY / AOPASS / iGUCA / Iwate Green Pass / odeca / cherica / totra / Welcome Suica / ecomayca
* 電子マネー: PASMO / 楽天Edy / nanaco / WAON

また、AS-289R2プリンタシールドを接続すると、Suica/PASMO履歴データを印字することができます。

# 使用した機材

* Paspberry Pi Pico  
https://www.switch-science.com/catalog/6900/
* PIMORONI Tiny 2040  
https://www.switch-science.com/products/7615
* FeliCa リーダー・ライター RC-S660/S  
https://www.switch-science.com/products/9660/
* FeliCa RC-S620S/RC-S730 ピッチ変換基板のセット(フラットケーブル付き)  
https://www.switch-science.com/catalog/1029/
* I2C接続の小型LCD搭載ボード(3.3V版)  
https://www.switch-science.com/catalog/1405/
* AS-289R2プリンタシールド  
https://www.switch-science.com/catalog/2553/

※ RC-S660/S は必須ですが、I2C LCD や AS-289R2 はオプションです。LCDやプリンタシールドを接続しない最小構成でも、シリアルコンソールから履歴確認が可能です。

# デバイスとの接続

Pasberry Pi Picoと他の部品は以下のように接続してください。

|Rasberry Pi Pico|Tiny 2040|RC-S660/S|I2C LCD|AS-289R2|
|---|---|---|---|---|
|GPIO4 (pin 6)|GPIO4 (pin 12)|||RxD1 (D1)|
|VSYS (pin 39)|VBUS (pin 1)|||5V|
|3V3(OUT) (pin 36)|3V3(OUT) (pin 3)|VDD (pin 1)|VDD (pin 2)||
|GND (pin 38)|GND (pin 8)|GND (pin 4)|GND (pin 3)|GND|
|GPIO16 (pin 21)|GPIO0 (pin 16)|RXD (pin 3)|||
|GPIO17 (pin 22)|GPIO1 (pin 15)|TXD (pin 2)|||
|GPIO14 (pin 19)|GPIO26 (pin 7)||SDA (pin 4)||
|GPIO15 (pin 20)|GPIO27 (pin 6)||SCL (pin 5)||

# 必要なツールのインストール

## ツールチェインのインストール
以下の情報を参照して、ビルドに使用するツールチェインをインストールしてください。
https://pip-assets.raspberrypi.com/categories/610-raspberry-pi-pico/documents/RP-008276-DS-1-getting-started-with-pico.pdf

## pico-sdk
以下のサイトを参考にしてください。  
https://github.com/raspberrypi/pico-sdk

## picotool
以下のサイトを参考にしてください。  
https://github.com/raspberrypi/picotool

# プログラムのビルドと書き込み

## コマンドライン（cmake）でビルドする
以下のコマンドでリポジトリをクローンして、ビルドします。

### リポジトリのクローン

```bash
$ git clone https://github.com/toyowata/felica-reader-rcs660s-pico
$ cd felica-reader-rcs660s-pico
$ mkdir build && cd build
```

### ビルド

PICO_SDK_PATHにPICO SDKをインストールしたパスを設定します。

```
$ export PICO_SDK_PATH=<pico-sdk-path>
```

ボード名を指定して、ビルドします。  
Raspberry Pi Pico の場合
```bash
$ cmake .. -GNinja -DPICO_BOARD=pico
$ ninja
```

Raspberry Pi Pico 2 の場合
```bash
$ cmake .. -GNinja -DPICO_BOARD=pico2
$ ninja
```

PIMORONI Tiny 2040 の場合
```bash
$ cmake .. -GNinja -DPICO_BOARD=pimoroni_tiny2040
$ ninja
```

## Raspberry Pi Pico に書き込む
BOOTSELモードに設定し（基板上のボタンを押したまま電源を入れる）、以下のコマンドを実行します。

```bash
$ picotool load ./felica_reader_pico.uf2
```

## プログラムの実行

プログラム書き込み後、リセットを行うとプログラムが起動します。  
TeraTerm, CoolTerm等のシリアルターミナルソフトウェアでパソコンと接続します（115200,8,N,1）。日本語を表示するので、UTF-8が表示できるモードに設定してください。  
FeliCa リーダー・ライター上にSuicaなどの交通系ICカードを乗せると、履歴情報が表示されます。

履歴情報の例

```

*** RC-S660/S FeliCaリーダープログラム ***

IDm: 0101-xxxx-yyyy-zzzz

15 07 00 00 2C CB 00 00 00 00 DC 05 00 00 01 00 
機種種別: 券売機等
利用種別: 新規
処理日付: 2022/06/11
残額: 1500円

15 02 00 00 2C CB 26 C2 00 00 C4 09 00 00 02 00 
機種種別: 券売機等
利用種別: SFチャージ
千歳線 新千歳空港駅
処理日付: 2022/06/11
残額: 2500円

16 01 00 02 2C CB 86 40 87 34 CA 08 00 00 04 F0 
機種種別: 自動改札機
利用種別: 自動改札出場
入出場種別: 出場
地下鉄南北線 大通駅 - 地下鉄東西線 琴似駅
処理日付: 2022/06/11
残額: 2250円

C7 46 00 00 2C CC 3A 0C 4F 97 34 08 00 00 05 00 
機種種別: 物販端末
利用種別: 物販
処理日付: 2022/06/12 07:16:12
残額: 2100円

1D 01 00 02 2C CC FA 0D FA 01 48 06 00 00 07 00 
機種種別: 連絡改札機
利用種別: 自動改札出場
入出場種別: 出場
東京モノレール羽田空港線 羽田空港第2ビル駅 - 東京モノレール羽田空港線 浜松町駅
処理日付: 2022/06/12
残額: 1608円
```

## 注意点と既知の問題

### プリンタシールド（AS-289R2）の使用
デフォルトでは、プリント出力を行わないスタブクラス`AS298R2_STUB`が有効になっています。プリンタを使用する場合は、`main.cpp`を以下のように変更してください。

```cpp
//AS289R2_STUB tp(AS289R2_UART_TX, AS289R2_UART_RX, uart1);
AS289R2 tp(AS289R2_UART_TX, AS289R2_UART_RX, uart1);
```

### リセット方法
リセット用のタクトスイッチを付けると便利です。  
https://nuneno.cocolog-nifty.com/blog/2021/03/post-c5ccb6.html

### サイバネコード
駅データのサイバネコードは、以下のサイトのデータを使用させていただきました。  
https://github.com/MasanoriYONO/StationCode
https://ja.ysrl.org/atc/station-code.html

※リポジトリには生成済みの`sc_utf8.h`が同梱されているため、通常は再変換の作業は不要です

このデータから必要な項目だけを抽出し、csv形式からバイナリ形式に変更を行っています。変換用のツールは以下に公開しました。  
https://github.com/toyowata/csv2bin

### 制約事項
誤動作を防ぐために、同じカードを連続して読み込むことはできません。同じカードを読み込む場合は、リセットを行ってください。

## Acknowledgements
Parts of this project were developed with assistance from Google Antigravity.

## ライセンス
Apache License 2.0