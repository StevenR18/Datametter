#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <Windows.h>
#include "..\Vt\includes\virtual_t.h"
#include "..\Vt\includes\font.h" // contiene funciones para dibuajar caracteres
#include "..\Vt\includes\text_widget.h"


/*;;;;;;;;;;;;;;;;;;Funciones de la terminal virtual;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;*/
LineATributes process_escape_sequences(Line *line,HDC *hdc);




/*
BMP create_bmp(const char *name,
	       BGR * Color,
	       unsigned int widthInPixel,
	       unsigned int heightInPixel,
	       unsigned char bytePerPixel)
{
   BMP Bmp;
   BMPFILEHEADER bmpFileHeader={0};
   BMPINFOHEADER bmpInfoHeader={0};
   bmpFileHeader.bfType= BMP_TYPE;
   bmpFileHeader.bfSize= sizeof(BMPFILEHEADER);
   bmpFileHeader.bfOffBits= (sizeof(BMPFILEHEADER) + sizeof(BMPINFOHEADER));
   bmpInfoHeader.biSize= sizeof(BMPINFOHEADER);
   bmpInfoHeader.biWidth= widthInPixel;
   bmpInfoHeader.biHeight= heightInPixel;
   bmpInfoHeader.biPlanes= 1;
   bmpInfoHeader.biBitCount=bytePerPixel*8; // creará una imagen de 23 bits por pixel RGB
   bmpInfoHeader.biCompression= _RGB;
   bmpInfoHeader.biSizeImage= widthInPixel * heightInPixel * 3;

   FILE *File = fopen(name,"wb");
   if(File)
     {
       fwrite(&bmpFileHeader,sizeof(BMPFILEHEADER),1,File);
       fwrite(&bmpInfoHeader,sizeof(BMPINFOHEADER),1,File);
       // copiar los pixeles al archivo
       double mul4 = ceil(((double)(widthInPixel * 3) / 4)) * 4;
       int pading     = (mul4 - (widthInPixel *3));
       char arrPading[3]={0x00,0x00,0x00};
       for(int row=0; row < heightInPixel; row++)
	 {
	   for(int col=0; col < widthInPixel; col++)
	     {
	       fwrite(Color, sizeof(BGR),1,File);
	     }
	   fwrite(arrPading,pading,1,File);
	 }

       fclose(File);
       File=NULL;
       load_bmp(&Bmp, name);
     }
   else
     {
       // printf("\nNo se pudo crear el archiov %s",name);
     }
  
   return Bmp;
   
}

void load_bmp(BMP *Bmp, const char *path)
{
  BMPFILEHEADER bmpFileHeader;
  BMPINFOHEADER bmpInfoHeader;
  FILE * File= fopen(path,"rb");
  if(File)
    {
      int count= fread(&bmpFileHeader, sizeof(BMPFILEHEADER),1,File);
      if(count)
	{
	  count= fread(&bmpInfoHeader,sizeof(BMPINFOHEADER),1, File);
	  if(count)
	    {
	      if(bmpFileHeader.bfType == BMP_TYPE)
		{
		  int widthBmp     = bmpInfoHeader.biWidth;
		  int heightBmp    = bmpInfoHeader.biHeight;
		 
		  unsigned char arrPading[3];
		  if(bmpInfoHeader.biBitCount > 8)
		    {
		      // sin paleta
		      BGR Color;
		      int bitPerPixel  = 32;
		      int sizeBuffer32 = 4*widthBmp*heightBmp;
		      unsigned int * bufferPixel32 =(unsigned int *) malloc(sizeBuffer32);
		      double mul4 = ceil(((double)(widthBmp * 3) / 4)) * 4;
		      int pading     = (mul4 - (widthBmp *3));

		      for(int row =0; row < heightBmp; row++)
			{
			  for(int col=0; col < widthBmp; col++ )
			    {
			      unsigned int index  = widthBmp * row + col;
			      fread(&Color,sizeof(BGR),1,File);
			      unsigned int blue    = Color.blue;
			      unsigned int green   = Color.green;
			      unsigned int red     = Color.red;
			      
			      unsigned int color32 = ((0x00 <<24 )|
				                     (red << 16)|
				                     (green << 8)|
						      (blue));

			      bufferPixel32[index]=color32;
			    }
			  fread(arrPading,pading,1,File); // saltamos el pading
			  
			}
		      flip_bmp(bufferPixel32, sizeof(int),widthBmp,heightBmp);
		      Bmp->bmBits      = (void *)bufferPixel32;
		      Bmp->palete      = NULL;
		      Bmp->bmBitsPixel = bitPerPixel;
		      Bmp->bmPitch     = widthBmp * 4;
		      Bmp->bmHeight    = heightBmp;
		      Bmp->name        = path;		      
		    }
		  else
		    {
		      // con paleta
		      // primero leer la paleta
		      // luego leer los indices que apuntan hacia los colores de la pa

		      int sizePalete;
		      if(bmpInfoHeader.biClrUsed == 0)
			sizePalete = pow((double)2,(double)bmpInfoHeader.biBitCount);
		      else
			sizePalete =bmpInfoHeader.biBitCount;
		      RGB * Palete = (RGB *)malloc(sizeof(RGB)*sizePalete);
		      fread(Palete,(sizePalete *sizeof(RGB)),1,File);
		      double mul4       = ceil(((widthBmp)/4)) * 4;
		      int pading     = (mul4 - (widthBmp));

		      unsigned char * bufferOfIndex= (unsigned char *) malloc(widthBmp*heightBmp);
		      for(int row =0; row < heightBmp; row++)
			{
			  for(int col=0; col < widthBmp; col++ )
			    {
			      unsigned int index  = widthBmp * row + col;
			      unsigned char color;
			      fread(&color,sizeof(unsigned char),1,File);
			     
			      bufferOfIndex[index]=color;
			    }
			  fread(arrPading,pading,1,File); // saltamos el pading
			  
			}
		      flip_bmp(bufferOfIndex, sizeof(char),widthBmp,heightBmp);
		      Bmp->bmBits      = (void *)bufferOfIndex;
		      Bmp->palete      = Palete;
		      Bmp->bmBitsPixel = bmpInfoHeader.biBitCount;
		      Bmp->bmPitch     = widthBmp; // puede llevar pading?
		      Bmp->bmHeight    = heightBmp;
		      Bmp->name        = path;		
		    }

		  fclose(File);
		  
		}
	      else
		{
		  printf("\n No es un archivo BMP");
		}
	      
	    }
	  else
	    {

	      printf("\n Fallo al leer la cabezera BMPFILEHEADER");
	    }
	}
      else
	{
	  printf("\n Fallo al leer la cabezera BMPFILEHEADER");
	}
    }
  else
    {
      printf("\nNo se pudo cargar el archivo");
    }

  
    
}

void flip_bmp(void * pixel, unsigned int dataType, unsigned int width, unsigned int height)
{
  unsigned int pitch= width * dataType;
  unsigned int sizeBuffer=dataType*width * height;
  unsigned char * auxBuffer = (unsigned char *) malloc(sizeBuffer);
  memcpy(auxBuffer, pixel,sizeBuffer);
  unsigned  char * dest= (unsigned char *)pixel;
  unsigned  char * source = (unsigned char *) auxBuffer;
   
  for(int row=0; row < height; row++)
    {
      int indexDest= row * pitch;
      int indexSource= (height-1 - row) * pitch;
      memcpy((dest+indexDest), (source+indexSource), pitch);
      
    }

  free(auxBuffer);
  
}

void save_bmp(BMP *Bmp,const char* nameOu)
{

  BMPFILEHEADER bmpFileHeader;
  BMPINFOHEADER bmpInfoHeader;
  FILE * File= fopen(Bmp->name,"rb");
  if(File)
    {
      int count= fread(&bmpFileHeader, sizeof(BMPFILEHEADER),1,File);
      if(count)
	{
	  count= fread(&bmpInfoHeader,sizeof(BMPINFOHEADER),1, File);
	  if(count)
	    {
	      fclose(File);
	      File = NULL;
	      File = fopen(nameOu,"wb");
	      if(!File)return; // no se pudo abri el archivo
	      fwrite(&bmpFileHeader,sizeof(BMPFILEHEADER),1,File);
	      fwrite(&bmpInfoHeader,sizeof(BMPINFOHEADER),1,File);
		
	     	  int widthBmp     = bmpInfoHeader.biWidth;
		  int heightBmp    = bmpInfoHeader.biHeight;		  		 
		  unsigned char arrPading[3];
		  if(bmpInfoHeader.biBitCount > 8)
		    {
		      // sin paleta
		      BGR Color;
		       unsigned int * bufferPixel32 = (unsigned int *)Bmp->bmBits;
		       flip_bmp(bufferPixel32, sizeof(int),widthBmp,heightBmp);
		       double mul4 = ceil(((double)(widthBmp * 3) / 4)) * 4;
		       int pading     = (mul4 - (widthBmp *3));

		      for(int row =0; row < heightBmp; row++)
			{
			  for(int col=0; col < widthBmp; col++ )
			    {
			      unsigned int index  = widthBmp * row + col;
			      unsigned char * byte=(unsigned char *)&bufferPixel32[index];
			      
			      Color.blue  = byte[1];
			      Color.green = byte[2];
			      Color.red   = byte[3];
			      fwrite(&Color,sizeof(BGR),1,File);
			    }
			  fwrite(arrPading,pading,1,File); // saltamos el pading
			}	      
		    }
		  else
		    {
		      // con paleta
		      // primero leer la paleta
		      // luego leer los indices que apuntan hacia los colores de la pa

		      int sizePalete;
		      if(bmpInfoHeader.biClrUsed == 0)
			  sizePalete = pow(2,bmpInfoHeader.biBitCount);
		      else
			sizePalete =bmpInfoHeader.biBitCount;
		      RGB * Palete = Bmp->palete;
		      fwrite(Palete,(sizePalete *sizeof(RGB)),1,File);
		      double  mul4       = ceil(((double)(widthBmp ) / 4)) * 4;
		      int  pading     = (mul4 - (widthBmp));
		      unsigned char * bufferOfIndex=(unsigned char *)Bmp->bmBits;
		      flip_bmp(bufferOfIndex, sizeof(char),widthBmp,heightBmp);
		      for(int row =0; row < heightBmp; row++)
			{
			  for(int col=0; col < widthBmp; col++ )
			    {
			      unsigned int index  = widthBmp * row + col;
			      unsigned char color= bufferOfIndex[index];
			      fwrite(&color,sizeof(unsigned char),1,File);
			     
			    }
			  fwrite(arrPading,pading,1,File); // saltamos el pading
			  
			}
		    
		    }

		  fclose(File);
	    }
	  else
	    {

	      printf("\n Fallo al leer la cabezera BMPFILEHEADER");
	    }
	}
      else
	{
	  printf("\n Fallo al leer la cabezera BMPFILEHEADER");
	}
    }
  else
    {
      printf("\nNo se pudo cargar el archivo");
    }

 
}
*/
