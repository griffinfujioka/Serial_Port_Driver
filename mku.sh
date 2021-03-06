#VFD=/root/vmware/Other/fd
#VFD=/root/dosemu/mtximage
#VFD=../mtximage

echo ------------------------ make $1 --------------------------
bcc -c -ansi $1.c
as86 -o u.o u.s
ld86 -o $1 u.o $1.o mtxlib /usr/lib/bcc/libc.a #link the files and call it $1 (first argument) 

mount -o loop mtximage.bin fd0
cp $1 fd0/$1
cp $1 fd0/bin/$1
sleep 1 	# Sleep for a second so fd0 will not be busy and is able to umount 
sudo umount fd0

rm *.o $1
echo $1 user mode image completed. 

