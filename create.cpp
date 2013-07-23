#include <boost/filesystem/operations.hpp>
#include <iterator>
#include <iomanip>
#include <vector>

#include "utils.h"
#include "file_header.h"


namespace fs = boost::filesystem;

namespace newtar
{

void create(const fs::path file_path, std::ostream& out)
{
  if (!fs::exists(file_path))
  {
    std::cout<< "file or dir " << file_path.string() << " not found" << std::endl;
    return;
  }

  file_header header(file_path);
  out.write((char*)&header, sizeof(file_header));

  if (fs::is_symlink(file_path))
    return;

  if (fs::is_directory(file_path))
  {
    fs::directory_iterator end_itr;
    for (fs::directory_iterator itr(file_path); itr != end_itr; ++itr)
        create(*itr, out);

    return;
  } 

  std::ifstream file(file_path.c_str(), std::ios::binary);
  size_t size = fs::file_size(file_path);
  stream_copy_n(file, size, out);
  pad_block(size, out);
}

void create(const std::vector<fs::path>& file_paths, std::ostream& out)
{
  for (auto it = file_paths.begin(); it != file_paths.end(); ++it)
    create(*it, out);
}

}