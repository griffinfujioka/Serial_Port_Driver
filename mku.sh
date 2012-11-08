#VFD=/root/vmware/Other/fd
#VFD=/root/dosemu/mtximage
VFD=../mtximage

echo ------------------------ make $1 --------------------------
bcc -c -ansi $1.c
as86 -o u.o u.s
ld86 -o $1 u.o $1.o ../mtxlib /usr/lib/bcc/libc.a

mount -o loop $VFD /mnt
cp $1 /mnt/bin
umount /mnt

rm *.o $1

