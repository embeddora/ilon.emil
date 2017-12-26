/*
 * Linker script for Ebeddora Image Loader (EMIL v0.99)
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


OUTPUT_FORMAT("elf32-bigmips", "elf32-bigmips", "elf32-littlemips")
OUTPUT_ARCH(mips)
ENTRY(TrampStart)
SECTIONS
{
 . = 0x80010000;
 .tramp_text : {    *(.tramp_text*)  }

 . = 0x80010D00-0x200;
 .data : { emil.o(.data*); }

 . = 0x80010D00-0x10;
 .binval  : { CodeArray.o(.sdata) } 


/* . = 0x80010D00; */
 . = 0x80020000; 
 .binarr  : { CodeArray.o(.rodata) }

	. = 0x80010500;
	.text : {    *(.text*)  }

	. = ALIGN(4);
	.tramp_fn :	{	*(.tramp_fn*) 	}

	. = ALIGN(4);
	.rodata  : { *(.rodata) }

	. = ALIGN(4);
	.data  : { *(.data) }

	. = ALIGN(4);
	.sdata  : { *(.sdata) }

	. = ALIGN(4);
	.sbss (NOLOAD)  : { *(.sbss) }

	.bss (NOLOAD)  : { *(.bss) }

	
        tramp_end = .;
 

}
