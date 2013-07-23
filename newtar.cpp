#include <fstream>
#include <iostream>

#include <boost/filesystem/operations.hpp>

#include "utils.h"
#include "commands.h"


namespace fs = boost::filesystem;


using namespace newtar;


void show_help()
{
  std::cout<< "**********HELP**********" << std::endl;
  std::cout<< "-c  create a new archive" << std::endl;
  std::cout<< "-t  list the contents of an archive" << std::endl;
  std::cout<< "-x  extract files from an archive" << std::endl;
  std::cout<< "-f  use archive file or device ARCHIVE" << std::endl;
  std::cout<< "--help  give this help list" << std::endl;
}

void run_create(const int argc, const char *argv[])
{
  const int NEXT_TO_LAST = argc - 2;
  const int MINIMAL_ARGC_FOR_USE_FILE = 5;
  const int MINIMAL_ARGC_FOR_USE_CREATE = 3;
  const char** FIRST_FILE_FOR_ARCHIVE = argv + 2;

  if (argc >= MINIMAL_ARGC_FOR_USE_FILE && std::strcmp(argv[NEXT_TO_LAST], "-f") == 0)
  {
    std::vector<fs::path> file_paths(FIRST_FILE_FOR_ARCHIVE, argv + NEXT_TO_LAST);
    std::ofstream output_file (argv[argc - 1]);
    create(file_paths, output_file);
  }
  else if (argc >= MINIMAL_ARGC_FOR_USE_CREATE)
  {
    std::vector<fs::path> file_paths(FIRST_FILE_FOR_ARCHIVE, argv + argc);
    create(file_paths, std::cout);
  }
  else
  {
    show_help();
  }
}

void run_extract(const int argc, const char *argv[])
{
  if (argc == 4 && std::strcmp(argv[2], "-f") == 0)
    extract(argv[3]);
  else
    show_help();
}

void run_list(const int argc, const char *argv[])
{
  if (argc == 4 && std::strcmp(argv[2], "-f") == 0)
    list(argv[3]);
  else
    show_help();
}

int main(const int argc, char const *argv[])
{
  if (argc <= 1)
    show_help();
  else if (std::strcmp(argv[1], "-c") == 0)
    run_create(argc, argv);
  else if (std::strcmp(argv[1], "-x") == 0)
    run_extract(argc, argv);
  else if (std::strcmp(argv[1], "-t") == 0)
    run_list(argc, argv);
  else
    show_help();
  
	return 0;
}
