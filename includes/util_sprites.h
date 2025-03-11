#ifndef SPRITE_H
#define SPRITE_H
#include <windows.h>
#define MAX_SPRITES 4
#define FRAME_DURATION 100

typedef struct Entity
{
  POINT      vPosition;   // vector posicion x,y
  POINT      vVelocity;   // vecotor velocidad x,y
  HBITMAP    anime[MAX_SPRITES];
  HBITMAP    mask[MAX_SPRITES];
  int        currentFrame;
  int        frameTime;
  
}Entity;

HBITMAP loadBmp(const char *path);
//void drawSprite(HBITMAP sprite, HDC *hdc,POINT pos);
void drawSprite(Entity *e, HDC *hdc);
void moveSprite(Entity *e, int deltaTime);

#endif
