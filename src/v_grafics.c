/*
  v_grafics.c modulo encargado de dibujarlos vectores en un liezo

 */
#include "..\..\Vt\includes\v_grafics.h"
#include "..\..\Vt\includes\dbg.h"
#include <stdio.h>
#include <stdlib.h>
VGRAFIC Gvgrafic={.vgHdc=NULL,.vghBitmap=NULL};

void inicVgrafics(VGRAFIC *vGrafic,
		  HDC * backBuffer,
		  int widthInPixel,
		  int heightInPixel)
{
  /// inicializar el hdc
  //  inicializar el bitmap
  vGrafic->vgHdc = CreateCompatibleDC(*backBuffer);
  vGrafic->vghBitmap = (HBITMAP)CreateCompatibleBitmap(*backBuffer,
						       widthInPixel,
						       heightInPixel);
  vGrafic->width  = widthInPixel;
  vGrafic->height = heightInPixel;
  SelectObject(vGrafic->vgHdc,vGrafic->vghBitmap);
}

void clearScreen(HDC *hdcVgrafic, int wDest,int hDest)
{
  
  HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
  HBRUSH holdBrush = (HBRUSH) SelectObject(*hdcVgrafic, hBrush);
  PatBlt(*hdcVgrafic,0,0,wDest,hDest,PATCOPY);
  SelectObject(*hdcVgrafic,holdBrush);
  DeleteObject(hBrush);
}

void renderScreen(VGRAFIC *vGrafic, HDC *backBuffer)
{
  HDC hdcDest = *backBuffer;
  HDC hdcSrc  = vGrafic->vgHdc;
  int wDest   = vGrafic->width;
  int hDest   = vGrafic->height;
  
  int ok = BitBlt(hdcDest,vGrafic->position.x,
		  vGrafic->position.y,
		  wDest,
		  hDest,
		  hdcSrc
		  ,0,0,
		  SRCCOPY);
  

  if(!ok)
    {
      OutputDebugStringA("Error\n");
      return;
    }
   clearScreen(&hdcSrc,wDest,hDest);
  /// limpiar  la pantalla despues de renderizar
}


void vgDrawScreen()
{
  //drawGrid(HDC *hdcTerm,int width,int height)

}






/**************************************************************************************
 *  -Cargar una imagen (listo)
 *  -Guardar iamgen (listo)
 *  -Aplicar filtros
 *    *convertir a escala de grises
 *    *binarizar una imagen
 *  -Vectorizar
 *  -extraer los bordes
 *    *elmimar bordes innesesarios
 *    *cerrar bordes abiertos
 * 
 *
 ***************************************************************************************/


/*
  Nombre de la funcion: Carga un Bmp

 */
Bmp loadBitmap(const char * path)
{
  Bmp myBmp;
  HBITMAP hBitmap =(HBITMAP) LoadImageA(NULL,
				  path,
				  IMAGE_BITMAP,
				  0,
				  0,
				  LR_LOADFROMFILE);
  
  int ok =GetObject(hBitmap,sizeof(BITMAP),&myBmp.bmp);
 
  int width    = myBmp.bmp.bmWidth;
  int height   = myBmp.bmp.bmHeight;
  int bitCount = myBmp.bmp.bmBitsPixel;

  HDC hdc   = GetDC(NULL);
  myBmp.memDc = CreateCompatibleDC(hdc);
  SelectObject(myBmp.memDc,hBitmap);

  BITMAPINFO bmInfo={0};
  myBmp.bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  myBmp.bmInfo.bmiHeader.biWidth = width;
  myBmp.bmInfo.bmiHeader.biHeight = -height; // Negativo para formato top-down
  myBmp.bmInfo.bmiHeader.biPlanes = 1;
  myBmp.bmInfo.bmiHeader.biBitCount = bitCount;
  myBmp.bmInfo.bmiHeader.biCompression = BI_RGB;

  myBmp.bufferPixelSize = (width * height * (bitCount/8));
  myBmp. pixels = (unsigned char *) malloc(myBmp.bufferPixelSize);
  GetDIBits(myBmp.memDc,hBitmap,0,height,myBmp.pixels,&myBmp.bmInfo,DIB_RGB_COLORS);
  return myBmp;
}


/*
  Nombre de la funcion: writeBmp()
  Crea un archivo bmp

 */

void writeBmp(const char * path, Bmp myBmp)
{
  // Abre el archivo de salida para escribir en él
  FILE *f = fopen(path, "wb");
  if (!f)
    {
        return; // Error al abrir el archivo
    }
    // Escribir encabezado del archivo BMP
    BITMAPFILEHEADER bfHeader = {0};
    bfHeader.bfType = 0x4D42; // 'BM' en hexadecimal
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfHeader.bfSize = bfHeader.bfOffBits + myBmp.bufferPixelSize;
    fwrite(&bfHeader, sizeof(BITMAPFILEHEADER), 1, f);
    // Escribir encabezado de la imagen
    BITMAPINFOHEADER *biHeader = &myBmp.bmInfo.bmiHeader;
    fwrite(biHeader, sizeof(BITMAPINFOHEADER), 1, f);
    // Escribir los datos de los píxeles de la imagen
    fwrite(myBmp.pixels, myBmp.bufferPixelSize, 1, f);
    // Cierra el archivo
    fclose(f);
}

/*
  Nombre de la funcion: converterToGrisEscale()
  Convierte a escala de grises un Bmp
  devuelve una imagen con un solo canal
 */

Bmp  convertToGrisEscale(Bmp *src)
{
  // bmp de salida de un canal
  Bmp bmp8;
  // crear un DC compatible
  bmp8.memDc = CreateCompatibleDC(NULL);
  // llenar la informacion del bitmap info
  bmp8.bmInfo.bmiHeader.biSize        =  sizeof(BITMAPINFOHEADER);
  bmp8.bmInfo.bmiHeader.biWidth       =  src->bmInfo.bmiHeader.biWidth;
  bmp8.bmInfo.bmiHeader.biHeight      =  src->bmInfo.bmiHeader.biHeight;
  bmp8.bmInfo.bmiHeader.biPlanes      =  1;
  bmp8.bmInfo.bmiHeader.biBitCount    =  8; // 8 bits por pixel
  bmp8.bmInfo.bmiHeader.biCompression =  BI_RGB;
  bmp8.bmInfo.bmiHeader.biSizeImage   = src->bmInfo.bmiHeader.biWidth * src->bmInfo.bmiHeader.biHeight;
  bmp8.bmInfo.bmiHeader.biXPelsPerMeter  = src->bmInfo.bmiHeader.biXPelsPerMeter;
  bmp8.bmInfo.bmiHeader.biYPelsPerMeter  = src->bmInfo.bmiHeader.biYPelsPerMeter;
  bmp8.bmInfo.bmiHeader.biClrUsed     = 256; // usa una paleta de colores
  bmp8.bmInfo.bmiHeader.biClrImportant = 256; // todos los colores son importantes

  // cear un paleta de escala de grises
  for (int x=0; x< 256; x++)
    {
      bmp8.bmInfo.bmiColors[x].rgbBlue = x;
      bmp8.bmInfo.bmiColors[x].rgbGreen = x;
      bmp8.bmInfo.bmiColors[x].rgbRed = x;
      bmp8.bmInfo.bmiColors[x].rgbReserved = x;
    }

  /// crear un bitmap compatible de 8 bits con create dibseccion
  HBITMAP hBitmap= CreateDIBSection(bmp8.memDc,(BITMAPINFO *)&bmp8.bmInfo,DIB_RGB_COLORS,(void **)&bmp8.pixels,NULL,0);

  // convertir los pixeles a escala de grisese
  //diseñar el algoritmo para que convierta la imagen a escala de grises
  
  SelectObject(bmp8.memDc, hBitmap);
  // Calcular el tamaño del buffer de píxeles
  bmp8.bufferPixelSize = bmp8.bmInfo.bmiHeader.biSizeImage;
  return bmp8;
  
}








