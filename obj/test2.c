#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>

#define LEDMAX 3  /* Numof LEDs(4) -1 */

/* SPI data */
typedef struct {
  unsigned char adr;
  unsigned char dat;
} OneLine;
typedef struct  {
  OneLine Led[LEDMAX+1];
} _send_data;


/* FONTX Header */
typedef struct {
        char Identifier[6];
        char FontName[8];
        unsigned char XSize;
        unsigned char YSize;
        unsigned char CodeType;
        unsigned char Tnum;     // number of Block[]s
        struct {
                unsigned short Start;
                unsigned short End;
        } Block[];
} font_head;

/* FONT Buffer */
unsigned char ucFontBuf[0x10000];  /* 64KB */
font_head *pFH;         /* Buffer Pointer */

/*
 * Functions
 */


/* FONT File Read */
int FontInit()
{
        FILE *fp;

        if ( (fp = fopen ("MISAKI.FNT","rb")) == NULL ){
                return -1;
        }
        fread(ucFontBuf,sizeof(ucFontBuf), 1, fp);
        fclose(fp);
        pFH = (font_head *)ucFontBuf;
        return 0;
}

/* Function to write kanji(sjis) image to memory(binary)  */
int SjisToImg(char *sjis, char *binary)
{
    int i=0,j=0,k=0,l=0,m=0;
    int font_base,offset;
    unsigned short code;

    font_base = 18+4*pFH->Tnum;
    do {
        /* KANJI code offset Serch */
        code = (unsigned short)sjis[i] << 8 | (unsigned short)sjis[i+1];
        offset = font_base;
        for(j=0; j<pFH->Tnum; j++){
            if(pFH->Block[j].Start <= code &&
                pFH->Block[j].End >= code){
                offset += (int)(code - pFH->Block[j].Start) * 8;
                break;
            }
            offset += (int)(pFH->Block[j].End - pFH->Block[j].Start + 1) * 8;
        }
        /* Binary Image Create */
        if (j != pFH->Tnum){
            memcpy(binary+l, ucFontBuf+offset, 8);
        }
        i += 2;
        l += 8;
    } while (sjis[i] != 0);
    return 0;
}

/*
 * arguments:
 *    binary      : [in]Binary Data
 *    bit offset  : [in]start bit offset of the Binary Data
 *    data        : [out]Send Data binary
 */
void SendDataCreate(char *binary, int bit_offset, _send_data *data)
{
    int i=0,j=0,remind,offset;

    /* bit shift */
    for(i=0; i<8; i++){
        for(j=LEDMAX-1; j>=0; j--){
            data[i].Led[j].adr = i+1;
            data[i].Led[j].dat = data[i].Led[j].dat << 1;
            data[i].Led[j].dat |= (data[i].Led[j+1].dat >> 7);
        }
    }

    /* LED0 bit0 */
    remind = bit_offset%8;
    offset = (bit_offset - remind);
    for(i=0; i<8; i++){
        data[i].Led[LEDMAX].adr = i+1;
        data[i].Led[LEDMAX].dat = data[i].Led[LEDMAX].dat << 1;

        if ( binary[offset + i] & ( 0x80 >> remind ) ){
            data[i].Led[LEDMAX].dat |= 1;
        }
    }
}


/* Main Routine */
int main(int argc, char **argv)
{
    int fd;
    int i,j,iSize;
    static _send_data data[8];
    static unsigned char set_data[(LEDMAX+1)*2];
    FILE *fp;
    static char sjis_buf[512];
    static char disp_buf[512*8+8+8];

    /* Argument check */
    if ( argc != 2){
        printf("usage : sudo %s sjis_file \n",argv[0] );
        return 0;
    }

    /* Load SJIS strings */
    memset(sjis_buf,0,sizeof(sjis_buf));
    if ((fp = fopen(argv[1], "rb") ) < 0 ){
        printf("cannot open %s \n",argv[1] );
        return -1;
    }
    iSize = fread(sjis_buf, 1, sizeof(sjis_buf), fp);
    if (iSize < 0){
        printf("cannot read file\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);

    /* FONT load and Initialize */
    if( FontInit() <0 ){
        return -1;
    }

    /* Setup SPI libraly */
    wiringPiSetup();
    if((fd = wiringPiSPISetup(0,500000)) < 0){
        printf("SPI initialize error\n");
        return -1;
    }

    /* Initialize MAX7219 */
    memset((void *)data,0,sizeof(data));
    set_data[0] = 0x09, set_data[1] = 0x00; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x0a, set_data[1] = 0x04; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x0b, set_data[1] = 0x07; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x0c, set_data[1] = 0x01; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x0f, set_data[1] = 0x00; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x00, set_data[1] = 0x00; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x00, set_data[1] = 0x00; wiringPiSPIDataRW(0,set_data,2);
    set_data[0] = 0x00, set_data[1] = 0x00; wiringPiSPIDataRW(0,set_data,2);
    delay(100);

    memset(disp_buf,0,sizeof(disp_buf));
    SjisToImg (sjis_buf, disp_buf);

    /* Draw LED matrix */
    iSize = iSize * 4 + 8;
loop:
    for( i=0; i<iSize; i++){
        SendDataCreate(disp_buf, i, data);
        for(j=0; j<8; j++){
            memcpy(set_data,(unsigned char *)(&data[j]),sizeof(set_data));
            wiringPiSPIDataRW(0,set_data,sizeof(set_data));
        }
        delay(100);
    }
    goto loop;
}