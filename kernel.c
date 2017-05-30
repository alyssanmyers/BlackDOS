/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:_Alyssa Myers________________________ Date:_04/10/2017__        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.03, Spring 2017.                 */

void handleInterrupt21(int, int, int, int);

void main()
{
   char* file = "fib";
   char* buffer = "Hi there!";
   char buf[13312];
   int size;
   
   makeInterrupt21();

   interrupt(33,12,4,16,0);
   interrupt(33,0,"___.   .__                 __       .___           \r\n\0",0,0);
   interrupt(33,0,"\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0",0,0);
   interrupt(33,0," | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___/\r\n\0",0,0);
   interrupt(33,0," | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0",0,0);
   interrupt(33,0," |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0",0,0);
   interrupt(33,0,"     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0",0,0);
   interrupt(33,0," V. 1.03, C. 2017. Based on a project by M. Black. \r\n\0",0,0);
   interrupt(33,0," Author(s): Alyssa Myers & Luciano Mogorovic\r\n\r\n\0",0,0);
   interrupt(33,0,"Hello World!\r\n\0",0,0);
   interrupt(33,0,"\r\n\0",0,0);

   interrupt(33,4,"Shell\0",2,0);
   interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);

   /*interrupt(33,8,file,buffer,2);
   interrupt(33,3,file,buf,&size);
   interrupt(33,7,file,0,0);
   /*interrupt(33,0,buf,0,0);*/

   while(1);
}

/************************************/
/* Part 1: Booting and System Calls */
/************************************/

void printString(char* c)
{
   int ax, i = 0;
   char al, ah = 14;
   do
   {
      al = *(c + i);
      ax = ah * 256 + al;
      interrupt(16, ax, 0, 0, 0);
      i++;
   } while( *(c + i) != '\0' );
   return;
}

void readString(char* c)
{
   int i = 0;
   char* key;

  do 
  {
     *key = interrupt(22,0,0,0,0);
     if(*key == 8 && !(i < 1))
     {
        interrupt(16, 14*256 + *key, 0, 0, 0);
        i--;
     }
     else
     {
       *(c + i) = *key;
       interrupt(16, 14*256 + *key, 0, 0, 0);
       i++;
     }
  } while(*key != 13);

   *(c + i - 1) = '\0';

   return;
}

void clearScreen(int bx, int cx)
{
   int i = 0;

   for(i = 0; i <= 24; i++)
   {
      printString("\r\n\0");
   }

   interrupt(16,512,0,0,0);

   if(bx > 0 && bx < 9 && cx > 0 && cx < 17)
   {
      interrupt(16,1536,4096*(bx-1)+256*(cx-1),0,6223);
   }
   return;
}

int mod(int a, int b)
{
   int x = a;
   while (x >= b) x = x - b;
   return x;
}

int div(int a, int b)
{
   int q = 0;
   while (q * b <= a) q++;
   return (q - 1);
}

void writeInt(int x)
{
   char number[6], *d;
   int q = x, r;
   if (x < 1)
   {
      d = number; *d = 0x30; d++; *d = 0x0; d--;
   }
   else
   {
      d = number + 5;
      *d = 0x0; d--;
      while (q > 0)
      {
         r = mod(q,10); q = div(q,10);
         *d = r + 48; d--;
      }
      d++;
   }
   printString(d);
}

void readInt(int* number)
{
   int result = 0; 
   int i;

   char* line;
   readString(line);

   for(i = 0; *(line + i) != '\0'; i++){
     result = (result * 10) + (*(line + i) - '0');
   }

   *number = result;
}

/******************************************/
/* Part 2: Loading and Executing Programs */
/******************************************/

void readSector(char* buffer, int sector)
{
   int ah = 2, /* tells the BIOS to read a sector */
   al = 1, /* number of sectors to read */
   ch = div(sector,36), /* track number, trackNo */
   cl = mod(sector,18) + 1, /* relative sector number, relSecNo */
   dh = mod(div(sector,18),2), /* head number, headNo */
   dl = 0, /* device number for the floppy disk */

   ax = ah * 256 + al,
   cx = ch * 256 + cl,
   dx = dh * 256 + dl;
   
   interrupt(0x13, ax, buffer, cx, dx);
   return;
}

void error(int bx)
{
   char errMsg0[18], errMsg1[17], errMsg2[13], errMsg3[17];

   errMsg0[0] = 70; errMsg0[1] = 105; errMsg0[2] = 108; errMsg0[3] = 101;
   errMsg0[4] = 32; errMsg0[5] = 110; errMsg0[6] = 111; errMsg0[7] = 116;
   errMsg0[8] = 32; errMsg0[9] = 102; errMsg0[10] = 111; errMsg0[11] = 117;
   errMsg0[12] = 110; errMsg0[13] = 100; errMsg0[14] = 46; errMsg0[15] = 13;
   errMsg0[16] = 10; errMsg0[17] = 0;
   errMsg1[0] = 66; errMsg1[1] = 97; errMsg1[2] = 100; errMsg1[3] = 32;
   errMsg1[4] = 102; errMsg1[5] = 105; errMsg1[6] = 108; errMsg1[7] = 101;
   errMsg1[8] = 32; errMsg1[9] = 110; errMsg1[10] = 97; errMsg1[11] = 109;
   errMsg1[12] = 101; errMsg1[13] = 46; errMsg1[14] = 13; errMsg1[15] = 10;
   errMsg1[16] = 0;
   errMsg2[0] = 68; errMsg2[1] = 105; errMsg2[2] = 115; errMsg2[3] = 107;
   errMsg2[4] = 32; errMsg2[5] = 102; errMsg2[6] = 117; errMsg2[7] = 108;
   errMsg2[8] = 108; errMsg2[9] = 46; errMsg2[10] = 13; errMsg2[11] = 10;
   errMsg2[12] = 0;
   errMsg3[0] = 71; errMsg3[1] = 101; errMsg3[2] = 110; errMsg3[3] = 101;
   errMsg3[4] = 114; errMsg3[5] = 97; errMsg3[6] = 108; errMsg3[7] = 32;
   errMsg3[8] = 101; errMsg3[9] = 114; errMsg3[10] = 114; errMsg3[11] = 111;
   errMsg3[12] = 114; errMsg3[13] = 46; errMsg3[14] = 13; errMsg3[15] = 10;
   errMsg3[16] = 0;

   switch(bx) {
   case 0: printString(errMsg0); break;
   case 1: printString(errMsg1); break;
   case 2: printString(errMsg2); break;
   default: printString(errMsg3);
   }
}

int comp(char* c1, char* c2)
{
   while(*c1 != '\0')
   {
      if(*c1++ != *c2++) return 0;
   }

   return 1;
}

void readFile(char* fname, char* buffer, int* size)
{
   char dir[512];
   int i = 0, iBuffer, iFound;
   *size = 0;

   readSector(dir, 2);

   for(; i < 16; i++)
   {
      if(comp(fname, &dir[iBuffer]))
      { 
         iFound = iBuffer + 6;

         while(dir[iFound] != 0)
         {
            readSector(buffer, dir[iFound]);
            buffer += 512;
            *size += 512;
            iFound++;
         }
         return;
      }
      iBuffer += 32;
   }

   error(0);
   return;
}

void runProgram(char* name, int segment)
{
   char buf[13312];
   int i;
   readFile(name, buf);

   segment = segment * 4096;
   for(i = 0; i < 13312; i++)
   {
      putInMemory(segment, i, buf[i]);
   }

   launchProgram(segment);
}

void stop()
{
   launchProgram(8192);
}

/*************************************/
/* Part 3: Shell and Writing to Disk */
/*************************************/

void writeSector(char* buffer, int sector)
{
   int ah = 3, /* tells the BIOS to write a sector */
   al = 1, /* number of sectors to read */
   ch = div(sector,36), /* track number, trackNo */
   cl = mod(sector,18) + 1, /* relative sector number, relSecNo */
   dh = mod(div(sector,18),2), /* head number, headNo */
   dl = 0, /* device number for the floppy disk */

   ax = ah * 256 + al,
   cx = ch * 256 + cl,
   dx = dh * 256 + dl;
   
   interrupt(0x13, ax, buffer, cx, dx);
   return;
}

void deleteFile(char* name)
{
   char map[512], disk[512];
   char* current;
   int found = 0, i;

   /* load disk directory and map to 512 byte character arrays */
   readSector(map, 1);
   readSector(disk, 2);

   current = disk;

   /* search through the directory and try to find the file name */
   for(i = 0; i < 16; i++)
   {
      if(comp(name, current))
      {
         found = 1;

         /* set the first byte of file to zero */
         *current = 0x0;
         current += 6;

         /* step through sector numbers belonging to file and set map byte to zero */
         while(*current != 0x0)
         {
            map[*current] = 0x0;
            current++;
         }
      }

      current += 32;
   }

   if(!found)
   {
      error(0);
   }
   else printString("DELETED! :)\r\n\0");

   /* write the char arrays holding the directory and map back to sectors */
   writeSector(map, 1);
   writeSector(disk, 2);
}

void writeFile(char* name, char* buffer, int numberOfSectors)
{
   char map[512], disk[512];
   int free_directory = 0, free_sector = 0, disk_byte = 0, i = 0, y = 6;

   readSector(map,1);
   readSector(disk,2);

   while (disk_byte < 512)
   {
     // check if file already exists
     if (disk[disk_byte] != 0x0)
       if (comp(name, &disk[disk_byte]))
       {
         error(1);
         return;
       }
    
     // saves the first free sector
     if (free_directory == 0)
       if (disk[disk_byte] == 0x0)
         free_directory = disk_byte;
     
    disk_byte = disk_byte + 32;
   }

   if (free_directory == 0) return;

   // first set all 32 bits to 0
   for (i = 0; i < 32; ++i)
   {
     disk[free_directory + i] = 0x0;
   }
    
   // save the name of the file
   for (i = 0; i < y; ++i)
   {
     if (*(name + i) == 0x0) break;
     disk[free_directory + i] = *(name + i);
   }

   for (i = 0; i < numberOfSectors; ++i)
   {
     for (y = 0; y < 256; ++y)
     {
       if (map[y] == 0x0)
       {
         map[y] = 255;
         disk[free_directory + 6 + i] = y;
         writeSector(buffer,y);
         buffer = buffer + 512;
         y = 512;
         ++free_sector;
       }
     }
   }

   if (free_sector != numberOfSectors) return;

   writeSector(map, 1);
   writeSector(disk,2);

   return;
}

void handleInterrupt21(int ax, int bx, int cx,  int dx)
{
   if(ax == 0) printString(bx);
   else if(ax == 1) readString(bx);
   else if(ax == 2) readSector(bx,cx);
   else if(ax == 3) readFile(bx,cx,dx);
   else if(ax == 4) runProgram(bx,cx);
   else if(ax == 5) stop();
   else if(ax == 6) writeSector(bx,cx);
   else if(ax == 7) deleteFile(bx);
   else if(ax == 8) writeFile(bx,cx,dx);
   else if(ax == 11) interrupt(25,0,0,0,0);
   else if(ax == 12) clearScreen(bx, cx);
   else if(ax == 13) writeInt(bx);
   else if(ax == 14) readInt(bx);
   else if(ax == 15) error(bx);
   else printString("Error: Invalid call");
}
