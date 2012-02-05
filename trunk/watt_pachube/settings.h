// The environment ID for your datastreams
#define ENVIRONMENT_ID "99999"

// Your API key (a public secure key is recommended)
#define API_KEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// REPLACE WITH A PROPER MAC ADDRESS
byte macAddress[] = { 
  0x00, 0x50, 0xC2, 0x97, 0x22, 0xC3 };

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

