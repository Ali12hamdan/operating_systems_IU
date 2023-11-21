#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(){
  DIR *dp = opendir("/");
  
  struct dirent *de;
  while ((de = readdir(dp)) != NULL)
    puts(de->d_name);

  return 0;
}
