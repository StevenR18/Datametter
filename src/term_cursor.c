
switch (key) {
  case L'D':
    {
      // LEFT
      int ofx=0;
      vEmu.vCursor.x--;
      if(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))== ROW_MAIN)ofx=vEmu.lenDirectory;
      if((vEmu.vCursor.x+ofx+vEmu.ofsetX) < 0)
	{
	  vEmu.ofsetX =((vEmu.widthBufferInChar- vEmu.widthInChar));
	  if(ROW_TYPE(vEmu.arrRow,((vEmu.vCursor.y-1)+vEmu.ofsetYTop))== ROW_MAIN) ofx=vEmu.lenDirectory;
	  vEmu.vCursor.x= (vEmu.widthInChar-ofx-1);
	  vEmu.vCursor.y--;
	  break;
	}

      if((vEmu.vCursor.x+ofx+vEmu.ofsetX) < vEmu.ofsetX)
	{
	  vEmu.ofsetX--;
	  vEmu.vCursor.x=0;
	  break;
	}
      if((vEmu.vCursor.x+ofx+vEmu.ofsetX) < (ofx) ) 
	{
	  vEmu.vCursor.x++;
	  break;
	}   
  }break;
  case L'C':
    {
      /// cursor inicia en 0
      //RIGH
      int ofx=0;
      // en primeras aumentamos el cursor
      vEmu.vCursor.x++;
      if(ROW_TYPE(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))== ROW_MAIN)ofx+=vEmu.lenDirectory;
      //revisa si el vEmu.vCursor.x+vEmu.ofsetX sea menor que el len de la fila actual
      if(!((ROW_LEN(vEmu.arrRow,(vEmu.vCursor.y+vEmu.ofsetYTop))+1) > (vEmu.vCursor.x+vEmu.ofsetX+ofx)))
	{
	  // aqui el cursor no se puede mover y sale
	  vEmu.vCursor.x--;
	  break;
	}
      if(vEmu.vCursor.x+vEmu.ofsetX+ofx > (vEmu.widthInChar+vEmu.ofsetX-1))
	{
	  vEmu.vCursor.x--;
	  vEmu.ofsetX++;
	}
      
      if((vEmu.vCursor.x+vEmu.ofsetX+ofx)>(vEmu.widthBufferInChar-1))
	{
	  vEmu.ofsetX=0;
	  vEmu.vCursor.x=0;
	  vEmu.vCursor.y++;
	}
      
    } break;
  case L'A':
    {
      // ring buffer up
         
    }break;
    case L'B':
      {
	// ring buffer dow
	
      } break;
  }
