#include "exitCodes.h"
#include <stdio.h>

void error(int code) {
    switch (code) {
        case 0: 
            // Normal exit
            exit(0);
            break;
        case 10:
            // The client executable has received the wrong number of args
            fprintf(stderr, "Usage: client chatscript");
            break;
        case 11:
            // The bot executable has received the wrong number of args
            fprintf(stderr, "Usage: clientbot responsefile");
            break;
        case 20:
            // Some sort of communications error
            fprintf(stderr, "Communications error");
            break;
        case 30:
            // The client was kicked by another user or bot
            fprintf(stderr, "Kicked");
            break;
        exit(code/10);
    }
}
