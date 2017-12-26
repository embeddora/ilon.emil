/*
 * Main source file for Ebeddora Image Loader (EmIL v0.99)
 *
 * (C) Copyright 2013-2014, Embeddora Tech. LLC, info@embeddora.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


/* i.LON-100 warm reset address */
#define RAM_RESET_ADDR	0xBFC00000

/* 'reset' address of U-BOOT */
#define JUMP_TO_ADDR 	0x80020490

/* 'kernel_entry' address */
//++#define JUMP_TO_ADDR 	0x8051dc00

/* U2's base address in non-cached memory region. Good for VxWorks (cached) and for Linux kernel regions */
#define UART2_ADDR      0xB1300000

/* Offset of Line Status Register */
#define UART_LSR	0x1C

/* Offset of Transmit buffer */
#define UART_TX		4

/* Transmit-hold-register is empty */
#define UART_LSR_THRE	0x20


typedef  void (*VPF) () ;

extern const volatile unsigned long * CodeArray __attribute__((section(".binarr")));;

extern unsigned long CodeLength __attribute__((section(".binval")));;

void TrampStart() __attribute__((section(".tramp_text")));


void memcp(volatile unsigned long * , volatile unsigned long * , int __n) ;
void serial0_putc (char);
void _SerialWriteHexOfDepth(int port, unsigned long n, unsigned long depth);
void _SerialWriteNumDecimal(int port, unsigned long n);
void _SerialPuts(int port, unsigned char * str2put);


void TrampStart()
{
VPF pFunc;
unsigned long iLen;

	_SerialPuts(0, "[EMIL START] First instruction address is <");
	_SerialWriteHexOfDepth(0, JUMP_TO_ADDR, 8);
	_SerialPuts(0, ">, now jumping over there [EMIL END] \n\n\n");
	pFunc = (VPF) ( JUMP_TO_ADDR /* kernel_entry */);
	pFunc();

	/* Under normal operating not reached. */
	_SerialPuts(0, "[EMIL ERROR] Nobody should see this message. Contact software vendor and report this problem. ");

	/* Roboot to prevent executing potentially hazardous things */
	__asm__ __volatile__("jr\t%0"::"r"(RAM_RESET_ADDR));

	/* definately not-reached point. If nevertheless IS reached then the compiler generates WRONG binary => a) Copiler is bad OR b) on-board DRAM hardware is spoiled somehow. */
	_SerialPuts(0, "[EMIL imageloader ERROR] Most likely current compiler is bad. Contact software vendor and report this problem. ");
}

/* Own memcpy() ; relacating 'unsigned long' memory cells witin volatile(cached) memory */
void memcp(volatile unsigned long * __dest, volatile unsigned long * __src, int __n)
{
	while (__n--)
		*__dest++ = * __src++;
		

}

/* Put single character into UART-2, which is CONSOLE on i.LON-100 */
void serial0_putc (const char c)
{
volatile unsigned long *uart_lsr;
volatile unsigned long *uart_tx;
int iCnt; unsigned long ulSome;

	uart_lsr = (volatile unsigned long*)(UART2_ADDR+UART_LSR);
	uart_tx = (volatile unsigned long*)(UART2_ADDR+UART_TX);

	/* Waiting to empty fifo */
	while((*uart_lsr&UART_LSR_THRE)==0);

	*uart_tx = (unsigned long)c;/* __asm__ __volatile__ (".set mips2; sync; .set mips0": : :"memory"); */

	iCnt = 0xFF;while (iCnt--) { ulSome =0x12345678;  } ;
}

/* write string representing a hexadecimal number 'n' into serial port 'port' */
void _SerialWriteHexOfDepth(int port, unsigned long n, unsigned long depth)
{
#if (1)
    /* if hexadecimal to be emitted stil has 4-bit tetrade */
    if (depth)
    {
	/* recursively emit everything but this 4-bit tetrade */
        _SerialWriteHexOfDepth(0, n >> 4, depth-1);

	/* now take only this 4-bit tetrade and store its value in 'n' */
        n &= 0x0000000F;
	
	/* if the value needs to be represented as hexadecimal digit 0xA, .., 0xF */
        if (n > 9)
	    /* make corresponding char - 'A', ..., 'F' */
            n += 'A' - '0' - 10;
	/* make corresponding char '0' - '9', otherwise */
        n += '0';
	
	/* and put this char into serial port 'port' */
	serial0_putc( (unsigned char)n );
    }
#endif /* (0) */
}


/* write string representing an adecimal number 'n' into serial port 'port' */
void _SerialWriteNumDecimal(int port, unsigned long n)
{
#if (1)
    /* if decimal to be emitted stil not representable by single lowest digit */ 
    if (n > 9)
    {
	/* then recursively emit everything but its lowest digit */
        _SerialWriteNumDecimal(port, n / 10);
	
	/* now take only this lowest digit */
        n %= 10;
    }
    
    /* make char of this digit, and put this char into serial port 'port' */
    serial0_putc( (unsigned char)(n + '0') );   
#endif /* (0) */
}

/* write string pointed by str2put into serial port 'port' */
void _SerialPuts(int port, unsigned char * str2put)
{
/* temporary variable to store strings's contents */
char c;
#if (1)
	while ( ( c = *str2put++ ) != '\0' )
	
	    /* if NLC character is to be emitted */
	    if ( c == '\n' )
	    {
	    	/* emit newline character into serial port 'port' */
		serial0_putc(13);
		
		/* emit cariagereturn character into serial port 'port' */
		serial0_putc(10);
		
	    }
	    /* otherwise */
	    else	
		/* simply emit current character */
		serial0_putc(c);

#endif /* (0) */
}