#ifndef KEY_BOARD_H
#define KEY_BOARD_H
#include <windows.h>
#define MAX_ESCAPE 10
typedef struct
{
  int vk; // codigo de telca virtual
  int processed;
  int ordinal; // indica en que orden llegan los eventos
  int isDown;  // indica si la telca fue presionada
  int changed; // indica si ubo un cambio
  int previous;  // estado anterio del la tecla
  int actual; // estado actual del teclado
  wchar_t ecape[MAX_ESCAPE];// para guardar la secuencia de escape
  wchar_t unicode; // caracter unicode de la tecla que se presiono
  double pressedTime; // momento en que se presiona la tecla
  double releaseTime; // momento en que se liberó la tecla
}Button;

enum {
    BUTTON_CTRL          = 0x11,  // VK_CTRL
    BUTTON_LEFT          = 0x25,  // VK_LEFT
    BUTTON_RIGHT         = 0x27, // VK_RIGHT
    BUTTON_UP            = 0x26,    // VK_UP
    BUTTON_DOWN          = 0x28,  // VK_DOWN
    BUTTON_W             = 0x57,     // 'W'
    BUTTON_A             = 0x41,     // 'A'
    BUTTON_S             = 0x53,     // 'S'
    BUTTON_D             = 0x44,     // 'D'
    BUTTON_BS            = 0x20,
    BUTTON_SHIFT         = 0x10,
    BUTTON_OEM_1         = 0xBA,	
    BUTTON_OEM_PLUS      = 0xBB,	
    BUTTON_OEM_COMMA     = 0xBC,	
    BUTTON_OEM_MINUS     = 0xBD,	
    BUTTON_OEM_PERIOD    = 0xBE,	
    BUTTON_OEM_2         = 0xBF,	
    BUTTON_OEM_3         = 0xC0,	
    BUTTON_OEM_4         = 0xDB,	
    BUTTON_OEM_5         = 0xDC,	
    BUTTON_OEM_6         = 0xDD,	
    BUTTON_OEM_7         = 0xDE,	
    BUTTON_OEM_8         = 0xDF,	
    BUTTON_OEM_102       = 0xE2,
    BUTTON_RETURN        = 0x0D,
    BUTTON_BACK          = 0x8,
    BUTTON_COUNT =256
};

typedef struct
{
  Button buttons[BUTTON_COUNT]; // array de botones
  Button orderButtons[BUTTON_COUNT]; // orden de botones segun llegada
  int countEvent; // cuenta el numero de eventos que ocurrio en el actual  fotograma
  char keystate[BUTTON_COUNT]; //estados de las teclas
}KeyboardEvent;

int ispressedAnyKey();
void debugKeyboard(KeyboardEvent k);
int ispressedCtrlAt(Button b);
int ispressedAlphaAt(Button b);
int isdownAlphaAt(Button b);
int ispressedArrowKeyAt(Button b);
int isdownArrowKeyAt(Button b);
int releasedArrowKey(Button b);
int downAt(Button b, int button);
int downAnyKey();
int isdownCtrlAt(Button b);
int dowPrintAble(Button b);
int releasedPrintAble(Button b);
int ispressedPrintable(Button b);
int ispressedOemButton(Button b);

/*
  Nombre de la macro: haveInput()
  macro para saber si hay entradas de teclado 
 */
#define HAVE_INPUT(b) (((k->buttons[b].changed) && \
			(k->buttons[b].isDown))||\
		       ((!(k->buttons[b].changed))&&\
			(k->buttons[b].isDown))\
		       )

#define ISPRESSPRINTABLE(z)(					\
	     (							\
	(ispressedPrintable(k->orderButtons[x])\
	 ||							\
	 (ispressedOemButton(k->orderButtons[x])))	\
	||							\
	((dowPrintAble(k->orderButtons[x]))		\
	 &&							\
	 (!releasedPrintAble(k->orderButtons[x]))		\
	 )							\
								))

/*
  Nombre de la macro: haveCtrlButton 
  Macro para saver si si presiono una tecla de control-
  no importa si se presiono una sola ve o se mantubo presionada

 */
#define HAVE_CTRL_BUTTON(x)(\
			   ispressedCtrlAt(k->orderButtons[(x)])	\
			   ||						\
			   isdownCtrlAt(k->orderButtons[((x))]))
#define PRESSED(b) ( \
    (k->buttons[(b)].isDown) && \
    (k->buttons[(b)].changed) \
)

#define RELEASED(b) ( \
    !(k->buttons[(b)].isDown) && \
    (k->buttons[(b)].changed) \
)

#define DOWN(b) ( \
    (k->buttons[(b)].isDown) && \
    !(k->buttons[(b)].changed) \
)



#define ispressedArrowKey (\
			   k->buttons[BUTTON_LEFT]\
			   ||\
			   k->buttons[BUTTON_RIGHT]\
			   ||\
			   k->buttons[BUTTON_DOWN]\
			   ||\
			   k->buttons[BUTTON_UP])


				
#endif

