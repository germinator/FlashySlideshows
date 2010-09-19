#ifndef DIRECTORY_LISTING_H_INCLUDED
#define DIRECTORY_LISTING_H_INCLUDED

unsigned int GetDirectoryList(char * thedirectory,unsigned int store_results_in_space);
unsigned int GetTotalFilesInDirectory();
unsigned int GetTotalViewableFilesInDirectory();
unsigned int GetViewableFilenameforFile(unsigned int file_id,char *directory,char *filename);
unsigned int CountPicturesInDirectory(char * thedirectory);

#endif // DIRECTORY_LISTING_H_INCLUDED
