import math
import sys

file1 = open('mes3_3_200_3.txt', 'r')
file2 = open('null.txt','w')
Lines = file1.readlines()
l = []
for i,line in enumerate(Lines):
    print("############################################################")
    print(i)
    print("############################################################")
    file2.write("############################################################\n")
    file2.write(str(i)+"\n")
    file2.write("############################################################\n")
    val = float(line)
    V = 0
    for i in range(2**12+1):
        prew = ((i-1)*(3.3/((2**12)-1)))
        now = i*(3.3/((2**12)-1))
        if now>val:
            if now-val<val-prew:
                print(i)
                l.append(i)
                # print(now)
                # print(val)
                # print("d-=",now-val)
                # print("dd=",val-prew)
                V = now
            if now-val>val-prew:
                print(i-1)
                l.append(i-1)
                # print(prew)
                # print(val-1)
                # print("d-=",val-prew)
                # print("dd=",now-val)
                V = prew
            if now-val==val-prew:
                print(i)
                l.append(i)
                # print(now,prew)
                # print(val)
                # print("d-=",now-val)
                # print("dd=",val-prew)
                V = now
            break
    if len(sys.argv)>1:
        if len(sys.argv)>2 and int(sys.argv[2])==0:
            V = val

        if int(sys.argv[1])==1:
            Vcc = 3.3
            R1 = 200.0
            R2 = 0
            I = (Vcc-V)/R1
            R2 = V/I
            M = (R2-200)/-4.0
            print("r=",R2)
            print("m=",M)
            file2.write(str(round(R2, 3))+"&"+str(round(M, 4))+"\n")
        if int(sys.argv[1])==2:
            Vcc = 3.3
            R1 = 200.0
            R2 = 0
            R2 = (R1*Vcc+(2*R1*V))/(R1*Vcc-(2*R1*V))*R1
            M = (R2-200)/-4.0
            print("r=",R2)
            print("m=",M)
            file2.write(str(round(R2, 3))+"&"+str(round(M, 4))+"\n")
        if int(sys.argv[1])==3:
            R1 = 200.0
            Rf = 20000.0
            Vcc = 3.3
            D = 2*R1*V/((Rf*Vcc)-(R1*V))
            R2 = R1*(1+D)
            M = (R2-200)/-4.0
            print("r=",R2)
            print("m=",M)
            file2.write(str(round(R2, 3))+"&"+str(round(M, 4))+"\n")
file2.close()
print(l)
print(15.0/(2**12))
print(16.0/(2**12))
print(1470.0/(2**12))