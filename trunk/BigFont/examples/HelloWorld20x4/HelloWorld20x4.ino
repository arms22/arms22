#include <LiquidCrystal.h>
#include <BigFont.h>

// キャラクタLCDクラス（RS=>7, E=>6, D4=>5, D5=>4, D4=>3, D3=>2)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// ビッグフォントクラス
BigFont bigf;

void setup() {
  // キャラクタLCDを初期化（20文字×4行）
  lcd.begin(20, 4);
  // キャラクタLCDをビッグフォントクラスにアタッチ
  bigf.attach(&lcd);
}

void loop() {
  // 表示をクリア
  lcd.clear();
  // 通常フォントで表示
  lcd.setCursor(7, 0);
  lcd.print("HELLO");
  lcd.setCursor(7, 2);
  lcd.print("WORLD");
  // １秒待つ
  delay(1000);  
  // 表示をクリア
  bigf.clear();
  // ビッグフォントで表示
  bigf.setCursor(0, 0);
  bigf.print("HELLO");
  bigf.setCursor(0, 2);
  bigf.print("WORLD");
  // １秒待つ
  delay(1000);
}

