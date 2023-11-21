#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KB 1024

struct inode
{
  char name[16];        // file name
  int size;             // file size ( in number of blocks )
  int blockPointers[8]; // direct block p o i n t e r s
  int used;             // 0 = > inode is free ; 1 = > in use
};

union block
{
  struct
  {
    char free[128];
    struct inode inodes[16];
  } super_block;
  char data[KB];
};

union block disk[128];

void create(char name[16], int size)
{
  // search for free inode
  struct inode *newInode = NULL;
  {
    for (int i = 0; i < 16; i++)
    {
      if (disk[0].super_block.inodes[i].used == 0)
      {
        newInode = &disk[0].super_block.inodes[i];
        break;
      }
    }
  }
  newInode->used = 1;
  strncpy(newInode->name, name, 16);
  newInode->size = 0;

  for (int i = 0; i < 128 && newInode->size < size; i++)
  {
    if (disk[0].super_block.free[i] == 0)
    {
      newInode->blockPointers[newInode->size++] = i;
      disk[0].super_block.free[i] = 1;
    }
  }
}

void delete(char name[16])
{
  for (int i = 0; i < 16; i++)
  {
    if (disk[0].super_block.inodes[0].used == 1 && strncmp(name, disk[0].super_block.inodes[i].name, 16) == 0)
    {
      for (int j = 0; j < disk[0].super_block.inodes[i].size; j++)
        disk[0].super_block.free[disk[0].super_block.inodes[i].blockPointers[j]] = 0;
      disk[0].super_block.inodes[i].used = 0;
      break;
    }
  }
}

void ls()
{
  for (int i = 0; i < 16; i++)
  {
    if (disk[0].super_block.inodes[i].used == 1)
      printf("%s\n", disk[0].super_block.inodes[i].name);
  }
}

void write(char name[16], int blockNum, char buf[1024])
{
  for (int i = 0; i < 16; i++)
  {
    if (disk[0].super_block.inodes[0].used == 1 && strncmp(name, disk[0].super_block.inodes[i].name, 16) == 0)
    {
      if (disk[0].super_block.inodes[i].size < blockNum)
      {
        perror("invalid write \n");
        return;
      }
      else
        strncpy(disk[1 + disk[0].super_block.inodes[i].blockPointers[blockNum]].data, buf, KB);
    }
  }
}

void read(char name[16], int blockNum, char buf[1024])
{
  for (int i = 0; i < 16; i++)
  {
    if (disk[0].super_block.inodes[0].used == 1 && strncmp(name, disk[0].super_block.inodes[i].name, 16) == 0)
    {
      if (disk[0].super_block.inodes[i].size > blockNum)
      {
        perror("invalid read\n");
        return;
      }
      else
      {
        strncpy(buf, disk[1 + disk[0].super_block.inodes[i].blockPointers[blockNum]].data, KB);
      }
    }
  }
}

int main(int argc, char *argv[])
{
  char buf[1024];
  scanf("%s", buf); // note: the filesystem lies completely in memory, it is not synced with the disk
  {
    for (int i = 0; i < 128; i++)
      disk[0].super_block.free[i] = 0;
    for (int i = 0; i < 16; i++)
      disk[0].super_block.inodes[i].used = 0;
  }
  while (1)
  {
    char command;
    char filename[16];
    int count;

    scanf("%c", &command);
    switch (command)
    {
    case 'C':
      scanf("%s %d", filename, &count);
      create(filename, count);
      break;
    case 'D':
      scanf("%s", filename);
      delete (filename);
      break;
    case 'L':
      ls();
      break;
    case 'R':
      scanf("%s %d", filename, &count);
      read(filename, count, buf);
      break;
    case 'W':
      scanf("%s %d", filename, &count);
      write(filename, count, buf);
      break;
    default:
      break;
    }
  }
}
