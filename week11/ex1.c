#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(){
  // opens the root directory ("/") and returns a pointer to the directory stream. This pointer is stored in dp.
  DIR *dp = opendir("/");
  
  struct dirent *de;
  //reads the next directory entry from the directory stream pointed to by dp
  //it returns a pointer to a dirent structure or NULL if no more entries.
  while ((de = readdir(dp)) != NULL)
    puts(de->d_name);

  return 0;
}
