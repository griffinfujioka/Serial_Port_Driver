#VFD=/root/vmware/Other/fd
#VFD=/root/dosemu/mtximage
VFD=mtximage.bin

echo compiling t.c ...
bcc -c -ansi t.c
as86 -o ts.o ts.s
echo linking mtx ts.o t.o mtxlib
ld86 -o mtx -d ts.o t.o mtxlib /usr/lib/bcc/libc.a

mount -o loop mtximage.bin fd0 
cp mtx /fd0/boot
sleep 1
umount /mnt

#(cd USER; mkallu)

echo done

