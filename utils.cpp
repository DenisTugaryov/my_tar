#include "utils.h"

#include <iostream>

namespace newtar
{

bool stream_copy_n(std::istream& in, size_t n, std::ostream& out)
{
  char buffer[BLOCK_SIZE] = {0};

  while(!in.eof() && n > 0)
  {
    const size_t request_size = BLOCK_SIZE < n ? BLOCK_SIZE : n;    
    in.read(buffer, request_size);

    const size_t read_size = in.gcount();
    n -= read_size;

    if (read_size != request_size)
      return false;

    out.write(buffer, read_size);
  }

  return n == 0 && in.good();
}

}
