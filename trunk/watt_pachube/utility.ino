// Since "+" operator doesn't support float values,
// convert a float value to a fixed point value
void appendFloatValueAsString(String& outString,float value) {
  int integerPortion = (int)value;
  int fractionalPortion = (value - integerPortion + 0.0005) * 1000;

  outString += integerPortion;
  outString += ".";

  if (fractionalPortion < 10) {
    // e.g. 9 > "00" + "9" = "009"
    outString += "00";
  } 
  else if (fractionalPortion < 100) {
    // e.g. 99 > "0" + "99" = "099"
    outString += "0";
  }

  outString += fractionalPortion;
}

int elapsedSecond(int last)
{
  int cur = second();
  return (cur >= last) ? (cur - last) : (60 - (last - cur));
}

int elapsedMinute(int last)
{
  int cur = minute();
  return (cur >= last) ? (cur - last) : (60 - (last - cur));}

