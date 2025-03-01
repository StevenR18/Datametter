#define CTRL_DOWN (GetKeyState(VK_CONTROL) & 0x8000)
#define ALT_DOWN  (GetKeyState(VK_MENU) & 0x8000)
#define SHIFT_DOWN (GetKeyState(VK_SHIFT) & 0x8000)

void translate_keyevent_tovt_sequence(KeyboardEvent *k)
{
  ///TODO: COMPLETAR LA FUNCION

  for(int key=0; key < (BUTTON_COUNT-1); key++)
    {
      if(HAVE_INPUT(key))
	{
	  wchar_t alpha[2]={0};
	  GetKeyboardState(k->keystate);
	  int result= ToUnicode(k->buttons[key].vk,0,k->keystate,alpha,2,0);
	  k->buttons[key].unicode=alpha[0];

	  if (CTRL_DOWN && key == 'C')
            {
	      wcsncpy(k->buttons[key].escape, L"^C", MAX_ESCAPE);
            }
	  else if (ALT_DOWN && key == VK_UP)
            {
	      wcsncpy(k->buttons[key].escape, L"\x1b[1;3A", MAX_ESCAPE);
            }
	  else if (CTRL_DOWN && key == VK_DOWN)
            {
	      wcsncpy(k->buttons[key].escape, L"\x1b[1;5B", MAX_ESCAPE);
            }
            else
            {
	      k->buttons[key].escape[0] = L'\0'; // Limpia si no hay escape
            }
	}
    }
}



