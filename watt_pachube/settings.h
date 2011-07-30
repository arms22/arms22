// The environment ID for your datastreams
const int environmentId = 99999;

// Your API key (a public secure key is recommended)
const char *apiKey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

// REPLACE WITH A PROPER MAC ADDRESS
byte macAddress[] = { 
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };

// Update interval in minutes
const int updateIntervalInMinutes = 1;

// Time sync interval in seconds
const int timeSyncIntervalInSeconds = 300;

// 商用電源周波数
#define PWRLINE_FREQ (60)

// 電圧センサ係数（実測）
#define kVT (86.9817579)

// 電流センサ係数（R * 係数 / 巻き数）
#define kCT (100.0 * 0.99 / 800.0)

