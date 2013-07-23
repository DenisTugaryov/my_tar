#include <boost/filesystem/path.hpp>

#include "utils.h"
#include "file_header.h"

namespace fs = boost::filesystem;

namespace newtar
{

void list_processor(std::istream& input, const file_header& header);

void list(const fs::path input_file_path) 
{
  iterator(input_file_path, list_processor);
}

void list_processor(std::istream& input, const file_header& header)
{
  std::cout<< header.file_name << std::endl;

  int size = bytes_to_long(header.file_size);
  size_t new_shift = size + (- size) % BLOCK_SIZE;
  input.seekg(new_shift, std::ios::cur);
  
}

}