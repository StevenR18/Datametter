

void processInput(KeyboardEvent *k, List * inp)
{
  ///TODO: COMPLETAR LA FUNCION

  for(int key=0; key < (BUTTON_COUNT-1); key++)
    {
      if(HAVE_INPUT(key))
	{
	  unsigned int len = wcslen(k->buttons[key].escape);
	  wchar_t * esc    = (wchar_t *)k->buttons[key].escape;
	  if(PRESSED(key))
	    {
	      // SI SOLO SE PRESIONA UAN SOLA VEZ
	      appendNodoToList(inp,esc,len);
	    }
	  if(DOWN(key))
	    {
	      // SI UNA TECLA SE MATIENE PRESIONADA
	      appendNodoToList(inp,esc,len);
	      
	    }
	  if(RELEASED(key))
	    {
	      // SI UNA TECLA SE SUELTA
	      
	    }
	 
	}
    }
}



