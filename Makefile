#
# Makefile for Ebeddora Image Loader (EmIL v0.99)
#
# (C) Copyright 2013-2014, Embeddora Tech. LLC, info@embeddora.com
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

CFLAGS		=  

ASMFLAGS	=

all: iLonSystem iLonSystem.odf

iLonSystem.odf:  iLonSystem
	mips-sdl-elf-objdump.exe    -D          --section-headers  	iLonSystem > iLonSystem.odf	
#obsolete	mips-sdl-elf-objdump.exe    -D -a -f -p -x -d -S -g         --section-headers  	emil > emil.odf	

vmlinux.odf:  vmlinux
	mips-sdl-elf-objdump.exe    -D         --section-headers  	vmlinux > vmlinux.odf	
#obsolete	mips-sdl-elf-objdump.exe    -D -S -g         --section-headers  	vmlinux > vmlinux.odf	

CodeArray.c: x386-elf-ca.exe
	./x386-elf-ca.exe >CodeArray.c 

#vmlinux.bin.odf:  vmlinux.bin
#	mips-sdl-elf-objdump.exe    -D -S -g         --section-headers  	vmlinux.bin > vmlinux.bin.odf	

emil.o: emil.c
	mips-sdl-elf-gcc.exe $(CFLAGS) -c emil.c -o emil.o 

CodeArray.o: CodeArray.c
	mips-sdl-elf-gcc.exe $(CFLAGS) -c CodeArray.c -o CodeArray.o 

iLonSystem: emil.o CodeArray.o
	mips-sdl-elf-ld.exe  emil.o CodeArray.o -o 	iLonSystem  -Temil.ld   

x386-elf-ca.exe: x386-elf-ca.c
	gcc x386-elf-ca.c -o x386-elf-ca.exe



clean:
	rm -f *.[oa]  *.map        emil iLonSystem CodeArray.* *.odf    


