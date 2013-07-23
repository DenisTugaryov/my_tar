#ifndef UTILS_H
#define UTILS_H

#include <sstream>
#include <iostream>
#include <numeric>
#include <string.h>
#include "file_header.h"

namespace fs = boost::filesystem;

namespace newtar
{

const size_t BLOCK_SIZE = 512; 

inline int sum(const char* str, const size_t size)
{
  unsigned char* u_str = (unsigned char*)str;
  return std::accumulate(u_str, u_str + size, 0);
}

inline unsigned long bytes_to_long(const char* str)
{
  unsigned long long_value = 0;
  std::istringstream(str) >> std::oct >> long_value;
  return long_value;
}

inline void make_oct_string(const int number, const size_t size_string, char* str)
{
  std::ostringstream ss;
  ss.width(size_string - 1);
  ss.fill('0');
  ss << std::oct << number;
  strncpy(str, ss.str().c_str(), size_string);
}

inline void pad_block(const size_t size, std::ostream& out)
{
  const size_t PADDING_SIZE = (- size) % BLOCK_SIZE;
  const char BUFFER[BLOCK_SIZE] = {0};
  out.write(BUFFER, PADDING_SIZE);
}

template <typename F>
void iterator(const fs::path input_file_path, F f)
{
  if (!fs::exists(input_file_path))
  {
    std::cout<< "FILE " << input_file_path.string() << " doesn't exist" << std::endl;
    return;
  }

  std::ifstream input_file(input_file_path.string());

  while(input_file.good())
  {
    file_header header(input_file);
    
    if (bytes_to_long(header.checksum) == 0 || input_file.eof())
      break;

    if (!header.checksum_is_good())
    {
      std::cout << "Bad header." << std::endl;
      break;
    }

    f(input_file, header);
  }
}

bool stream_copy_n(std::istream& in, size_t n, std::ostream& out);

}

#endif//UTILS_H
