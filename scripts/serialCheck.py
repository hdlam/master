import serial, sys
print "cheking ports"

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


for i in range(0,4):
    try:
        ser = serial.Serial("/dev/rfcomm"+str(i), 9600, timeout=1)
        got = ser.read()
        if(got!='r'):
            print bcolors.WARNING + "rfcomm" + str(i) + " did not respond correctly" + bcolors.ENDC
            print bcolors.OKBLUE + "got response: " + int(got) + "." + bcolors.ENDC
        ser.close()
    except:
        print bcolors.FAIL + "rfcomm" + str(i) + " was busy or sumthing"+bcolors.ENDC
print "complete"
