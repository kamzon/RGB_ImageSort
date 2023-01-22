#include "bmp_io.h"
#include "rgb_image.h"

#include <iostream>


int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "usage: imgsort <input file> [<output file>]" << std::endl;
    std::cout << "<input file> has to be a file path to an uncompressed 24 "
                 "bit BMP image file"
              << std::endl;
    return 0;
  };

  RgbImage input_image;

  try {
    input_image = bmpio::loadRgbImage(argv[1]);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  RgbImage sorted_image = bmpio::sortRgbImage(input_image);

  std::string output_filename;

  if (argc >= 3)
    output_filename = argv[2];
  else
    output_filename = "output.bmp";

  try {
    bmpio::saveRgbImage(output_filename, sorted_image);
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
};