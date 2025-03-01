#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BLOCK_SIZE      (4 * 1024)
#define PERIPHERAL_BASE 0x20000000      // PI0
//#define PERIPHERAL_BASE       0x7E000000  // PI2
#define GPIO_BASE       PERIPHERAL_BASE + 0x00200000


char FontBuf[64*1024];
typedef struct {
        unsigned long           gpio_base;
        int                     memory_fd;
        void                    *map;
        volatile unsigned int   *addr;
} rpi_gpio;

typedef struct {
        char Identifier[6];
        char FontName[8];
        unsigned char XSize;
        unsigned char YSize;
        unsigned char CodeType;
        unsigned char Tnum;     // number of MojiCode Entory
        struct {
                unsigned short Start;
                unsigned short End;
        } Block[];
} font_head;


font_head *pFH;

int GX[8] = {  2,  3,  4, 17, 18, 27, 22, 23 };
int GY[8] = { 24, 25, 12,  5,  6, 13, 19,  0 };

/*
 * Functions
 */
int FontInit()
{
        char Tmp[80];
        FILE *fp;

        if ( (fp = fopen ("MISAKI.FNT","rb")) == NULL ){
                return -1;
        }
        fread(FontBuf,sizeof(FontBuf), 1, fp);
        fclose(fp);
        pFH = (font_head *)FontBuf;
        return 0;
}

int SjisToImg(char *sjis, char *binary)
{
        int i=0,j=0,k=0,l=0,m=0;
        int font_base,code,offset;

        font_base = 18+4*pFH->Tnum;

        do {
                /* KANJI code offset Serch */
                code = (int)sjis[i] << 8 | (int)sjis[i+1];
                offset = font_base;
                for(j=0; j<pFH->Tnum; j++){
                        if(pFH->Block[j].Start <= code &&
                           pFH->Block[j].End >= code){
                                offset += (code - pFH->Block[j].Start) * 8;
                                break;
                        }
                        offset += (pFH->Block[j].End - pFH->Block[j].Start + 1) * 8;
                }
                if (i == pFH->Tnum){
                        printf("Out of scope!\n");
                        return -1;
                }

                /* Binary Image Create */
                for(k=0; k<8; k++){
                        for(m=0; m<8; m++){
                                if( FontBuf[offset+m] & (1<<k) ){
                                        binary[l+8-k] |= (1<<m);
                                }
                        }
                }
                i += 2;
                l += 8;
        } while (sjis[i] != 0);

        return 0;
}

int MapGPIO(rpi_gpio *gpio)
{
        gpio->memory_fd = open("/dev/mem", O_RDWR|O_SYNC );
        if(gpio->memory_fd < 0) {
                perror("Failed to open /dev/mem\n");
                return -1;
        }
        gpio->map = mmap( NULL, BLOCK_SIZE, PROT_READ|PROT_WRITE,
                        MAP_SHARED, gpio->memory_fd, gpio->gpio_base );
        if(gpio->map == MAP_FAILED){
                perror("cannot mmap\n");
                return -2;
        }
        gpio->addr = (volatile unsigned int *)gpio->map;
        return 0;
}

void UnmapGPIO(rpi_gpio *gpio)
{
        munmap(gpio->map, BLOCK_SIZE);
        close(gpio->memory_fd);
}

void InitGPIO(rpi_gpio *gpio,unsigned long GpioNo, unsigned long data)
{
        unsigned long gpio_addr,gpio_write_data,bit_shift;
        gpio_addr = (GpioNo/10)*4 + (unsigned long)gpio->addr;
        bit_shift = (GpioNo%10)*3;
        gpio_write_data = *((unsigned long *)gpio_addr);
        gpio_write_data &= ~(0x7UL << bit_shift);
        gpio_write_data |= data << bit_shift;
        *(unsigned long *)gpio_addr = gpio_write_data;
}
void OutOnGPIO(rpi_gpio *gpio,unsigned long GpioNo)
{
        *(gpio->addr + 7) = 1 << GpioNo;
}

void OutOffGPIO(rpi_gpio *gpio,unsigned long GpioNo)
{
        *(gpio->addr + 10) = 1 << GpioNo;
}


void write_binary(rpi_gpio *p_gpio,char *p_dat,int offset)
{
        int y;
        int px,py,i,k;
        px = 7;
        py = 0;
        for (i=0; i<8; i++){
                OutOnGPIO(p_gpio,GX[px]);
                k=0;
                for(y=7; y>=0; y--){
                        if(p_dat[offset+i] & (1<<y)){
                                OutOnGPIO(p_gpio,GY[py]);
                                k++;
                        } else {
                                OutOffGPIO(p_gpio,GY[py]);
                        }
                        py++;
                }
                usleep(200+200*k);
                OutOffGPIO(p_gpio,GX[px]);
                py = 0;
                px--;
        }
}

/*
 * Main routine
 */
int main(int argc, char **argv)
{
        rpi_gpio gpio = {GPIO_BASE};
        int i,j;
        int x,y;
        FILE *fp;
        char sjis_buf[512];
        char disp_buf[512*4+8+8];

        if ( argc != 2){
                printf("usage : sudo %s sjis_file \n",argv[0] );
                return 0;
        }

        i = MapGPIO(&gpio);
        if(i){
                printf("Error End\n");
                return i;
        }
        for (i=0; i<29;i++){
                InitGPIO(&gpio,i,1);
                OutOffGPIO(&gpio,i);
        }
        if( FontInit() <0 ){
                return -1;
        }
        for (i=0; i<29;i++){
                OutOffGPIO(&gpio,i);
        }
        /*
         * Load SJIS strings
         */
        if ((fp = fopen(argv[1], "rb") ) < 0 ){
                printf("cannot open %s \n",argv[1] );
                return -1;
        }
        if (fread(sjis_buf, 1, sizeof(sjis_buf), fp) <= 0 ){
                printf("cannot read file\n");
                fclose(fp);
                return -1;
        }
        fclose(fp);

        /*
         * convert sjis string  to  image data
         */
        memset(disp_buf,0,sizeof(disp_buf));
        if(SjisToImg(sjis_buf, disp_buf+8) < 0){
                printf("cannot convert SJIS code\n");
                return -1;
        }

        /*
         * Display to LED Matrix
         */
        j = (strlen(sjis_buf)+2 )*8/2;  /* +2 = pre space + after space */
        while(1){
                for(x=0; x<j; x++){
                        for (i=0; i<7; i++) write_binary(&gpio, disp_buf, x);
                }
        }
        return 0;
}