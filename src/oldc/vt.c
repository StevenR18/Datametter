
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <windows.h>
#include "..\Vt\includes\vt.h"
#include "..\Vt\includes\font.h"
#include "..\Vt\includes\window64_platform.h"
#include "..\Vt\includes\keyboard.h"
#include "..\Vt\includes\liked_list.h"
#include "..\Vt\includes\main_datametter.h"
#include "..\Vt\includes\memory_traker.h"

extern HDC GHdc; // HDC QUE PERTENECE  A LA CAPA  DE PLATAFORMA
extern HWND Wwindow;
extern Term emu;
/* Funciones para manenejar el buffer de la consola*/
#define CURSOR_IS_VISIBLE(flag)(flag & 0x01)// revisa si el ultimo bit esta activo
#define CURSOR_IS_BLINKING(flag)(flag &0x02) //revisa si el segundo bit esta activo 
#define CURSOR_BLOQUE        0
#define CURSOR_EMPTY         1
#define CURSOR_BARRABAJA     2
#define CURSOR_BARRAVERTICAL 3
#define CTRL_SEQ(ascii) ((ascii) & 0x1F)
typedef struct
{
  POINT position;
  wchar_t *ptrEscape;
}LineATributes;
/*
typedef struct
{
  POINT *pCxy;        // posicion del cursor 
  wchar_t *glifo;     // caracter que muestra el cursor 
  unsigned char flag; // flag para indicar que propiedades estan activas   
}Cursor;
*/
wchar_t cursorSymbols[4] = {
        L'█', // Bloque
	L' ',
        L'_', // Barra baja
        L'|' // Barra vertica
       
    };


unsigned char enableBlinkCursor=0; // se activa cada segundo
POINT saveCursor={0};
POINT cursor={.x=0,.y=0};
POINT oldCursor={.x=0,.y=0};

Cursor cur={
  .pCxy=&cursor,
  .glifo=&cursorSymbols[CURSOR_BLOQUE],
  .flag=0x00};   
Nodo * inputBuffer=NULL;

LineATributes vt_process_escape_sequences(Line *line, HDC *hdc);
void vt_reset_cursor();

int vt_len_char_printable(const wchar_t *ptrChar)
{
  int len=0; 
  while((isprint(*ptrChar)||(*ptrChar >= 0x2500) && (*ptrChar <= 0x257F)))
    {
      len++;
      ptrChar++;
    }
  return len;
}
/*
  char * translate_keyevent_tovt_sequence
  traduce un evento de teclado a una secuencia de escape correspondiente al evento

 */

void bubleSort(KeyboardEvent *k)
{
  
  int n=BUTTON_COUNT; // Número de botones en el evento
  for (int i = 0; i < n - 1; i++)
    {
      for (int j = 0; j < n - i - 1; j++)
	{
	  // Comparar el campo `ordinal`
	  if (k->orderButtons[j].ordinal > k->orderButtons[j + 1].ordinal)
	    {
	      // Intercambiar los elementos si están en el orden incorrecto
	      Button temp = k->orderButtons[j];
	      k->orderButtons[j] = k->orderButtons[j + 1];
	      k->orderButtons[j + 1] = temp;
            }
        }
    }
}

Nodo ** translate_keyevent_tovt_sequence(KeyboardEvent keyEvent)
{
  /*
    Navegación: Enter, Tab, Esc, Backspace.
    // Modificación: Shift, Ctrl, Alt, Caps Lock.
    Navegación avanzada: Arrow Keys, Home, End, Page Up, Page Down.
    Funcionales: F1 a F12.
    Sistema: PrtSc, Pause, Insert, Delete.
    ORDENAR LOS EVENTOS DE TECLADO
   */
  if(ispressedAnyKey()|| isdownAnyKey())
    {
      {
	int x=0;
	while(keyEvent.orderButtons[x].isDown != 255)
	  {
	    keyEvent.orderButtons[x].vk = 255;
	    keyEvent.orderButtons[x].processed = 255;
	    keyEvent.orderButtons[x].ordinal = 255;
	    keyEvent.orderButtons[x].isDown = 255;
	    keyEvent.orderButtons[x].changed = 255;
	  }
      }
      for (int x = 0; x < 255; x++)
	{
	if(keyEvent.buttons[x].isDown == 1)
	  {
	    keyEvent.orderButtons[x].vk = keyEvent.buttons[x].vk;
	    keyEvent.orderButtons[x].processed =keyEvent.buttons[x].processed;
	    keyEvent.orderButtons[x].ordinal = keyEvent.buttons[x].ordinal;
	    keyEvent.orderButtons[x].isDown = keyEvent.buttons[x].isDown;
	    keyEvent.orderButtons[x].changed = keyEvent.buttons[x].changed;
	  }
	}
	bubleSort(&keyEvent); // ordena los eventos segun el orden de llegada
       #if 1
	debugKeyboard(keyEvent);
#endif
#define MAX_EVENT ((x <= BUTTON_COUNT )&& (keyEvent.orderButtons[x].isDown!=255))
	for(int x=1; MAX_EVENT; x++)
	{
	  if(ispressedCtrlAt(keyEvent.orderButtons[x])||
	     isdownCtrlAt(keyEvent.orderButtons[x]))
	    {
	      // Modificación: Shift, Ctrl, Alt, Caps Lock.
	      if(downAt(keyEvent.orderButtons[x],BUTTON_CTRL) &&(ispressedAlphaAt(keyEvent.orderButtons[x+1])))
		{
		  /// Obtener el carcter alfabetico que se extrajo CTRL+A
		  char button= (char)keyEvent.orderButtons[x+1].vk;
		  switch(button)
		    {
		    case 'Q':
		    case 'q':
			{
			  insert_nodo_end(&inputBuffer,"\x11",2);
			  // generar la secuencia de escape correspondiente al ctrl+q
			}break;
		    default: break;
		    }
		  x++;
		}
	      else
		{
		  /// si se presiona una tecla de flecha
		  if(ispressedArrowKeyAt(keyEvent.orderButtons[x]))
		    {     
		      // obtener todos los botones arrowkey
		      /// si quieres procesar mas de una pulsacion las teclas flecha
		      /// deves de generar 2 secuencias de escape
		      switch(keyEvent.orderButtons[x].vk)
			{
			case BUTTON_UP:insert_nodo_end(&inputBuffer,"\x1b[A",4);break;
			case BUTTON_DOWN:insert_nodo_end(&inputBuffer,"\x1b[B",4);break;
			case BUTTON_RIGHT:insert_nodo_end(&inputBuffer,"\x1b[C",4);break;
			case BUTTON_LEFT:insert_nodo_end(&inputBuffer,"\x1b[D",4);break;
			default: break;
			}
		    }
		  
		}
	    }
	      
	}// end for
    }// end if
  return &inputBuffer;
}




Line *vt_create_newLine(const wchar_t **str)
{
    Line *newLine = (Line *)mymalloc(sizeof(Line),__func__,__FILE__);
    if (newLine == NULL) return NULL;
    size_t sizeStr = (str[0] == NULL) ? 0 : (wcslen(str[0]) + 1); 
    size_t sizeEsc = (str[1] == NULL) ? 0 : (wcslen(str[1]) + 1);
    if (sizeStr > 0)
      {
	newLine->ptrStr = (unsigned char *)mymalloc(sizeof(wchar_t) * sizeStr,__func__,__FILE__);
	if (newLine->ptrStr == NULL) return NULL;
	wcscpy_s((wchar_t *)newLine->ptrStr, sizeStr, str[0]);
        newLine->len = (int)(sizeStr - 1);
      }
    else
      {
	newLine->ptrStr = NULL;
      }
    if (sizeEsc > 0)
      {
	newLine->ptrEscape = (unsigned char *)mymalloc(sizeof(wchar_t) * sizeEsc,__func__,__FILE__);
	if (newLine->ptrEscape == NULL) return NULL;
	wcscpy_s((wchar_t *)newLine->ptrEscape, sizeEsc, str[1]);
      }
    else
      {
	newLine->ptrEscape = NULL;
      }
    newLine->isEnableEsc = 0; // Inicializamos el flag para secuencias de escape
    newLine->next = NULL; // Inicializamos el puntero al siguiente nodo como NULL
    return newLine;
}

/*
 * add_line2  es una funcion para crear un array de listas enlazadas, a paratir de
 * cadenas ,las cadenas estan divididad en palabras, secuencias de escape: ejempl
 * Palabra: "hola", "Mundo" etc
 * Secuencias de escape: "\x1[A" , "\n" etc.
 * la lista se llena de esta forma :
 * Secuencias de escape = SE
 * Palabra = WORD
 * Entonces: si tenemos un array  de caracteres que tiene esto [SE|WORD|WORD|SE], este
 * array puede estar en cualquier, tonces la lista tiene que formarse en ese orde
 * [SE|ptrSigiente]--> [WORD|ptrSigiente]-->[WORD|ptrSigiente]-->[SE|ptrSigiente]-->null
 *--------------------------------------------------------------------------------------
 *Parametros de entrada (ListLine *, const char *)
 *ListLine: Miembros Line**, int countLine
 *           Miembors Line : const char * ptrStr (Almacena la cadena), int len (longitudo * de cadena)
 *--------------------------------------------------------------------------------------
 * Retorno : retorna un (int) que son el numero de Linias agegadas
 *-------------------------------------------------------------------------------------
 * Estructura de list line:
 * ListLine * -> [ListLine]-->[Line**]---->[*Line]-->[[WORD|ptrSig]]->[[WORD|ptrSig]]-
 *                                         [*Line+1]->[[WORD|ptrSig]]->[[WORD|ptrSig]]
 *                                         [*Line+2]
 *                                         [*Line+n] 
 *
 ****************************************************************************************
 */
// todo modificar la funcion para obtner las secuencias de escape y poder obtenerlas
// 1 por 1

int vt_len_escape(const wchar_t *sscape)
{
    if (!sscape) return 0; // Validación de puntero nulo

    int len = 0;

    while (*sscape != L'\0') {
        // Verificar si es una secuencia de escape (ESC o carácter de control)
        if (*sscape == L'\x1b' || iswcntrl(*sscape)) {
            len++; // Contamos el carácter inicial (ESC o de control)
            sscape++;

            if (*sscape == L'[') {
                len++; // Contamos el carácter '['
                sscape++;

                // Contar los parámetros dentro de la secuencia (números, ';', etc.)
                while (*sscape != L'\0' && !iswalpha(*sscape)) {
                    len++;
                    sscape++;
                }

                // Verificar si hay un carácter alfabético final
                if (iswalpha(*sscape)) {
                    len++; // Contamos el carácter final
                    sscape++;
                }
            } else {
                // Si no hay '[', avanzar al siguiente carácter
	      if(iswcntrl(*sscape))
		continue;
	      else
		break;
            }
        } else {
            // Si no es una secuencia de escape, simplemente avanzamos
            sscape++;
        }
    }

    return len;
}


/// agrega una cadena al final de cada linia

int vt_add_end_nodo(Line **head,Line *line)
{
 
  static Line *aux=NULL;
  if(*head == NULL)
    {
      Line *lastNodo= line;
      *head= lastNodo;
       aux= *head;
      return 1;
    }
   Line *lastNodo=line;
   aux->next=lastNodo;
   aux = aux->next;
   return 0;  
}

/*TODO ARREGLA ESTA FUNCION  TIENE FUGA DE MEMRORIA*/

int vt_add_line2(Line **head, const wchar_t *str, int len)
{
  const wchar_t *firstChar = str;
  int x = 0;
  while (firstChar[x] != L'\0') // Usa `L'\0'` para cadenas de `wchar_t`
    {
      int countElement = 0;
      Line *aux = NULL;
      wchar_t *ptrChars[2] = {NULL, NULL};
      int direccion=0;
      while (countElement < 2 && firstChar[x] != L'\0')
	{
	  if (iswprint(firstChar[x])||((firstChar[x] >= 0x2500) &&
					 (firstChar[x] <= 0x257F)))
	    {
	      // Extracción de la cadena
	      int lenPrintable = len/*vt_len_char_printable(&firstChar[x])*/;
	      ptrChars[0] = (wchar_t *)mymalloc(sizeof(wchar_t) * (lenPrintable + 1),__func__,__FILE__);
	      if (ptrChars[0] == NULL) return -1;
	      wcsncpy(ptrChars[0], &firstChar[x], lenPrintable);
	      ptrChars[0][lenPrintable] = L'\0'; // Usa `L'\0'` para el terminador nulo
	      x += lenPrintable;
	      
	      
	     
	    }
	  else
	    {
	      if (iswcntrl(firstChar[x]))
                {
		  const wchar_t *iterChar = &firstChar[x];
		  int lenNoPrintable = vt_len_escape(iterChar);
		  ptrChars[1] = (wchar_t *)mymalloc(sizeof(wchar_t)*(lenNoPrintable + 1),__func__,__FILE__);
		  if (ptrChars[1] == NULL) return -1;
		  wcsncpy(ptrChars[1], &firstChar[x], lenNoPrintable); // Usa `wcsncpy`
		  ptrChars[1][lenNoPrintable] = L'\0';
		  x += lenNoPrintable;
		  direccion= countElement; // si direccion esta en cero tonses esta
		                           // a la isquierda
                }
            }
            countElement++;
        }
      aux = vt_create_newLine((const wchar_t *)ptrChars);
      aux->isEnableEsc = 1;
      aux->dirEsc=direccion; //0= <isquierda> 1= <derecha>
      vt_add_end_nodo(head, aux); // Agregamos el nodo
      if (ptrChars[0] != NULL) myfree(ptrChars[0]);
      if (ptrChars[1] != NULL) myfree(ptrChars[1]);
      ptrChars[0] = NULL;
      ptrChars[1] = NULL;
    }
    return 1;
}

int vt_free_list(Line **head)
{
  if((*head == NULL)) return -1;
  while(*head!=NULL)
    {
       Line * temp=*head;
       *head = (*head)->next;
       myfree(temp);
    }
  *head=NULL;
  return 0;
}


/*fuciones para procesesar  el buffer de la consola */
/*
Vterm *vt_inic_virtualt(int Width, int Height,unsigned char * NameFont)
{
  Vterm *consol = (Vterm *)malloc(sizeof(Vterm));
  consol->Cursor.x=0;
  consol->Cursor.y=0;
  consol->Width=Width;
  consol->Height=Height;
  consol->MaxCol = Width/8;
  consol->MaxRow = Height/16;
  consol->NameFont= NameFont;
  return consol;
}

void vt_free_console(Vterm * consol)
{

  free(consol);
}
*/
/*
  render_line : Recibe un buffer de linias que en errealidad es un estrucutra
                 con un buffer de caracteres, [a,e,b,\n,ESC] debe ser capas
		 de renderizar los caracteres imprimibles y actuar de acuerdo
		 a las secuencias de escape moviendo el cursor, haciendo saltos de linea
		 retornos de caro, cambiar de color los caracrteres,limpiar la pantalla,

 */


int vt_update_atri(Line * line, LineATributes * atri)
{
  if(line == NULL) return -1;
  line->pxy=atri->position;
  return 1;
}

/*actualizar la funciones render_lines para procesar las secuencias de escape*/
int vt_write(Line **lines, HDC *memdc)
{
  // TODO ARREGLAR ESTA FUCNION
  
    if (lines == NULL || memdc == NULL) return -1; // Validar punteros
        Line *firstLine = *lines;
        LineATributes atri={0}; 
        while (firstLine != NULL)
        {
	  int lenLine = firstLine->len;
	  wchar_t *str = (firstLine->ptrStr!= NULL)?firstLine->ptrStr:NULL;
	  wchar_t *esc = firstLine->ptrEscape;
	  unsigned char flag = (esc != NULL && str !=NULL)? firstLine->isEnableEsc : 0;
	  if(flag)
	    {
	      // un nodo completo
	      // aqui tengo que saver si la secuencias se ecuentra al lado isquierdo
	      // o derecho de la palbra
	      if(firstLine->dirEsc == 0)
		{
		  atri=vt_process_escape_sequences(firstLine,memdc);
		  cursor= atri.position;
	          vt_update_atri(firstLine,&atri);
		  cursor= atri.position; // actualizamos el cursor
	          firstLine->isEnableEsc=0;
		  draw_font(memdc, str, firstLine->pxy);
		  firstLine->isEnableEsc=0;
		  firstLine = firstLine->next; // Avanzar al siguiente nodo
		  continue;
		  
		}
	      
		
		  draw_font(memdc, str, cursor);
		  atri=vt_process_escape_sequences(firstLine,memdc);
		  cursor= atri.position;
		  firstLine->isEnableEsc=0;
		
	      
	      
	     
	    }
	  else
	    {
	      if (str != NULL && lenLine > 0)
		{
	      // un nodo que solo tine una cadena pero no secuencia de escape
		  printf("cadena %s\n",str);
		
		firstLine->pxy=cursor;
		//if(cursor.y >= 40) vt_reset_cursor();
		//TextOutA(*memdc,(pxy.x*7),(pxy.y*16),(LPCSTR)str, lenLine);
		draw_font(memdc, str, firstLine->pxy);
		cursor.x+=(lenLine);
		}
	      else
		{
		  if(esc)
		    {
		      /// caso en el que solo hay secuencia de escape
		      atri=vt_process_escape_sequences(firstLine,memdc);
		      //vt_update_atri(firstLine,&atri);
		      cursor=atri.position;
		      firstLine->isEnableEsc=0;
		    }
		}
	    }
	  
	    
            firstLine = firstLine->next; // Avanzar al siguiente nodo
        }
    return 1; // Éxito
}

typedef struct
{
  void (*funcion)(const wchar_t * params, LineATributes * atri,HDC *memdc); // funcion
  wchar_t charFuncion; // cadena para saber de que funcion se trata
}FUNCIONVT100;

/// crear un arreglo de esta estructura que contenga las diferente funciones

// secuencias de escape para manipular el cursor

void cursor_up_inverse_newline(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    if (*params == L'M')  // Usamos L'M' para caracteres anchos
        cursor.y -= 1;    // Mueve el cursor una línea hacia arriba
    atri->position = cursor;
}

void save_cursor(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    if (*params == L'7')  // Usamos L'7' para caracteres anchos
        saveCursor = cursor;
    atri->position = cursor;
}

void restore_cursor(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    if (*params == L'7')  // Usamos L'7' para caracteres anchos
        cursor = saveCursor;
    atri->position = cursor;
}

// Funciones CSI
void csi_cursor_up(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
    int n = 0;
    if (len > 0)
        n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
    else
        n = 1;  // Si no hay parámetros, se asume que se movió 1 línea
    cursor.y -= n;
    atri->position = cursor;
}

void csi_cursor_down(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
    int n = 0;
    if (len > 0)
        n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
    else
        n = 1;  // Si no hay parámetros, se asume que se movió 1 línea
    cursor.y += n;
    atri->position = cursor;
}

void csi_cursor_C(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    // sin len en 0 quiere decir que se omitieron los parametros
    int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
    int n = 0;
    if (len > 0)
        n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
    else
        n = 1;  // Si no hay parámetros, se asume que se movió 1 columna
    cursor.x += n;
    atri->position = cursor;
}

void csi_cursor_D(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    // sin len en 0 quiere decir que se omitieron los parametros
    int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
    int n = 0;
    if (len > 0)
        n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
    else
        n = 1;  // Si no hay parámetros, se asume que se movió 1 columna
    cursor.x -= n;
    atri->position = cursor;
}

void csi_cursor_E(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    // sin len en 0 quiere decir que se omitieron los parametros
    int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
    int n = 0;
    if (len > 0)
        n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
    else
        n = 1;  // Si no hay parámetros, se asume que se movió 1 fila
    cursor.y += n;
    atri->position = cursor;
}

void csi_cursor_F(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
    // sin len en 0 quiere decir que se omitieron los parametros
    int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
    int n = 0;
    if (len > 0)
        n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
    else
        n = 1;  // Si no hay parámetros, se asume que se movió 1 fila
    cursor.y -= n;
    atri->position = cursor;
}


/*secuencias de escape  para manipular el aspecto del cursor*/


/// se mueve  de manera orizontal a la posicion que le indique de manera absoluta

void csi_cursor_G(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
  int n = 0;
  if (len > 0)
     n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
  else
    n = (wcstol(params, NULL, 10) + 1);  // wcstol en lugar de atoi
  cursor.x = n;
  atri->position = cursor;
}

void csi_cursor_d(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  int len = (int) wcslen(params);  // Usamos wcslen para cadenas de wchar_t
  int n = 0;
  if (len > 0)
     n = (wcstol(params, NULL, 10) == 0) ? 1 : wcstol(params, NULL, 10);  // wcstol para convertir a entero
  else
    n = (wcstol(params, NULL, 10) + 1);  // wcstol en lugar de atoi
  cursor.y = n;
  atri->position = cursor;
}

void csi_cursor_H(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  /// todo
  //hay que controlar  tambien cunado no tiene parametros
  wchar_t* buffer;
  wchar_t * token = wcstok((wchar_t *)params, L";",&buffer);
  int x = 0;
  int n = 0;
  int valueParams[2];
  while (token != NULL)
  {
    valueParams[x] = (int) wcstol(token, NULL, 10);  // wcstol para convertir a entero
    token = wcstok(NULL, L";",&buffer);
    x++;
  }
  
  for (int y = 0; (y < 2 && x != 0); y++)
  {
    if (valueParams[y] > 0)
      n = (valueParams[y] == 0) ? 0 : valueParams[y];
    else
      n = (valueParams[y]);
    if (y == 0) cursor.y = n;
    else cursor.x = n;
  }
  if(x == 0)
    {
      cursor.x=0;
      cursor.y=0;
    }
  
  atri->position = cursor;
}

void csi_cursor_f(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  csi_cursor_H(params, atri,memdc);
}

void csi_cursor_s(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  if (*params == L's') saveCursor = cursor;  // Cambié 's' por L's' para usar wchar_t
  atri->position = cursor;
}

void csi_cursor_u(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  if (*params == L's') cursor = saveCursor;  // Cambié 's' por L's' para usar wchar_t
  atri->position = cursor;
}

/* Secuencias de escape simples */
void process_simple_escape_sequence(const wchar_t *params, LineATributes *atri,HDC *memdc)
{
  wchar_t es = *params;  // Cambié char por wchar_t
  switch (es)
  {
    case L'\n':  // Cambié '\n' por L'\n' para usar wchar_t
    {
      cursor.y += 1;
      atri->position = cursor;
    } break;
    case L'\r':  // Cambié '\r' por L'\r' para usar wchar_t
    {
      cursor.x = 0;
      atri->position = cursor;
    } break;
    default:
      break;
  }
}

/*secuencias de escape para borrar la pantalla*/
void csi_cls_screen_EL_K(const wchar_t *params, LineATributes *atri, HDC *memdc)
{
    /* si <n> se omite el valor predeterminado e cero
       0: borra contenido desde la posición actual del cursor (inclusive) hasta el final de la línea o pantalla.
       1: borra contenido desde el principio de la línea o la pantalla hasta la posición actual del cursor (inclusive).
       2: borra la línea o pantalla completa.
     */

    wchar_t *buffer;
    wchar_t *token = wcstok((wchar_t *)params, L";", &buffer);
    int n = 0;
    if (token && !iswalpha(*token))
      {
        n = (int)wcstol(token, NULL, 10);
      }
    POINT currentCursor = cursor;
    int x0 = 0, xf = (emu.screencol-1), dx = 0;
    wchar_t *line = NULL;

    switch (n)
      {
        case 0: // Desde el cursor hasta el final
            x0 = currentCursor.x;
            dx = (xf - x0) + 1; // Incluye terminador nulo
            break;
        case 1: // Desde el inicio hasta el cursor
            x0 = 0;
            xf = currentCursor.x;
            dx = (xf - x0) + 1; // Incluye terminador nulo
            break;
        case 2: // Línea completa
        default:
            x0 = 0;
            break;
      }

    line = (wchar_t *)mymalloc(sizeof(wchar_t) * dx,__func__,__FILE__);
    if (!line) {
        wprintf(L"Error: no se pudo asignar memoria.\n");
        return;
    }

    wmemset(line, L' ', dx - 1);
    line[dx - 1] = L'\0';

    draw_font(memdc, (const wchar_t *)line, currentCursor);
    atri->position=cursor;

    // Liberar memoria
    myfree(line);
}

void csi_cls_screen_ED_J(const wchar_t *params, LineATributes *atri, HDC *memdc)
{
  /// BORRA EN PANTALLA

  /* si <n> se omite el valor predeterminado e cero
       0: borra contenido desde la posición actual del cursor (inclusive) hasta el final de la línea o pantalla.
       1: borra contenido desde el principio de la línea o la pantalla hasta la posición actual del cursor (inclusive).
       2: borra la línea o pantalla completa.
     */

    wchar_t *buffer;
    wchar_t *token = wcstok((wchar_t *)params, L";", &buffer);
    int n = 0;
    wchar_t bufferP[20];
    if (token && !iswalpha(*token))
      {
        n = (int)wcstol(token, NULL, 10);
      }
    POINT currentCursor = cursor;
    int x0 = 0, y =(emu.screenrow-1), dx = 0;
    wchar_t *line = NULL;
    switch (n)
      {
      case 0:// Desde el cursor hasta el final
	x0 = currentCursor.x;
	break;
      case 1: // Desde el inicio hasta el cursor
	x0 = 0;
	break;
      case 2: // Línea completa
      default:
	x0 = 0;
	break;
      }

    for (int i=0; i < y; i++)
      {
	swprintf(buffer, 20 ,L"%d;%d", x0, (currentCursor.y+i));
	csi_cls_screen_EL_K(bufferP,atri,memdc);
      }
    
}


#define ID_TIMER_BLINK_CURSOR 1

/* esta funcion controla las propiedades del cursro*/

void csi_cursor_ctrl(const wchar_t *params, LineATributes *atri, HDC *memdc)
{
  /// HWND hWindow= WindowFromDC(GHdc); 
  wchar_t command[3]={params[1],params[2],L'\0'};
  wchar_t estate= params[3]; // estado=activo/desactivate
  if((wcscmp(command, L"12") == 0))
      {
	 static int timerCreated = 0;
	if (estate == L'h' )
	  {
	   
	    if (timerCreated == 0)
	      {
		cur.flag|=0x02;
		int result = SetTimer(Wwindow, ID_TIMER_BLINK_CURSOR, 500, NULL);
		
		if (result == 0)
		  {
		    printf("No se pudo crear el temporizador\n");
		  }
		else
		  {
		    timerCreated = 1; // Marcamos que el temporizador fue creado correctamente
		  }
	      }
	    else
	      {
		//if(cur.flag & 0x02)cur.flag&=~0x02;
		//else cur.flag|=0x02;
		
	      }
	  }
	else
	  {
	   
	    if(timerCreated == 1)
	      {
		cur.flag&=~0x02;
	      }
	  }
      }
  else
    {
      if((wcscmp(command, L"25") == 0))
	{

	  static int showCursor=0;
	  if (estate == L'h')
	  {
	    // implementar
	     // muestra el cursor
	    if(showCursor == 0)
	      {
		 cur.flag|=0x01;
		 showCursor=1;
	      } 
	  }
	else
	  {
	    // implemetar
	    // oculta el cursor
	    if(showCursor == 1)
	      {
		 cur.flag&=~0x01;
		 showCursor=0;
	      }
	   
	    
	  }
	  
	}
      
    }

  // printf("flag cursor %d\n",cur.flag);
}




#define NUMBER_FUNCION 19
void vt_process_funcion(wchar_t funcion, wchar_t* params, FUNCIONVT100 * arrf, LineATributes *atri, HDC *memdc)
{
  if((funcion == L'h') || (funcion == L'i'))
    {
      params[3]=funcion;
      params[4]=L'\0';
      funcion= L'?';
    }
  
    for (int x = 0; x < NUMBER_FUNCION; x++)
    {
        if (arrf[x].charFuncion == funcion)
        {
            // Ahora la función en el array también debe recibir un wchar_t* en lugar de un char*
	  void (*func)(const wchar_t *paramas, LineATributes *atri,HDC *memdc) = arrf[x].funcion;
            
            // Ejecutamos la función pasándole el parámetro `params`, que es ahora un `wchar_t*`
	  func(params, atri,memdc);
        }
    }
}


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


LineATributes vt_process_escape_sequences(Line *line, HDC *hdc)
{
    LineATributes lineAtri = {0};
    
    if ((line != NULL) && (line->ptrEscape != NULL))
    {
        wchar_t *cBuffer = (wchar_t *)line->ptrEscape;  // Usamos wchar_t en lugar de char
        int iterBuffer = 0;
        
        while (cBuffer[iterBuffer] != L'\0')
        {
            if ((cBuffer[iterBuffer] == 0x1b) && cBuffer[iterBuffer + 1] == L'[')
            {
                // Secuencias de escape CSI
                iterBuffer += 2;  // Avanzamos más allá de '\x1b' y '['
                
                wchar_t params[100];
                int lenParams = 0;
                
                while ((cBuffer[iterBuffer + lenParams] != L'\0' &&
                        cBuffer[iterBuffer + lenParams] != 0x1b) &&
                       (!iswalpha(cBuffer[iterBuffer + lenParams])))
                {
                    lenParams++;
                }
                
                wcsncpy(params, &cBuffer[iterBuffer], lenParams);  // Usamos wcsncpy para copiar la cadena
                params[lenParams] = L'\0';  // Aseguramos que la cadena esté terminada
                
                wchar_t funcion = cBuffer[iterBuffer + lenParams];  // Función asociada con la secuencia de escap                
                // Llamada a la función que procesa la secuencia
                vt_process_funcion(funcion, (wchar_t *)&params, arrFuncionVT100, &lineAtri, hdc);
                
                iterBuffer += (lenParams > 0) ? (lenParams + 1) : 1;  // Avanzamos en el buffer
            }
            else
            {
                if (cBuffer[iterBuffer] == 0x1b)
                {
                    if (iswprint(cBuffer[iterBuffer + 1]))
                    {
                        iterBuffer++;
                        wchar_t funcion = cBuffer[iterBuffer];
                        
                        // Procesamos la función con el caracter correspondiente
                        vt_process_funcion(funcion, &funcion, arrFuncionVT100, &lineAtri, hdc);
                        
                        iterBuffer++;  // Avanzamos a la siguiente secuencia
                    }
                }
                else
                {
                    // Secuencias de escape simples
                    wchar_t funcion = L'z';  // Asumimos un valor por defecto para el ejemplo
                    wchar_t params = cBuffer[iterBuffer];  // Guardamos el carácter de la secuencia
                    vt_process_funcion(funcion, &params, arrFuncionVT100, &lineAtri, hdc);
                    iterBuffer++;  // Avanzamos a la siguiente secuencia
                }
            }
        }
    }
    else
    {
        // No hay línea que procesar
    }
    
    return lineAtri;
}



void vt_reset_cursor()
{
  cursor=(POINT){.x=0,.y=0};
  oldCursor=(POINT){.x=0,.y=0};
}





