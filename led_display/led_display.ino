const int maxRows = 16;
const int maxCols = 16;

enum bufftype {
  INT16,
  INT8,
  BOOL
};

class matrixDisplay{
  private:
  int row;
  
  union {
    unsigned int idata;
    unsigned char cdata[2];
  } matbuf[16];
  
  int selectPin[4];
  int _clock, _data, _latch, _blank;
  
  public:
  
  void upbuf(int row, int input) {
    matbuf[row].idata = input;
  }
  void upbuf(int row, int col, int input) {
    matbuf[row].cdata[col] = input & 0xFF;
  }
  void upbuf(int row, int col, bool input) {
    if ( row-- == 0 ) row = 15;
    bool flag = getbuf(BOOL, row, col);
    int bpos = 1<<(15-col);
    if  (flag != input) {
      if (input) {
        matbuf[row].idata |= bpos;
      } else {
        matbuf[row].idata &= ~bpos;
      }
    }
  }
  void fillbuff(int input) {
    for (int i = 0; i < maxRows; i++) upbuf(i, (int) input);
  }
  void fillbuff(char input) {
    for (int i = 0; i < maxRows; i++) for (int j = 0; j < maxCols/8; j++) upbuf(i,j, (int) input);
  }
  void blankbuff() {
    fillbuff((int)0);
  }
  int getbuf(enum bufftype gettype, int row, int col = 0) {
    switch (gettype) {
      case INT16:
        return matbuf[row].idata;
        break;
      case INT8:
        return matbuf[row].cdata[col] & 0xff;
        break;
      case BOOL:
        return matbuf[row].idata & 1<<(15-col);
        break;
      default:
        return 0xB00B;
    }
  }
  void updateDisplay() {
    digitalWrite(_blank, HIGH);
    digitalWrite(_latch, HIGH);
    for (int i = 0; i<4; i++) {
      digitalWrite(selectPin[i], (row>>i)&1);
    }

    for (int j = 0; j < maxCols; j++) {
      digitalWrite(_data, (~matbuf[row].idata>>j)&1);
      digitalWrite(_clock, HIGH);
      digitalWrite(_clock, LOW);
    }
    digitalWrite(_latch, LOW);
    digitalWrite(_blank, LOW);

    if (++row == maxRows) {
      row = 0;
    }
  }
  
  matrixDisplay(int A, int B, int C, int D, int DI, int CLK, int LAT, int G) {
    selectPin[0] = A;
    pinMode(A, OUTPUT);
    digitalWrite(A, LOW);
    selectPin[1] = B;
    pinMode(B, OUTPUT);
    digitalWrite(B, LOW);
    selectPin[2] = C;
    pinMode(C, OUTPUT);
    digitalWrite(C, LOW);
    selectPin[3] = D;
    pinMode(D, OUTPUT);
    digitalWrite(D, LOW);
    _clock = CLK;
    pinMode(CLK, OUTPUT);
    digitalWrite(CLK, LOW);
    _data = DI;
    pinMode(DI, OUTPUT);
    digitalWrite(DI, LOW);
    _latch = LAT;
    pinMode(LAT, OUTPUT);
    digitalWrite(LAT, LOW);
    _blank = G;
    pinMode(G, OUTPUT);
    digitalWrite(G, HIGH);
    row = 0;
  }
};

matrixDisplay md0( 4, 5, 6, 7, 8, 9, 10, 11);

int n, m, nf, mf;
int framecounter = 0;
unsigned char counter;

                                     //0123|456789ABCDEF|
         int gol[16] = { 0x64f7,     // .. | .  |....| ...|
                         0x8Af5,     //.   |. . |....| . .|
                         0xAE96,     //. . |... |.  .| .. |
                         0x6A97,     // .. |. . |.  .| ...|
                         
                         0x0000,     //    |    |    |    |
                         0x07e0,     //    | .  |... |    |
                         0x0A80,     //    |. . |.   |    |
                         0x0AC0,     //    |. . |..  |    |
                         
                         0x0780,     //    | .  |.   |    |
                         0x0000,     //    |    |    |    |
                         0x22ee,     //  . |  . |... |... |
                         0x228A,     //  . |  . |.   |. . |
                         
                         0x22cc,     //  . |  . |..  |..  |
                         0x3a8e,     //  ..|. . |.   |... |
                         0x0000,     //    |    |    |    |
                         0x201e };   //  . |    |   .|... |

int boundRows(int n) {
  while (n < 0) n +=maxRows;
  while (n >= maxRows) n-= maxRows;
  return n;
}
int boundCols(int n) {
  while (n < 0) n +=maxCols;
  while (n >= maxCols) n-= maxCols;
  return n;
}

int getgol(const int row, const int col) {
  int r = boundRows(row);
  int c = boundCols(col);
  return !!(gol[r] & (1 << ( maxCols - (1+c) )));
}

int progits = 2;
int maxits = 0;
int golits = maxits;

void iterateGol() {
  if ( golits >= 0 ) {
    for ( int n = 0; n < maxRows * maxCols; n++ ) {
      int row = n / maxRows;
      int col = n % maxCols;
      int sum = getgol(row+1,col) 
              + getgol(row+1,col+1) 
              + getgol(row,col+1)
              + getgol(row-1,col+1)
              + getgol(row-1,col)
              + getgol(row-1,col-1)
              + getgol(row,col-1)
              + getgol(row+1,col-1);
      bool cellstatus = getgol(row,col)?              // is the cell currently alive?
                            (sum >= 2 && sum <= 3 )   // ..when alive
                           :sum==3;                   // ..when dead
    
    //char buf[90];
    //sprintf(buf, "row %2d col %2d = %2d [%5s -> %5s]", row, col, sum, md0.getbuf(BOOL, row, col)?"alive":"dead",cellstatus?"alive":"dead");
    //Serial.println(buf);

    //Serial.print(cellstatus);
    //if (col==maxCols-1) {
    //  Serial.println();
    //}
      md0.upbuf(row+1,col,(bool) cellstatus);
    }
    for (int m = 0; m < maxRows; m++ ) gol[m] = md0.getbuf(INT16, m);
  }

  if (golits++ == maxits) {
    golits = -10;
    for (int n = 0; n < maxRows; n++) gol[n] = 0;
    progits++;
    //Serial.println(progits);
    top_iteration:
    if (progits == 1 ) {
      gol[5] = 0x0180, 
      gol[6] = 0x0300,
      gol[7] = 0x0100;
      maxits = 80;
    } else if (progits == 2) {
      gol[5] = 0x0902,
      gol[6] = 0x0081,
      gol[7] = 0x0887,
      gol[8] = 0x0780;
      maxits = 110;
    } else if (progits == 3) {
      gol[3] = 0x0E00,
      gol[4] = 0x0020,
      gol[5] = 0x0010,
      gol[6] = 0x0070;
      maxits = 140;
    } else {
      progits = 1;
      goto top_iteration;
    }
    for (int n = 0; n < maxRows; n++) md0.upbuf(n,gol[n]);
  }
}


void setup() {
  Serial.begin(19200);
  pinMode(A0, INPUT);
  m = n = mf = nf = 0;
  counter = 200;

  OCR0A = 0x04;
  TIMSK0 |= _BV(OCIE0A);
  
  //md0.blankbuff();
  for (int n = 0; n < maxRows; n++) md0.upbuf(n,(int)gol[n]);
  for (int n = 20; n--; delay(100)) Serial.println(n); 
  Serial.println("Gol starting..");
}

int updateQuarterFrames = 128;

SIGNAL(TIMER0_COMPA_vect) {
  /*
  if (framecounter++ >= updateQuarterFrames*4) {
    framecounter = 0;
    gameloop();
  }
  */
  
  md0.updateDisplay();
}

int edge = 2;

void gameloop() {
  /*
  if ( edge > 7 ) {
    nf = 0, mf = 1; //00: right, 01: down, 10: up, 11: left
    n = 8, m = 8, edge = 0;
    counter++; 
  }
   
  md0.upbuf(n,m, (bool)0);

  if ( nf && mf ) n--;
  if ( !nf && mf ) m--;
  if ( nf && !mf ) m++;
  if ( !nf && !mf ) n++;

  if ( n >= 15-edge ) nf = 1;
  if ( m >= 15-edge ) mf = 1;
  if ( n <= edge + 1 ) nf = 0;
  if ( m <= edge ) mf = 0; 
  if ( n == edge + 1 && m == edge ) edge++;
  
  md0.upbuf(n,m, (bool)1);

  md0.upbuf(15,0,counter);
  md0.upbuf(15,1,~counter);
  updateQuarterFrames = (edge+4)-edge/3;
  */
  iterateGol();
}

void loop(){
  gameloop();
  
  delay(100);
}
