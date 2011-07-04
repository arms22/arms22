#include <LiquidCrystal.h>
#include <BigFont.h>

// キャラクタLCDクラス（RS=>7, E=>6, D4=>5, D5=>4, D4=>3, D3=>2)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// ビッグフォントクラス
BigFont bigf;

void setup() {
  // キャラクタLCDを初期化
  lcd.begin(16, 2);
  // 通常フォントで表示
  lcd.print("Hello, World!");
  // キャラクタLCDをビッグフォントクラスにアタッチ
  bigf.attach(&lcd);
  // 3秒待つ
  delay(3000);
}

void loop() {
  // カーソルをホームポジションに移動
  bigf.home();
  // ビッグフォントでランダム値を表示
  bigf.print(random() % 10000);
  // 200ミリ秒待つ
  delay(200);
}

