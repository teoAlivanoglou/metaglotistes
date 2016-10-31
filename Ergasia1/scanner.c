/* Onoma arxeiou:	scanner-C-skel.c
   Perigrafh:		Skeletos gia lektiko analyth grammeno se glwssa programmatismou C
   Syggrafeas:		Ergasthrio Metaglwttistwn, Tmhma Mhx.Plhroforikhs TE, TEI Athhnas
   Sxolia:		To paron arxeio parexei th basikh domh gia thn anaptyksh enos
                        lektikou analyth. Sthn parousa tou morfh _DEN_ kanei compile kai
                        apaitei diorthwseis kai arketes prosthhkes gia na leitourghsei swsta!
 */

#include <stdio.h>

enum State {
    S0, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, S12, FOUND, GOOD, BAD, INT, FLOAT, IMAGINARY
};
typedef enum State State;

char ch;
FILE *fp;
FILE *fo;
int counter;

void avoidblanks();
void avoidchars();
void token1();
void token2();
void token3();
void token4();

// H synarthsh typwnei keimeno pou dieykolynei thn aposfalmatwsh - katanohsh
// tou kwdika. An mpei sxolio prin to printf, apenergopoiountai ola ta
// mhnymata tou diaxeiristh lathwn.

void dbg(char* prn) {
    printf("%s", prn);
}

// H synarthsh main - Arxiko shmeio ekteleshs tou programmatos.

int main(int argc, char** argv) {
    fp = fopen("input.txt", "r");
    fo = fopen("output.txt", "w");

    if (fp == NULL) return 1;
    ch = fgetc(fp);
    while (ch != EOF) {
        counter = 0;
        avoidblanks();
        /* Anagnwrish xarakthra. Sto shmeio auto tha prepei na apofasistei to eidos
           tou anagnwristikou (px.onoma metablhths,akeraios arithmos,klp) pou tha
           mporouse na arxizei apo ton xarakthra ch pou diabasthke kai akolouthws na
           klhthei h synarthsh pou sarwnei xarakthra-xarakthra thn phgaia symvoloseira
           kai pragmatopoiei thn anagnwrish */
        if (ch >= '0' && ch <= '9' || ch == '.') {
            token1();
            continue;
        } else if (ch == '#') {
            token2();
            continue;
        } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
            token3();
            continue;
        } else if (ch == '\'' || ch == '\"') {
            token4();
            continue;
        } else {
            // O xarakthras den mporese na tairiaksei kapou -> mh apodektos
            fprintf(fo, "+Error! Can't recognize character!\n");
            avoidchars();
        }
        ch = fgetc(fp);
    }
    fclose(fp);
    fclose(fo);
    return 0;
}

// H synarthsh avoidblanks diabazei thn eisodo xarakthra-xarakthra
// prospernwntas ta kena, tis nees grammes (\n) kai ta tabs (\t)
// mexri na brei kapoion 'kanoniko' xarakthra, opote kai epistrefei.

void avoidblanks() {
    while (ch != EOF) {
        if ((ch != ' ') && (ch != '\n') && (ch != '\t')) {
            break;
        }
        ch = fgetc(fp);
    }
}

// H synarthsh avoidchars diabazei thn eisodo xarakthra-xarakthra
// prospernwntas olous tous 'kanonikous' xarakthres mexri na synanthsei
// kapoio keno h nea grammh (\n) h tab (\t), opote kai epistrefei. Me
// ton tropo auto to programma prospernaei agnwstes seires xarakthrwn.
// Xrisimi episis otan h diadikasia anagnorisis mias symboloseiras apotyxei
// kai prepei na oloklirothei h anagnwsi twn xarakthrvn ths

void avoidchars() {
    dbg("Found unrecognizable characters!\n");
    if (ch != ' ' && ch != '\n' && ch != '\t') {
        while (ch != EOF) {
            if (ch != ' ' && ch != '\n' && ch != '\t')
                ch = fgetc(fp);
            else
                break;
        }
    }
}

// Deigma synarthshs pou anagnwrizei kapoio sygkekrimeno anagnwristiko

void token1() {
    dbg("Trying to indentify token1\n");
    State currentState = S0;
    State prevState;
    while (1) {
        switch (currentState) {
            case S0:
            {
                dbg("  S0");
                if (ch == '.') {
                    currentState = S8;
                    prevState = S0;
                    break;
                } else if (ch == '0') {
                    currentState = S1;
                    prevState = S0;
                    break;
                } else if (ch >= '1' && ch <= '9') {
                    currentState = S4;
                    prevState = S0;
                    break;
                }// loipoi xarakthres
                else {
                    currentState = BAD;
                    prevState = S0;
                    break;
                }
            }
            case S1:
            {
                dbg("  S1");
                if (ch == '0') {
                    currentState = S1;
                    prevState = S1;
                    break;
                } else if (ch >= '1' && ch <= '9') {
                    currentState = S5;
                    prevState = S1;
                    break;
                } else if (ch == '.') {
                    currentState = S6;
                    prevState = S6;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = INT;
                    prevState = S1;
                    break;
                } else if (ch == 'b' || ch == 'B') {
                    currentState = S2;
                    prevState = S1;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S1;
                    break;
                }
            }
            case S2:
            {
                dbg("  S2");
                if (ch == '0' || ch == '1') {
                    currentState = S3;
                    prevState = S2;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S2;
                    break;
                }
            }
            case S3:
            {
                dbg("  S3");
                if (ch == '0' || ch == '1') {
                    currentState = S3;
                    prevState = S3;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = INT;
                    prevState = S3;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S3;
                    break;
                }
            }
            case S4:
            {
                dbg("  S4");
                if (ch >= '0' && ch <= '9') {
                    currentState = S4;
                    prevState = S4;
                    break;
                } else if (ch == '.') {
                    currentState = S6;
                    prevState = S4;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = INT;
                    prevState = S4;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S4;
                    break;
                }
            }
            case S5:
            {
                dbg("  S5");
                if (ch >= '0' && ch <= '9') {
                    currentState = S5;
                    prevState = S5;
                    break;
                } else if (ch == '.') {
                    currentState = S6;
                    prevState = S5;
                    break;
                } else if (ch == 'J' || ch == 'j') {
                    currentState = S12;
                    prevState = S5;
                    break;
                } else if (ch == 'E' || ch == 'e') {
                    currentState = S9;
                    prevState = S5;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S5;
                    break;
                }
            }
            case S6:
            {
                dbg("  S6");
                if (ch >= '0' && ch <= '9') {
                    currentState = S7;
                    prevState = S6;
                    break;
                } else if (ch == 'J' || ch == 'j') {
                    currentState = S12;
                    prevState = S6;
                    break;
                } else if (ch == 'E' || ch == 'e') {
                    currentState = S9;
                    prevState = S6;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = FLOAT;
                    prevState = S6;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S6;
                    break;
                }
            }
            case S7:
            {
                dbg("  S7");
                if (ch >= '0' && ch <= '9') {
                    currentState = S7;
                    prevState = S7;
                    break;
                } else if (ch == 'J' || ch == 'j') {
                    currentState = S12;
                    prevState = S7;
                    break;
                } else if (ch == 'E' || ch == 'e') {
                    currentState = S9;
                    prevState = S7;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = FLOAT;
                    prevState = S7;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S7;
                    break;
                }
            }
            case S8:
            {
                dbg("  S8");
                if (ch >= '0' && ch <= '9') {
                    currentState = S7;
                    prevState = S8;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S8;
                    break;
                }
            }
            case S9:
            {
                dbg("  S9");
                if (ch >= '0' && ch <= '9') {
                    currentState = S11;
                    prevState = S9;
                    break;
                } else if (ch == '+' || ch == '-') {
                    currentState = S10;
                    prevState = S9;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S9;
                    break;
                }
            }
            case S10:
            {
                dbg("  S10");
                if (ch >= '0' && ch <= '9') {
                    currentState = S11;
                    prevState = S10;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S10;
                    break;
                }
            }
            case S11:
            {
                dbg("  S11");
                if (ch >= '0' && ch <= '9') {
                    currentState = S11;
                    prevState = S11;
                    break;
                } else if (ch == 'J' || ch == 'j') {
                    currentState = S12;
                    prevState = S11;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = FLOAT;
                    prevState = S11;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S11;
                    break;
                }
            }
            case S12:
            {
                dbg("  S12");
                if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = IMAGINARY;
                    prevState = S12;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S12;
                    break;
                }
            }
            case INT:
            {
                dbg("  FOUND  INTEGER\n");
                currentState = GOOD;
                prevState = INT;
                fprintf(fo, "Type: INTEGER\t Num of chars: %d \n", counter - 1);
                break;
            }
            case FLOAT:
            {
                dbg("  FOUND  FLOAT\n");
                currentState = GOOD;
                prevState = INT;
                fprintf(fo, "Type: FLOAT\t Num of chars: %d \n", counter - 1);
                break;
            }
            case IMAGINARY:
            {
                dbg("  FOUND  IMAGINARY\n");
                currentState = GOOD;
                prevState = INT;
                fprintf(fo, "Type: IMAGINARY\t Num of chars: %d \n", counter - 1);
                break;
            }
            case BAD:
            {
                dbg("  BAD\n");
                prevState = BAD;
                fprintf(fo, "+Error! Invalid syntax for token1\n");
                avoidchars();
                break;
            }
            case GOOD:
                break;
        }
        if ((currentState == GOOD) || (currentState == BAD) && (prevState == BAD))
            break;
        ch = fgetc(fp);
        counter++;
    }
}

void token2() {
    dbg("Trying to indentify token2\n");
    State currentState = S0;
    State prevState;
    while (1) {
        switch (currentState) {
            case S0:
            {
                dbg("  S0");
                if (ch != '\n') {
                    currentState = S0;
                    prevState = S0;
                    break;
                }// loipoi xarakthres
                else {
                    currentState = FOUND;
                    prevState = S0;
                    break;
                }
            }
            case FOUND:
            {
                dbg("  FOUND  COMMENT\n");
                currentState = GOOD;
                prevState = FOUND;
                fprintf(fo, "Type: COMMENT\t Num of chars: %d \n", counter - 1);
                break;
            }
            case BAD:
            {
                dbg("  BAD\n");
                prevState = BAD;
                fprintf(fo, "+Error! Invalid syntax for token2\n");
                avoidchars();
                break;
            }
            case GOOD:
                break;
        }
        if ((currentState == GOOD) || (currentState == BAD) && (prevState == BAD))
            break;
        ch = fgetc(fp);
        counter++;
    }
}

void token3() {
    dbg("Trying to indentify token3\n");
    State currentState = S0;
    State prevState;
    while (1) {
        switch (currentState) {
            case S0:
            {
                dbg("  S0");
                if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_') {
                    currentState = S0;
                    prevState = S0;
                    break;
                } else if (ch == ' ' || ch == '\n' || ch == '\t') {
                    currentState = FOUND;
                    prevState = S0;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S0;
                    break;
                }
            }
            case FOUND:
            {
                dbg("  FOUND  IDENTIFIER\n");
                currentState = GOOD;
                prevState = FOUND;
                fprintf(fo, "Type: IDENTIFIER\t Num of chars: %d \n", counter - 1);
                break;
            }
            case BAD:
            {
                dbg("  BAD\n");
                prevState = BAD;
                fprintf(fo, "+Error! Invalid syntax for token1\n");
                avoidchars();
                break;
            }
            case GOOD:
                break;
        }
        if ((currentState == GOOD) || (currentState == BAD) && (prevState == BAD))
            break;
        ch = fgetc(fp);
        counter++;
    }
}

void token4() {
    dbg("Trying to indentify token4\n");
    State currentState = S0;
    State prevState;
    while (1) {
        switch (currentState) {
            case S0:
            {
                dbg("  S0");
                if (ch == '\'') {
                    currentState = S1;
                    prevState = S0;
                    break;
                } else if (ch == '\"') {
                    currentState = S3;
                    prevState = S0;
                    break;
                } else {
                    currentState = BAD;
                    prevState = S0;
                    break;
                }
            }

            case S1:
            {
                dbg("  S1");
                if (ch == '\\') {
                    currentState = S2;
                    prevState = S1;
                    break;
                } else if (ch == '\'') {
                    currentState = FOUND;
                    prevState = S1;
                    break;
                } else if (ch == '\n') {
                    currentState = BAD;
                    prevState = S1;
                    break;
                } else {
                    currentState = S1;
                    prevState = S1;
                    break;
                }

            }
            case S2:
            {
                dbg("  S2");
                currentState = S1;
                prevState = S2;
                break;
            }

            case S3:
            {
                dbg("  S3");
                if (ch == '\\') {
                    currentState = S4;
                    prevState = S3;
                    break;
                } else if (ch == '\"') {
                    currentState = FOUND;
                    prevState = S3;
                    break;
                } else if (ch == '\n') {
                    currentState = BAD;
                    prevState = S3;
                    break;
                } else {
                    currentState = S3;
                    prevState = S3;
                    break;
                }

            }
            case S4:
            {
                dbg("  S4");
                currentState = S3;
                prevState = S4;
                break;
            }
            case FOUND:
            {
                dbg("  FOUND  STRING\n");
                currentState = GOOD;
                prevState = FOUND;
                fprintf(fo, "Type: STRING\t Num of chars: %d \n", counter - 1);
                break;
            }
            case BAD:
            {
                dbg("  BAD\n");
                prevState = BAD;
                fprintf(fo, "+Error! Invalid syntax for token1\n");
                avoidchars();
                break;
            }
            case GOOD:
                break;

        }
        if ((currentState == GOOD) || (currentState == BAD) && (prevState == BAD))
            break;
        ch = fgetc(fp);
        counter++;
    }
}