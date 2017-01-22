#define DEFAULTIP "127.0.0.1"
#define DEFAULTPORT 1216

int startSocket(int *sock, const char *host, unsigned short port);

int exchangeBasics(int sock, unsigned char *players,
    unsigned char *mines, unsigned char *id, char *name, struct data **pData);

int game(WINDOW *left, WINDOW *bottom, WINDOW *textBox, WINDOW *middle,
    int sock, unsigned char id, unsigned char players, unsigned char mines, struct data *pData, unsigned char field[16][16]);
