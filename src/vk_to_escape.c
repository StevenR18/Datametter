 else if (CTRL_DOWN && virtualKey == VK_DOWN)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;5B",MAX_ESCAPE);
   }
 else if (CTRL_DOWN && virtualKey == VK_UP)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;5A",MAX_ESCAPE);
   }
 else if (CTRL_DOWN && virtualKey == VK_RIGHT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;5C",MAX_ESCAPE);
   }
 else if (CTRL_DOWN && virtualKey == VK_LEFT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;5D",MAX_ESCAPE);
   }
 else if((CTRL_DOWN) && (virtualKey >= 0x41 && virtualKey <= 0x5A))
   {
     switch(virtualKey)
       {
       case L'A':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*A",MAX_ESCAPE);break;
       case L'B':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^B",MAX_ESCAPE);break;
       case L'C':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*C",MAX_ESCAPE);break;
       case L'D':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^D",MAX_ESCAPE);break;
       case L'E':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^E",MAX_ESCAPE);break;
       case L'F':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*F",MAX_ESCAPE);break;
       case L'G':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^G",MAX_ESCAPE);break;
       case L'H':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*H",MAX_ESCAPE);break;
       case L'I':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*I",MAX_ESCAPE);break;
       case L'J':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*J",MAX_ESCAPE);break;
       case L'K':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^K",MAX_ESCAPE);break;
       case L'L':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*L",MAX_ESCAPE);break;
       case L'M':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*M",MAX_ESCAPE);break;
       case L'N':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^N",MAX_ESCAPE);break;
       case L'O':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^O",MAX_ESCAPE);break;
       case L'P':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^P",MAX_ESCAPE);break;
       case L'Q':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^Q",MAX_ESCAPE);break;
       case L'R':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^R",MAX_ESCAPE);break;
       case L'S':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^S",MAX_ESCAPE);break;
       case L'T':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^T",MAX_ESCAPE);break;
       case L'U':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^U",MAX_ESCAPE);break;
       case L'V':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*V",MAX_ESCAPE);break;
       case L'W':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^W",MAX_ESCAPE);break;
       case L'X':wcsncpy(keyEvent.buttons[virtualKey].escape,L"*X",MAX_ESCAPE);break;
       case L'Y':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^Y",MAX_ESCAPE);break;
       case L'Z':wcsncpy(keyEvent.buttons[virtualKey].escape,L"^z",MAX_ESCAPE);break;	 
       }
     
   }
 else if (SHIFT_DOWN && virtualKey == VK_RIGHT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;2C",MAX_ESCAPE);
   }
 else if (SHIFT_DOWN && virtualKey == VK_LEFT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;2D",MAX_ESCAPE);
   }
 else if (SHIFT_DOWN && virtualKey == VK_UP)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;2A",MAX_ESCAPE);
   }
 else if (SHIFT_DOWN && virtualKey == VK_DOWN)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;2B",MAX_ESCAPE);
   }
 
  else if (ALT_DOWN && virtualKey == VK_UP)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;3A",MAX_ESCAPE);
   }
 else if (ALT_DOWN && virtualKey == VK_DOWN)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;3B",MAX_ESCAPE);
   }
 else if (ALT_DOWN && virtualKey == VK_RIGHT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;3C",MAX_ESCAPE);
   }
 else if (ALT_DOWN && virtualKey == VK_LEFT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;3D",MAX_ESCAPE);
   }
 else if (ALT_DOWN && virtualKey == VK_UP)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[1;3A",MAX_ESCAPE);
   }
 else if(virtualKey == VK_RETURN)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\n",MAX_ESCAPE);
   }
 else if(virtualKey == VK_BACK)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[3~",MAX_ESCAPE);
   }
 else if(virtualKey == VK_UP)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[A",MAX_ESCAPE);
   }
 else if(virtualKey == VK_DOWN)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[B",MAX_ESCAPE);
   }
 else if(virtualKey == VK_RIGHT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[C",MAX_ESCAPE);
   }
 else if(virtualKey == VK_LEFT)
   {
     wcsncpy(keyEvent.buttons[virtualKey].escape,
	     L"\x1b[D",MAX_ESCAPE);
   }

