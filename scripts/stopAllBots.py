import serial
print "cheking ports"
for i in range(0,4):
    ser = serial.Serial("/dev/rfcomm"+str(i), 9600, timeout=1)
    if(ser.read()!='r'):
        print "rfcomm" + str(i) + " did not respond correctly"
    else:
        ser.write("\x00")
    ser.close()
print "complete"
