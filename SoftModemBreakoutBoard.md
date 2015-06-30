# SoftModemインターフェースボード #
<a href='http://www.flickr.com/photos/arms22/4823665444/' title='SoftModem BreakOut Board by arms22, on Flickr'><img src='http://farm5.static.flickr.com/4123/4823665444_39baf725cd.jpg' alt='SoftModem BreakOut Board' width='500' height='333' /></a>

iPhoneのオーディオ端子を使ってArduinoと通信するためのインターフェースボードです。

iPhoneからArduinoへ、ArduinoからiPhoneへデータを送ることができます。通信速度は1225bpsです（※実際のデータ転送レートはもう少し低いです）。センサー情報やスイッチ入力など少量のデータ通信に向いています。

半完成品です。ピンヘッダもしくはピンソケットをハンダづけして使います。

寸法：25 x 25mm

[スイッチサイエンス/商品詳細:SoftModemインターフェースボード](http://www.switch-science.com/products/detail.php?product_id=364)

## 応用例 ##

AndroidでSoftModemを動かしています: [AndroidでSoftModemを試してみた](http://side2.256byte.com/2010/08/android-softmodem/)

Android用のSoftmodem Terminalアプリケーションです: [Softmodem Terminal for Android](http://side2.256byte.com/application/softmodem-terminal/)

Androidで一眼レフカメラのシャッターを制御しています: [Androidで一眼レフカメラのシャッターを制御する](http://side2.256byte.com/2010/09/android-shutter-control/)

JavaScriptで通信用の音声ファイルを作成、再生させてデータ通信を行っています: [SoftModemインターフェースボードとJavaScriptで通信](http://d.hatena.ne.jp/NeoCat/20100822/1282486171)

RFID Tagリーダーを製作されています: [iPhone RFID Tag Reader](http://mkroll.mobi/hardware/rfid_tag_reader/rfid_tag_reader.html)


## 制限 ##

  * オーディオ端子を使った通信のため、時々データが壊れることがあります。チェックサムやリトライ処理など組み込むことを検討してください。
  * iPhoneの音量は最大にして使って下さい。

## 接続 ##
iPhoneとの接続には4極オスオスケーブルをお使いください。ケーブルによってはクロストークが発生し、通信エラーの原因になります。金メッキ端子のシールドされたケーブルをお使いください。ケーブルの長さは短ければ短いほどよいです。

Arduinoとは次のように接続します（右側がArduinoの端子）。
  * FSKOUT→D3
  * GND→GND
  * VCC→VCC
  * FSKIN→D6
  * AIN1→D7

**ボリューム調整**

Arduinoの電源電圧に合わせて調整します。AIN1の電圧をテスターで計り、(VCC / 2) + 300mVになるよう調節します。
  * 5V電源の場合、2.7V
  * 3.3V電源の場合、1.95V

## Arduinoライブラリ: SoftModem ##

[SoftModem-004.zip](http://code.google.com/p/arms22/downloads/detail?name=SoftModem-004.zip)

**サンプルスケッチ**
```
#include <SoftModem.h>
#include <ctype.h>

SoftModem modem;

void setup()
{
  Serial.begin(57600);
  delay(1000);
  modem.begin();
}

void loop()
{
  while(modem.available()){
    int c = modem.read();
    if(isprint(c)){
      Serial.println((char)c);
    }
    else{
      Serial.print("(");
      Serial.print(c,HEX);
      Serial.println(")");      
    }
  }
  if(Serial.available()){
    modem.write(0xff);
    while(Serial.available()){
      char c = Serial.read();
      modem.write(c);
    }
  }
}
```

**注意事項**
  * ライブラリ内でハードウェアタイマ2、アナログコンパレータを使用しています。その為、これら機能をアプリケーション側で使うことはできません。
  * tone関数は使えません。

## iPhoneアプリ: SoftModemTerminal.app ##

[SoftModemTerminal-004.zip](http://code.google.com/p/arms22/downloads/detail?name=SoftModemTerminal-004.zip)

SoftModemTerminalはシンプルなiPhone用のターミナルアプリです。前項のサンプルスケッチを書き込んだArduinoと英数字の送受信が行えます。iPhoneのオーディオ端子に何も接続していないと内蔵スピーカーから大きな音がでるので注意してください。

~~現在、SoftModemTerminalはAppStoreで配布していません。ソースコードをダウンロードし、ご自身でビルドして使って下さい。~~

SoftModemTerminalのビルドにはXcodeとiPhone SDKが必要です。XcodeはMacOSXのインストールDVDに収録されています。またAppleの開発者ページでもダウンロードできます。

iPhone SDKはAppleの開発者ページで開発者登録を行うとダウンロードできます。アプリケーションをiPhone実機で動作させるには有償のiPhoneデベロッパプログラム（年間￥10,800）に参加する必要があります。

### 音響電文 ###

SoftModemTerminal.appがAppStoreからダウンロードできるようになりました。iPhoneアプリケーションの開発環境をお持ちでないかたは次のURLからアプリケーションをダウンロードしてください。

[http://itunes.apple.com/jp/app/id385096654?mt=8](http://itunes.apple.com/jp/app/id385096654?mt=8)

## iPhoneソースコードの使用方法 ##

現在、iPhone側のSoftModemのソースコードはフレームワーク化されていません。あなたのプロジェクトでSoftModemを使いたい場合、SoftModemTerminalからSoftModemに関連するソースコードをコピーしてくる必要があります。次のリストはSoftModemに関連するソースコードの一覧です。これらのソースコードをプロジェクトフォルダにコピーしてください。
  * AudioQueueObject.h
  * AudioQueueObject.m
  * AudioSignalAnalyzer.h
  * AudioSignalAnalyzer.m
  * AudioSignalGenerator.h
  * AudioSignalGenerator.m
  * CharReceiver.h
  * FSKModemConfig.h
  * FSKByteQueue.h
  * FSKRecognizer.h
  * FSKRecognizer.mm
  * FSKSerialGenerator.h
  * FSKSerialGenerator.m
  * lockfree.h
  * MultiDelegate.h
  * MultiDelegate.m
  * PatternRecognizer.h

SoftModemは音声の入出力に次の2つのフレームワークを使います。これらフレームワームをプロジェクトに追加してください。
  * AudioToolbox.framework
  * AVFoundation.framework

**初期化**

まず最初にAVAudioSessionクラス使って、アプリケーションのカテゴリを設定します。音声の録音と再生を行うので AVAudioSessionCategoryPlayAndRecord を設定します。
```
AVAudioSession *session = [AVAudioSession sharedInstance];
session.delegate = self;
[session setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
[session setActive:YES error:nil];
```
次に音声解析用のAudioSignalAnalyzerクラス、FSKRecognizerクラスのインスタンスを生成します。AudioSignalAnalyzer はマイクから入力された波形を解析し、波形の上がり立ち下がりエッジを検出します。FSKRecognizer は AudioSignalAnalyzer が解析した結果を元にデータビットを復元します。
```
recognizer = [[FSKRecognizer alloc] init];
analyzer = [[AudioSignalAnalyzer alloc] init];
[analyzer addRecognizer:recognizer]; //analyzerにrecognizerを登録
[analyzer record]; //音声解析開始
```
次に音声出力用の FSKSerialGeneratorクラスのインスタンスを生成します。FSKSerialGenerator はデータビットを音声信号に変換し、出力します。
```
generator = [[FSKSerialGenerator alloc] init];
[generator play]; // 音声出力開始
```

**受信**

データの受信は CharReceiver プロトコルを実装したクラスを FSKRecognizer クラスに登録することで行います。
```
@interface MainViewController : UIViewController <CharReceiver>
```
初期化のタイミングで FSKRecognizer クラスに登録します。
```
MainViewController *mainViewController;
[recognizer addReceiver:mainViewController];
```
データを1バイト受信すると receivedChar: メソッドが呼ばれます。
```
- (void) receivedChar:(char)input
{
    //受信処理
}
```

**送信**

送信は受信に比べてずっと簡単で、FSKSerialGenerator クラスの writeByte: メソッドで1バイト送信します。
```
[generator writeByte:0xff];
```

## ドキュメント: ##
  * [回路図](http://www.flickr.com/photos/arms22/4821347734)
  * [Eagle Files](http://code.google.com/p/arms22/downloads/detail?name=iPhoneSoftModem%20BreakOut_EAGLE_v1.zip)

## その他の情報 ##
  * [なんでも作っちゃう、かも。SoftModem](http://arms22.blog91.fc2.com/?tag=SoftModem)
  * [Reinforce Lab - iPhone ソフトウェアモデム](http://sites.google.com/a/reinforce-lab.com/www/projects/iphone-software-modem)


---



|<div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/B0025Y6C5G/nandemotukuty-22/ref=nosim/'><img src='http://ecx.images-amazon.com/images/I/51MqUP-lVzL._SL160_.jpg' alt='Arduinoをはじめようキット' /></a></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/B0025Y6C5G/nandemotukuty-22/ref=nosim/'>Arduinoをはじめようキット</a><div>posted with <a href='http://www.amazlet.com/browse/ASIN/B0025Y6C5G/nandemotukuty-22/ref=nosim/' title='Arduinoをはじめようキット'>amazlet</a> at 10.07.24</div></div><div>スイッチサイエンス <br />売り上げランキング: 945<br /></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/B0025Y6C5G/nandemotukuty-22/ref=nosim/'>Amazon.co.jp で詳細を見る</a></div></div></div><div></div></div>|<div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/4873114535/nandemotukuty-22/ref=nosim/'><img src='http://ecx.images-amazon.com/images/I/51Tg0mv%2B5QL._SL160_.jpg' alt='Prototyping Lab ―「作りながら考える」ためのArduino実践レシピ' /></a></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/4873114535/nandemotukuty-22/ref=nosim/'>Prototyping Lab ―「作りながら考える」ためのArduino実践レシピ</a><div>posted with <a href='http://www.amazlet.com/browse/ASIN/4873114535/nandemotukuty-22/ref=nosim/' title='Prototyping Lab ―「作りながら考える」ためのArduino実践レシピ'>amazlet</a> at 10.07.24</div></div><div>小林 茂 <br />オライリージャパン <br />売り上げランキング: 4572<br /></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/4873114535/nandemotukuty-22/ref=nosim/'>Amazon.co.jp で詳細を見る</a></div></div></div><div></div></div>|<div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/4797355158/nandemotukuty-22/ref=nosim/'><img src='http://ecx.images-amazon.com/images/I/51Ynw2O9ONL._SL160_.jpg' alt='iPhone Core Audioプログラミング' /></a></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/4797355158/nandemotukuty-22/ref=nosim/'>iPhone Core Audioプログラミング</a><div>posted with <a href='http://www.amazlet.com/browse/ASIN/4797355158/nandemotukuty-22/ref=nosim/' title='iPhone Core Audioプログラミング'>amazlet</a> at 10.07.24</div></div><div>永野 哲久 <br />ソフトバンククリエイティブ <br />売り上げランキング: 14085<br /></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/4797355158/nandemotukuty-22/ref=nosim/'>Amazon.co.jp で詳細を見る</a></div></div></div><div></div></div>|<div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/0596516649/nandemotukuty-22/ref=nosim/'><img src='http://ecx.images-amazon.com/images/I/51KPTg%2B65eL._SL160_.jpg' alt='Iphone Hacks' /></a></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/0596516649/nandemotukuty-22/ref=nosim/'>Iphone Hacks</a><div>posted with <a href='http://www.amazlet.com/browse/ASIN/0596516649/nandemotukuty-22/ref=nosim/' title='Iphone Hacks'>amazlet</a> at 10.07.24</div></div><div>David Jurick Adam Stolarz Damien Stolarz <br />Make Books <br />売り上げランキング: 10806<br /></div><div><div><a href='http://www.amazon.co.jp/exec/obidos/ASIN/0596516649/nandemotukuty-22/ref=nosim/'>Amazon.co.jp で詳細を見る</a></div></div></div><div></div></div>|
|:---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|:-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|



---

Copyright © [なんでも作っちゃう、かも。](http://arms22.blog91.fc2.com/) All Rights Reserved.



<a href='Hidden comment: 
This text will be removed from the rendered page.
'></a>