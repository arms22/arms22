#ifndef JR_SAIKYO_LINE_H
#define JR_SAIKYO_LINE_H

#define KWG (0)
#define MUW (1)
#define OMY (2)
#define AKB (3)
#define IKB (4)
#define SKB (5)
#define OSK (6)

const prog_uint8_t PROGMEM jr_saikyo_line_shibuya_north_business_day[] = {
  7, KWG, 13, MUW, 33, KWG, 45, OMY, 56, Timeline::NXT,
  8, AKB,  5, MUW, 13, KWG, 16, OMY, 25, OMY, 28, OMY, 35, KWG, 40, OMY, 46, AKB, 54, Timeline::NXT,
  9, KWG,  5, OMY, 10, IKB, 15, OMY, 26, OMY, 32, KWG, 37, IKB, 42, OMY, 52, AKB, 56, Timeline::NXT,
  10, IKB,  3, KWG, 18, OMY, 27, OMY, 32, KWG, 59, Timeline::NXT,
  11, KWG, 17, KWG, 39, KWG, 59, Timeline::NXT,
  12, KWG, 19, KWG, 39, KWG, 57, Timeline::NXT,
  13, KWG, 19, KWG, 39, KWG, 59, Timeline::NXT,
  14, KWG, 19, KWG, 39, KWG, 57, Timeline::NXT,
  15, KWG, 17, OMY, 28, KWG, 56, Timeline::NXT,
  16, KWG, 15, KWG, 37, KWG, 55, Timeline::NXT,
  17, OMY,  1, KWG,  7, KWG, 14, OMY, 27, AKB, 46, KWG, 55, Timeline::NXT,
  18, AKB,  7, KWG, 14, OMY, 21, KWG, 33, OMY, 45, KWG, 56, Timeline::NXT,
  19, OMY,  5, KWG, 15, OMY, 23, KWG, 34, OMY, 45, OMY, 59, Timeline::NXT,
  20, AKB,  6, OMY, 20, KWG, 28, OMY, 44, OMY, 57, Timeline::NXT,
  21, KWG,  7, OMY, 21, KWG, 28, KWG, 34, KWG, 44, KWG, 55, Timeline::NXT,
  22, OMY,  1, KWG, 16, KWG, 29, OMY, 43, KWG, 50, Timeline::NXT,
  23, KWG,  0, AKB, 16, OMY, 33, KWG, 41, Timeline::END
};

const prog_uint8_t PROGMEM jr_saikyo_line_shibuya_south_business_day[] = {
  7, SKB, 15, SKB, 27, SKB, 38, SKB, 49, SKB, 55, Timeline::NXT,
  8, SKB,  2, SKB,  5, SKB, 13, SKB, 15, SKB, 22, SKB, 25, SKB, 31, SKB, 33, SKB, 36, SKB, 44, SKB, 49, SKB, 58, Timeline::NXT,
  9, SKB,  3, SKB, 14, SKB, 18, SKB, 27, SKB, 33, SKB, 47, SKB, 59, Timeline::NXT,
  10, SKB,  8, SKB, 31, SKB, 55, Timeline::NXT,
  11, SKB, 11, SKB, 31, SKB, 53, Timeline::NXT,
  12, SKB, 12, SKB, 30, SKB, 55, Timeline::NXT,
  13, SKB, 12, SKB, 30, SKB, 53, Timeline::NXT,
  14, SKB, 11, SKB, 30, SKB, 53, Timeline::NXT,
  15, SKB, 11, SKB, 31, SKB, 55, Timeline::NXT,
  16, SKB, 10, SKB, 30, SKB, 43, SKB, 53, Timeline::NXT,
  17, SKB,  8, SKB, 11, SKB, 17, SKB, 30, SKB, 34, SKB, 54, Timeline::NXT,
  18, SKB,  1, SKB, 11, SKB, 19, SKB, 30, SKB, 43, SKB, 49, Timeline::NXT,
  19, SKB,  0, SKB,  8, SKB, 18, SKB, 32, SKB, 40, SKB, 53, Timeline::NXT,
  20, SKB,  6, SKB, 15, SKB, 27, SKB, 45, SKB, 55, Timeline::NXT,
  21, SKB,  2, SKB, 16, SKB, 28, SKB, 38, SKB, 50, Timeline::NXT,
  22, SKB,  1, SKB, 10, SKB, 21, SKB, 33, SKB, 45, SKB, 56, Timeline::NXT,
  23, SKB,  8, SKB, 22, OSK, 36, Timeline::END
};

#endif
