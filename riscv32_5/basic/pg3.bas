10 print "Interpreter Basic , Virtual Machine";
20 println;
30 serial=0;
40 i=0;
50 j=0;
60 k=0;
70 print "ingrese un digito: ";
80 input serial;
90 println;
100 i=serial%2;
110 if i=1 then gosub 500;
120 if i=0 then gosub 550;
130 for j=0 to 20
140 print "*";
150 println;
160 next;
170 delay_ms 100;
180 delay_us 100;
190 pinmode 13,1;
200 dwrite 13,1;
210 awrite 8,200;
220 dread 0,k;
230 aread 1,k;
240 goto 70;
500 print "impar";
510 println;
520 return;
550 print "par";
560 println;
570 return;