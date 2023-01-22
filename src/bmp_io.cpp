#include "bmp_io.h"

#include <inttypes.h>
#include <array>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>

namespace bmpio {
namespace detail {

    class ErrorReadingFile : public std::runtime_error {
        public:
            ErrorReadingFile() : std::runtime_error("Error reading input file") {}
    };

    class ErrorWritingFile : public std::runtime_error {
        public:
            ErrorWritingFile() : std::runtime_error("Error writing input file") {}
    };

    class ErrorCorruptInputFile : public std::runtime_error {
        public:
            ErrorCorruptInputFile() : std::runtime_error("Input file is corrupt or incomplete") {}
    };

    class ErrorUnsupportedFormat : public std::runtime_error {
        public:
            ErrorUnsupportedFormat() : std::runtime_error("Input file has unsupported format") {}
    };

    struct BmpFileHeader {
        static constexpr uint32_t kFileHeaderSize = 14;
        static constexpr std::array<char, 2> kIdentifier = {'B', 'M'};

        uint32_t file_size = 0;
        uint32_t data_offset = 0;
    };

    constexpr std::array<char, 2> BmpFileHeader::kIdentifier;

    struct BmpInfoHeader {
        static constexpr uint32_t kInfoHeaderSize = 40;
        static constexpr uint16_t kNumColorPlanes = 1;
        static constexpr uint16_t kBytesPerPixel = 3;
        static constexpr uint16_t kBitsPerPixel = kBytesPerPixel * 8;
        static constexpr uint32_t kCompression = 0;
        static constexpr uint32_t kColorPaletteSize = 0;
        static constexpr uint32_t kNumImportantColors = 0;

        uint32_t image_size = 0;

        int32_t width = 0;
        int32_t height = 0;

        int32_t res_horz = 3780;
        int32_t res_vert = 3780;
    };

    template <typename T>
        T read(std::ifstream& file) {
        T val;
        file.read(reinterpret_cast<char*>(&val), sizeof(T));
        return val;
    }

    template <typename T>
        void write(std::ofstream& file, T val) {
        file.write(reinterpret_cast<char*>(&val), sizeof(T));
    }

    BmpFileHeader readBmpFileHeader(std::ifstream& file) {
        BmpFileHeader header;
        std::array<char, 2> ident;

        file.read(ident.data(), 2);

        if (!file.good())
            throw ErrorCorruptInputFile();

        if ((BmpFileHeader::kIdentifier[0] != ident[0]) ||
            (BmpFileHeader::kIdentifier[1] != ident[1]))
            throw ErrorUnsupportedFormat();

        header.file_size = read<uint32_t>(file);

        // Skip 4 reserved bytes
        file.seekg(4, std::ios_base::cur);

        header.data_offset = read<uint32_t>(file);

        if (!file.good())
            throw ErrorCorruptInputFile();

        return header;
    }

    void writeFileHeader(std::ofstream& file, const BmpFileHeader& header) {
        write(file, header.kIdentifier[0]);
        write(file, header.kIdentifier[1]);
        write(file, header.file_size);
        write(file, uint32_t(0));  // Reserved bytes
        write(file, header.data_offset);
    };

    BmpInfoHeader readBmpInfoHeader(std::ifstream& file) {
        BmpInfoHeader header;

        uint32_t size = read<uint32_t>(file);

        if (size != BmpInfoHeader::kInfoHeaderSize)
            throw ErrorUnsupportedFormat();

        header.width = read<int32_t>(file);
        header.height = read<int32_t>(file);

        uint16_t color_planes = read<uint16_t>(file);

        if (color_planes != BmpInfoHeader::kNumColorPlanes)
            throw ErrorUnsupportedFormat();

        uint16_t bpp = read<uint16_t>(file);

        if (bpp != BmpInfoHeader::kBitsPerPixel)
            throw ErrorUnsupportedFormat();

        uint32_t compr = read<uint32_t>(file);

        if (compr != BmpInfoHeader::kCompression)
            throw ErrorUnsupportedFormat();

        header.image_size = read<uint32_t>(file);
        header.res_horz = read<int32_t>(file);
        header.res_vert = read<int32_t>(file);

        uint32_t col_palette_size = read<uint32_t>(file);

        if (col_palette_size != BmpInfoHeader::kColorPaletteSize)
            throw ErrorUnsupportedFormat();

        if (!file.good())
            throw ErrorCorruptInputFile();

        return header;
    }

    void writeInfoHeader(std::ofstream& file, const BmpInfoHeader& header) {
        write(file, header.kInfoHeaderSize);
        write(file, header.width);
        write(file, header.height);
        write(file, header.kNumColorPlanes);
        write(file, header.kBitsPerPixel);
        write(file, header.kCompression);
        write(file, header.image_size);
        write(file, header.res_horz);
        write(file, header.res_vert);
        write(file, header.kColorPaletteSize);
        write(file, header.kNumImportantColors);
    };

} // namespace detail


}