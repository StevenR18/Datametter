
typedef struct
{
  void (*funcion)(const wchar_t * params,HDC *memdc); // funcion
  wchar_t charFuncion; // cadena para saber de que funcion se trata
}FUNCIONVT100;

void switchCntrl(wchar_t cntrl)
{
  switch(cntrl)
    {
    case L'\n':
      {
	cursor.y++;
      }break;
    case L'\r':
      {
        cursor.x = 0;
      }break;
    case L'\t':
      {
	cursor.x+=4;
      }break;
    }
}

#define NUMBER_FUNCION 19
void processFuncion(wchar_t funcion, wchar_t* params, FUNCIONVT100 * arrf,HDC *memdc)
{
  if((funcion == L'h') || (funcion == L'i'))
    {
      params[3]=funcion;
      params[4]=L'\0';
      funcion= L'?';
    }
  if(iswcntrl(funcion))
    {
      switchCntrl(funcion);
      return;
    }
    for (int x = 0; x < NUMBER_FUNCION; x++)
    {
        if (arrf[x].charFuncion == funcion)
        {
	  void (*func)(const wchar_t *paramas,HDC *memdc) = arrf[x].funcion;
	  func(params,memdc);
        }
    }
}

#include "vtfunc.c"

FUNCIONVT100 arrFuncionVT100[]=
  {
    {.funcion=cursor_up_inverse_newline,.charFuncion= L'M'}, // realiza la funcion inversa de \n
    {.funcion=save_cursor,.charFuncion=L'7'}, // permite guardar la posicion actual del cursor
    {.funcion=restore_cursor,.charFuncion=L'8'}, // restaura la posicion del cursor guardada en me
    {.funcion=csi_cursor_up,.charFuncion=L'A'},  // cusor hacia delante
    {.funcion=csi_cursor_down,.charFuncion=L'B'}, //cusosr hacia abajo
    {.funcion=csi_cursor_C,.charFuncion=L'C'},  ///.......
    {.funcion=csi_cursor_D,.charFuncion=L'D'}, //...
    {.funcion=csi_cursor_E,.charFuncion=L'E'}, ///... etc.
    {.funcion=csi_cursor_F,.charFuncion=L'F'},
    {.funcion=csi_cursor_G,.charFuncion=L'G'},
    {.funcion=csi_cursor_d,.charFuncion=L'd'},
    {.funcion=csi_cursor_H,.charFuncion=L'H'},
    {.funcion=csi_cursor_f,.charFuncion=L'f'},
    {.funcion=csi_cursor_s,.charFuncion=L's'},
    {.funcion=csi_cursor_u,.charFuncion=L'u'},
    {.funcion=process_simple_escape_sequence,.charFuncion=L'z'},
    {.funcion=csi_cls_screen_ED_J,.charFuncion=L'J'}, // BORRA EN PANTALLA
    {.funcion=csi_cls_screen_EL_K,.charFuncion=L'K'}, // BORRA EN LINIAS
    {.funcion=csi_cursor_ctrl,.charFuncion=L'?'} // BORRA EN LINIAS 
  };


void processEscape(Node *n, HDC *hdc)
{
  #define str n->s 
  
  if(n == NULL || str == NULL) return; // si el nodo es nulo salimos de la funcion
  int x=0;
  while(str[x] != L'\0')
    {
      if(str[x] == L'\x1b')
	{
	  x++; // contamos x1b
	  if(str[x] == L'[')
	    {
	      x++;// contamos el [
	      int lenP=0; // cuenta la cantidad de parametros incluido;
	      wchar_t bp[100]; // buffer de parametros
	      while(!iswalpha(str[x+lenP]))lenP++;
	      wchar_t f= str[x+lenP];//funcion
	      wcsncpy(bp,&str[x],lenP);
	      processFuncion(f,bp,arrFuncionVT100,hdc);
	      x+=lenP+1;
	    }
	  else
	    {
	      // secuenia sin parametros
	       processFuncion(str[x],NULL,arrFuncionVT100,hdc);
	       x++;
	    }
	}
      else
	{
	  // secuencia de control
	   processFuncion(str[x],NULL,arrFuncionVT100,hdc);
	   x++;
	}
      
    }
}

// funcion principla
#define isEsc(node) ((node)->s[0] == L'\x1b')
#define isCtrl(node)(iswcntrl((node)->s[0]))
void write(HDC *memdc)
{
  Node * current= head;

  while(current)
    {
      if(isEsc(current)||isCtrl(current)) // pregunta si es una secuencia de escape
	{
	  processEscape(current,memdc);
	}
      else
	{
	  /// caso contrario es una palabra
	  
	  draw_font(memdc,current->s,cursor.x,cursor.y);
	}
      current=(Node *) current->next;
      
    }
  
}








