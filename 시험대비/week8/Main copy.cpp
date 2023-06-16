
ifstream infStream;
ofstream outfStream;

char get_tetriskey(TetrisState state, bool fromUser, bool toFile) {
  char key;

  if (fromUser) {
    if(state == TetrisState::NewBlock)
      key = (char) ('0' + rand()%MAX_BLK_TYPES);
    else 
      key = getch()
  }
  else {
    if(ifstream.is_open()==false) {
      ifstream.open("keyseq.txt");
      if(ifstream.fail())
        exit(1)
      
      if (ifstream.eof())
        key='q'
      else 
        ifstream.get(key);
    }
  }

  if (toFile) {
    if(outfStream.is_open()==false) {
      outfStream.open("keyseq.txt");
      if(outfStream.fail())
        exit(1);
      
      outfStream << key;
    }
  }
}
