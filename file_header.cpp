#include "file_header.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include "utils.h"

namespace fs = boost::filesystem;

namespace newtar
{

static const char CHECKSUM_DEFAULT_VALUE = 0x20;

file_header::file_header(fs::path file_path)
{
  std::fill_n(reinterpret_cast<char*>(this), sizeof(file_header), 0);
  std::fill_n(checksum, sizeof(checksum), CHECKSUM_DEFAULT_VALUE);

  struct stat fileInfo = {};
  stat(file_path.c_str(), &fileInfo);
  
  make_oct_string(fileInfo.st_mode & ACCESSPERMS, sizeof(file_mode), file_mode);
  make_oct_string(fileInfo.st_uid, sizeof(file_uid), file_uid);
  make_oct_string(fileInfo.st_gid, sizeof(file_gid), file_gid);
  make_oct_string(fileInfo.st_mtime, sizeof(file_mtime), file_mtime);

  std::string path = file_path.string();
  size_t size = fileInfo.st_size;
  if (fs::is_directory(file_path))
  {
    path += '/';
    size = 0;
    link_indicator = DIRECTORY;
  }
  else if (fs::is_symlink(file_path))
  {
    fs::path s = fs::read_symlink(file_path);
    strncpy(name_linked_file, s.c_str(), sizeof(name_linked_file));
    link_indicator = SYM_LINK;
  }
  else
  {
    link_indicator = NORMAL;
  }
  
  strncpy(file_name, path.c_str(), sizeof(file_name));
  make_oct_string(size, sizeof(file_size), file_size);

  const int chksum = sum(reinterpret_cast<char*>(this), sizeof(file_header));
  make_oct_string(chksum, sizeof(checksum) - 1, checksum);
}

file_header::file_header(std::ifstream& in)
{
  in.read(reinterpret_cast<char*>(this), sizeof(file_header));
}

bool file_header::checksum_is_good() const
{
  unsigned long chksum = sizeof(checksum) * CHECKSUM_DEFAULT_VALUE;
  chksum += sum(file_name, sizeof(file_name) + sizeof(file_mode) + sizeof(file_uid) + sizeof(file_gid) + sizeof(file_size) + sizeof(file_mtime));
  chksum += link_indicator + sum(name_linked_file, sizeof(name_linked_file) + sizeof(rest));

  return chksum == bytes_to_long(checksum);
}

}