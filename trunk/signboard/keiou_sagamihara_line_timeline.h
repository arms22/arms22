#ifndef KEIOU_SAGAMIHARA_LINE_H
#define KEIOU_SAGAMIHARA_LINE_H

#define  HSM    0
#define  E_HSM  1
#define  R_HSM  2

#define  SJK    3
#define  TFU    4
#define  OSM    5
#define  SKR    6
#define  WKB    7
#define  YMT    8
#define  IWM    9
#define  E_SJK  10
#define  R_SJK  11
#define  E_OSM  12
#define  R_OSM  13
#define  E_YWT  14

const prog_uint8_t PROGMEM keiou_sagamihara_tama_center_for_hashimoto_holiday[] = {
   5,   HSM, 18,   HSM, 32,   HSM, 50, Timeline::NXT,
   6,   HSM,  1,   HSM, 11,   HSM, 25,   HSM, 40,   HSM, 56, Timeline::NXT,
   7,   HSM,  2,   HSM, 11,   HSM, 27,   HSM, 35,   HSM, 50,   HSM, 58, Timeline::NXT,
   8,   HSM,  3,   HSM, 12, E_HSM, 24,   HSM, 29,   HSM, 38, E_HSM, 42,   HSM, 49,   HSM, 53, Timeline::NXT,
   9, E_HSM,  2,   HSM, 13, E_HSM, 17, E_HSM, 23, R_HSM, 29,   HSM, 34, E_HSM, 42, R_HSM, 49,   HSM, 53, Timeline::NXT,
  10, E_HSM,  1, R_HSM,  9,   HSM, 12, E_HSM, 22, R_HSM, 29,   HSM, 35, E_HSM, 42, R_HSM, 49,   HSM, 55, Timeline::NXT,
  11, E_HSM,  2, R_HSM,  9,   HSM, 15, E_HSM, 22, R_HSM, 29,   HSM, 35, E_HSM, 42, R_HSM, 49,   HSM, 55, Timeline::NXT,
  12, E_HSM,  2, R_HSM,  9,   HSM, 15, E_HSM, 22, R_HSM, 29,   HSM, 35, E_HSM, 42, R_HSM, 49,   HSM, 55, Timeline::NXT,
  13, E_HSM,  2, R_HSM,  9,   HSM, 15, E_HSM, 22, R_HSM, 29,   HSM, 35, E_HSM, 42, R_HSM, 49,   HSM, 55, Timeline::NXT,
  14, E_HSM,  2, R_HSM,  9,   HSM, 15, E_HSM, 22, R_HSM, 29,   HSM, 35, E_HSM, 42, R_HSM, 49,   HSM, 55, Timeline::NXT,
  15, E_HSM,  2, R_HSM,  9,   HSM, 15, E_HSM, 22, R_HSM, 29,   HSM, 35, E_HSM, 42, R_HSM, 49,   HSM, 54, Timeline::NXT,
  16, E_HSM,  2, R_HSM,  9,   HSM, 14, E_HSM, 22, R_HSM, 29,   HSM, 34, E_HSM, 42, R_HSM, 49,   HSM, 54, Timeline::NXT,
  17, E_HSM,  2, R_HSM,  9,   HSM, 14, E_HSM, 23, R_HSM, 30,   HSM, 34, E_HSM, 43, R_HSM, 50,   HSM, 56, Timeline::NXT,
  18, E_HSM,  6, R_HSM, 11,   HSM, 17, E_HSM, 23, R_HSM, 31,   HSM, 37, E_HSM, 43, R_HSM, 51,   HSM, 57, Timeline::NXT,
  19, E_HSM,  3, R_HSM, 11,   HSM, 17, E_HSM, 23, E_HSM, 33, R_HSM, 41,   HSM, 46, E_HSM, 53, Timeline::NXT,
  20, R_HSM,  1,   HSM,  6, E_HSM, 13, R_HSM, 21,   HSM, 27, E_HSM, 33, E_HSM, 43, E_HSM, 53, Timeline::NXT,
  21, E_HSM,  3, E_HSM, 13, E_HSM, 23, E_HSM, 33, E_HSM, 43, E_HSM, 52, Timeline::NXT,
  22, E_HSM,  2, E_HSM, 12, E_HSM, 22, E_HSM, 32, E_HSM, 41,   HSM, 47,   HSM, 56, Timeline::NXT,
  23,   HSM,  6,   HSM, 16,   HSM, 26,   HSM, 36,   HSM, 51, Timeline::NXT,
  24,   HSM,  7,   HSM, 21, Timeline::END
};

const prog_uint8_t PROGMEM keiou_sagamihara_tama_center_for_tyoufu_holiday[] = {
   5,   SJK,  4,   SJK, 24,   TFU, 42,   TFU, 56, Timeline::NXT,
   6,   TFU, 16,   TFU, 26,   TFU, 41, R_OSM, 54, Timeline::NXT,
   7,   TFU,  4, R_OSM, 13,   TFU, 20,   SKR, 26, R_OSM, 35,   SJK, 46,   WKB, 52, R_OSM, 56, Timeline::NXT,
   8, R_SJK,  7,   TFU, 15, E_YWT, 24, R_SJK, 30,   TFU, 35, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
   9, E_YWT,  4,   WKB,  8,   TFU, 14, E_YWT, 24, R_SJK, 30,   TFU, 37, E_YWT, 44,   WKB, 48,   TFU, 56, Timeline::NXT,
  10, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
  11, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
  12, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
  13, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
  14, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
  15, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 44, R_SJK, 50,   TFU, 56, Timeline::NXT,
  16, E_YWT,  4, R_SJK, 10,   TFU, 16, E_YWT, 24, R_SJK, 30,   TFU, 36, E_YWT, 42, R_SJK, 48,   TFU, 56, Timeline::NXT,
  17, E_OSM,  3, R_SJK,  9,   TFU, 16, E_OSM, 23, R_SJK, 29,   TFU, 36, E_OSM, 43, R_SJK, 49,   TFU, 56, Timeline::NXT,
  18, E_OSM,  3, R_SJK,  9, R_OSM, 19,   TFU, 28, E_SJK, 33, R_OSM, 39,   TFU, 45, E_SJK, 53, R_OSM, 59, Timeline::NXT,
  19,   TFU,  6, E_SJK, 13, R_OSM, 20,   TFU, 26, E_SJK, 33, R_OSM, 40,   TFU, 46, E_SJK, 53, Timeline::NXT,
  20, R_OSM,  0, R_SJK, 10, R_OSM, 18, R_SJK, 28,   WKB, 36,   WKB, 42, R_SJK, 50, Timeline::NXT,
  21, R_OSM,  0, R_SJK, 10,   WKB, 18,   TFU, 28,   TFU, 37,   TFU, 47,   TFU, 57, Timeline::NXT,
  22,   TFU,  7,   TFU, 16,   TFU, 24,   TFU, 36,   TFU, 46,   TFU, 56, Timeline::NXT,
  23,   TFU,  6,   TFU, 20,   IWM, 30,   TFU, 44, Timeline::NXT,
  24,   WKB,  0,   WKB, 14,   WKB, 30,   WKB, 46, Timeline::END
};

#endif
