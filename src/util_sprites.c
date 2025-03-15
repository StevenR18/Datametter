/*
   este archivo contendra funciones para trabajar con sprites
   -loadSprite() devuelve un HBITMAP
   -drawSprinte() dibua el sprite

 */
#include "..\..\Vt\includes\util_sprites.h"
HBITMAP loadBmp(const char *path)
{
  HBITMAP bmp =(HBITMAP) LoadImageA(NULL,
				  path,
				  IMAGE_BITMAP,
				  0,
				  0,
				  LR_LOADFROMFILE);
  if(!bmp)
    {
      return NULL;
    }
  return bmp;
}


void drawSprite(Entity *e, HDC *hdc)
{
  if (e == NULL) return;
  HDC memdc = CreateCompatibleDC(*hdc);
  HDC maskdc = CreateCompatibleDC(*hdc);
  SelectObject(memdc, e->anime[e->currentFrame]);
  SelectObject(maskdc, e->mask[e->currentFrame]);
  // Aplicar la máscara: solo las partes blancas de la máscara se dibujarán
  
  BitBlt(*hdc, e->vPosition.x, e->vPosition.y, 16, 16, maskdc, 0, 0, SRCAND);
  
  BitBlt(*hdc, e->vPosition.x, e->vPosition.y, 16, 16, memdc, 0, 0, SRCPAINT);
  // Dibujar el sprite encima, usando la máscara

  DeleteDC(memdc);
  DeleteDC(maskdc);
}


/*fisicas */

void moveSprite(Entity *e, int deltaTime)
{
   if (e == NULL) return;

    // Calcula el desplazamiento en base a la velocidad y el tiempo
   e->vPosition.x += (e->vVelocity.x * deltaTime)/10;
   e->vPosition.y += (e->vVelocity.y * deltaTime)/10;

    // Acumulamos el tiempo para cambiar de frame
    e->frameTime += deltaTime;

    // Cambia de frame si pasa el tiempo necesario
    if (e->frameTime >= 100) {
        e->currentFrame = (e->currentFrame + 1) % MAX_SPRITES;
        e->frameTime = 0;
    }
   
}








