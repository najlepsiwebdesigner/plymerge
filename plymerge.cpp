#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "strnatcmp.hpp"

using namespace boost::filesystem;

void help() {
    std::cout << "Program usage: plymerge ./directory-name ./output-filename.ply" << std::endl;
}


int main (int argc, char** argv) {
  pcl::console::setVerbosityLevel(pcl::console::L_ALWAYS);

  if (argc != 3)
  {
    std::cerr << "Please specify both command line arguments!" << std::endl;
    help();
    exit(0);
  }

  boost::filesystem::path directory (argv[1]);
  boost::filesystem::path filename (argv[2]);

  if (!exists(directory) || !is_directory(directory)) {
    std::cerr << "Input directory does not exist or is not a directory!" << std::endl;
    exit(0);
  }

  if (exists(filename) && is_regular_file(filename)) {
    std::cerr << "Output file already exists! Do you wish to rewrite it? (yes/no)" << std::endl;

    std::string answer;
    std::cin >> answer;

    if (answer == "no"){
      exit(0);
    }
  }

  std::cout << "Wait please..." << std::endl << std::endl;

  directory_iterator end_itr;
  std::vector<std::string> filenames;
  for (directory_iterator itr(directory); itr != end_itr; ++itr)
  {
     if (is_regular_file(itr->path())) {
         std::string current_file = itr->path().string();
        filenames.push_back(current_file);
     }
   }

   std::sort(filenames.begin(), filenames.end(),compareNat);

   pcl::PointCloud<pcl::PointXYZRGB> merged_cloud;
   pcl::PointCloud<pcl::PointXYZRGB> temp_cloud;
   int i = 0;
   for(std::vector<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it) {
       if (i == 0) {
         pcl::io::loadPLYFile<pcl::PointXYZRGB>(*it, merged_cloud);
       } else {
         pcl::io::loadPLYFile<pcl::PointXYZRGB>(*it, temp_cloud);
         merged_cloud += temp_cloud;
       }

       std::cout << "\e[A";
       std::cout << "\r";
       std::cout << "Merging pointcloud " << i << " of " << filenames.size() << std::endl;

       i++;
   }

   std::cout << "Writing to PLY file" << std::endl;
   pcl::io::savePLYFileBinary(filename.string(), merged_cloud);
   std::cout << "Cloud with total " << merged_cloud.points.size() << " points successfully saved!" << std::endl;

   return (0);
}
