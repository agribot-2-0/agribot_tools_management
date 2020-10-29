#pragma once

#include <unordered_map>

namespace tool_manager
{
  enum class ErrorCode
  {
    Success = 0,
    NoTagsFound,
    MoreThanOneTagFound,
    RfidServiceError
  };
  enum class Antenna
  {
    Front = 1,
    Back = 2
  };
  class HashAnt
  {
  public:
    std::size_t operator()(const Antenna& a) const
    {
      return static_cast<std::size_t>(a);
    }
  };
  class Scanner
  {
  private:
    const std::size_t max_number_of_iterations = 128;
    const struct
    {
      double AntPwrResolution;
      double MinAntPower;
      double MaxAntPower;
    } antenna_power_info{ 0.25, 17.0, 33.0 };

    Antenna ant_;
    std::size_t pwr_change_attempts_;
    std::unordered_map<Antenna, double, HashAnt> pwr_;

  public:
    Scanner();
    void increaseAntennaPower();
    void setAnt(uint8_t sel_ant);
    uint8_t getAnt();
    void decreaseAntennaPower();
    bool entirePowerRangeInspected();
    double adjustPower(std::size_t numberOfTags);
  };
}  // namespace tool_manager