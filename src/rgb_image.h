#pragma once

#include <inttypes.h>
#include <vector>

class RgbImage {
 public:
  using RgbData = std::vector<uint8_t>;

  RgbImage() : width_(0), height_(0), data_(){};

  RgbImage(uint32_t width, uint32_t height)
      : width_(width), height_(height), data_(3 * width * height){};

  RgbImage(uint32_t width, uint32_t height, RgbData& data)
      : width_(width), height_(height), data_(data){};

  uint32_t getWidth() const { return width_; }
  uint32_t getHeight() const { return height_; }

  RgbData& getData() { return data_; }
  const RgbData& getData() const { return data_; }

 private:
  uint32_t width_;
  uint32_t height_;
  RgbData data_;
};