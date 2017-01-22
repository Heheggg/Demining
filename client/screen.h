#define MINLINES 20
#define MINCOLS 80

int initializeScreen(WINDOW **left, WINDOW **middle, WINDOW **right,
    WINDOW **bottom, WINDOW **textBox);

void drawMiddle(WINDOW *middle, unsigned char field[16][16]);

void drawLeft(WINDOW *left, unsigned char players, unsigned char id, unsigned char mines, struct data *pData);

void drawRight(WINDOW *right);

int keyboardHandler(WINDOW *middle, WINDOW *textBox,
    unsigned char *chatLen, unsigned char *isChatting, unsigned char *y, unsigned char *x,
    unsigned char id, unsigned char turn, unsigned char field[16][16],
    char *chatBuffer);

void addLine(WINDOW *bottom, unsigned char msgLen, char *buffer,
    struct data *pData);

void addPlayer(WINDOW *left, WINDOW *bottom, unsigned char id,
    struct data *pData);

void removePlayer(WINDOW *left, WINDOW *bottom, unsigned char id,
    struct data *pData);

void tellTurn(WINDOW *bottom, unsigned char turn, struct data *pData);

void setValue(WINDOW *middle, unsigned char y, unsigned char x,
    unsigned char v, unsigned char t);

void updateScore(WINDOW *left, unsigned char t, struct data *pData);

void startCursor(WINDOW *middle, unsigned char *y, unsigned char *x, unsigned char field[16][16]);

void updateMines(WINDOW *left, unsigned char mines);

void showWinner(WINDOW *bottom, unsigned char players, struct data *pData);
