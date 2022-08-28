#include "SD_Card.h"

bool SD_Card::init_sd()
{
  if (!SD.begin(4))
  {
    return false;
  }
  SD_Card::file = SD.open("readings.txt", FILE_WRITE);
  SD_Card::file.close();
  return true;
}
bool SD_Card::write_to_file(String str)
{
  if (SD.exists("readings.txt"))
  {
    if (SD_Card::file)
    {
      SD_Card::file == SD.open("readings.txt", FILE_WRITE);
      SD_Card::file.println(str);
      SD_Card::file.close();
      return true;
    }
  }
  return false;

}
