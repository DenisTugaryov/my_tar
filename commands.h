#ifndef COMMANDS_H
#define COMMANDS_H

#include <iostream>
#include <boost/filesystem/path.hpp>

namespace newtar
{

void create(const std::vector<fs::path>& file_paths, std::ostream& out);
void extract(const boost::filesystem::path input_file_path);
void list(const boost::filesystem::path file_path);

}

#endif//COMMANDS_H
