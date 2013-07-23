#ifndef FILE_HEADER_H
#define FILE_HEADER_H

#include <fstream>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

namespace newtar
{

enum file_type : char
{
  NORMAL = '0',
  HARD_LINK = '1',
  SYM_LINK = '2',
  DIRECTORY = '5'
};

#pragma pack(push, 1) 
struct file_header
{
  char file_name[100];
  char file_mode[8];
  char file_uid[8];
  char file_gid[8];
  char file_size[12];
  char file_mtime[12];
  char checksum[8];
  file_type link_indicator;
  char name_linked_file[100];
  char rest[255];//unused in PrePOSIX
  
  file_header(boost::filesystem::path file_path);
  file_header(std::ifstream& in);
  bool checksum_is_good() const;
  
};
#pragma pack(pop)

const size_t FILE_HEADER_SIZE = 512;
static_assert(sizeof(file_header) == FILE_HEADER_SIZE, "wrong file_header size");

}

#endif//FILE_HEADER_H
