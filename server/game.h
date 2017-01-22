struct data {
    //Stat: byte0 = connected, byte1 = named
    unsigned char stats;
    unsigned short score;
    unsigned char namelen;
    unsigned char toReceive;
    unsigned char toDiscard;
    unsigned char len;
    char name[17];
    char buffer[65];
};


void initialize(struct data *playerData, unsigned char field[16][16]);
