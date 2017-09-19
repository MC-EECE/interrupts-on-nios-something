# InterruptsNIOS
This template is starting point for creating a project based on your custom C code.
It will provide you a default project to which you can add your software files. To
add files to a project, manually copy the file into the application directory (e.g. 
using Windows Explorer), then right click on your application project and select 
refresh.

You can also add files to the project using the Nios II Software Build Tools for Eclipse import function. 
Select File -> Import. 
Expand General and select File System in the Import Window and click Next.
Identify the appropriate source and destination directories.
Check the files you want to add and click Finish.

EECE321F16_InterruptDemo (c) 2016 Brent Horine
This program demonstrates how to develop with interrupts and timer in the NIOS-II system. It is not necessarily
optimal code. See if you can improve it. Notice that some information is passed via context variable pointers 
while other is passed via global variables. Interrupt service routines are created and registered for the timer and 
the push buttons. The timer is designed to fire an interrupt once per second which is then used to increment a
counter. The counter value is displayed in either HEX or BCD on the HEX LED displays. 

BCD stands for binary coded decimal. It is a user friendly way of displaying numbers in decimal format for users 
that may not understand HEX. BCD also groups 4 binary bits for each displayed digit, but only ranges from 0 to 9. 
After the binary value reaches 1001b, it produces a carry to the next group and rolls over to 0000b instead of 1010b.

