case CTRL_KEY('q'):
	  {
	    exit_app();
	  }break;
	case CTRL_KEY('w'):
	  appendCharsRow(&row,L"^W",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break;
	case CTRL_KEY('e'):
	  appendCharsRow(&row,
			 L"^E",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('r'):
	  appendCharsRow(&row,
			 L"^R",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('t'):
	  appendCharsRow(&row,
			 L"^T",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('y'):
	  appendCharsRow(&row,
			 L"^Y",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break;
        case CTRL_KEY('u'):
	  appendCharsRow(&row,
			 L"^U",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('o'):
	  appendCharsRow(&row,
			 L"^O",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('p'):
	  appendCharsRow(&row,
			 L"^P",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('s'):
	  appendCharsRow(&row,
			 L"^S",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('d'):
	  appendCharsRow(&row,
			 L"^D",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('g'):
	  appendCharsRow(&row,
			 L"^G",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('k'):
	  appendCharsRow(&row,
			 L"^K",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('l'):
	  appendCharsRow(&row,
			 L"^L",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('z'):
	  appendCharsRow(&row,
			 L"^Z",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('x'):
	  appendCharsRow(&row,
			 L"^X",
			 cursor_x,
			 cursor_y,
			 ofset_y,
			 2,
			 NULL,
			 &caprow,
			 WIDTH_TERMV,
			 HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('b'):
	  appendCharsRow(&row,
			 L"^B",cursor_x,
			 cursor_y,ofset_y,
			 2,NULL,&caprow,
			 WIDTH_TERMV,HEIGHT_TERMV);
	  break; 
        case CTRL_KEY('n'):
	  appendCharsRow(&row,L"^N",
			 cursor_x,cursor_y,
			 ofset_y,2,
			 NULL,&caprow,
			 WIDTH_TERMV,HEIGHT_TERMV);
	  break;
