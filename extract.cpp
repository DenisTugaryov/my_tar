#include <utime.h>
#include <sys/stat.h>

#include <boost/filesystem/path.hpp>

#include "file_header.h"
#include "utils.h"

namespace fs = boost::filesystem;

namespace newtar
{

void extract_processor(std::istream& input, const file_header& header);

void extract(const fs::path input_file_path) 
{
  iterator(input_file_path, extract_processor);
}

void extract_processor(std::istream& input, const file_header& header)
{
    unsigned long mode = bytes_to_long(header.file_mode);
    unsigned long uid = bytes_to_long(header.file_uid);
    unsigned long gid = bytes_to_long(header.file_gid);
    unsigned long mtime = bytes_to_long(header.file_mtime);

    switch (header.link_indicator) 
    {
      case NORMAL : 
      {
        std::ofstream output_file(header.file_name, std::ios::binary);
        const unsigned long size = bytes_to_long(header.file_size);

        stream_copy_n(input, size, output_file);

        const size_t SKIP_SIZE = (- size) % BLOCK_SIZE;
        input.seekg(SKIP_SIZE, std::ios::cur);
        
        break;
      }
      case DIRECTORY : 
      {
        fs::create_directories(header.file_name);
        break;
      }
      case SYM_LINK : 
      {
        fs::create_symlink(header.name_linked_file, header.file_name);
        break;
      }
      case HARD_LINK : 
      {
        fs::create_hard_link(header.name_linked_file, header.file_name);
        break;
      }
    }

    chmod(header.file_name, mode);
    chown(header.file_name, uid, gid);

    struct stat buf = {};
    if (stat(header.file_name, &buf) == 0)
    {
      utimbuf ubuf = {};
      ubuf.actime = mtime;
      ubuf.modtime = mtime;
      utime(header.file_name, &ubuf);
    }
}

}