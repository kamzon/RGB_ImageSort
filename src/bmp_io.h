#pragma once
#include "rgb_image.h"

#include <string>

namespace bmpio {

RgbImage loadRgbImage(const std::string& file_name);
void saveRgbImage(const std::string& file_name, const RgbImage& image);
RgbImage sortRgbImage(RgbImage& image);

}  // namespace bmpio