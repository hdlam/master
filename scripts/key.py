#! /usr/bin/env python
############################################################################
# File name : keyinput.py
# Purpose : A Small Demo Practice Prgram using Pygame API
# Usages : Displays the pressed keys on the screen
# Start date : 17/12/2011
# End date : 17/12/2011
# Author : Ankur Aggarwal
# License : GNU GPL v3 http://www.gnu.org/licenses/gpl.html
# Dependency  : None
############################################################################

#importing modules
import pygame, serial
from pygame.locals import *
import sys


#initializing variables
pygame.init()
screen=pygame.display.set_mode((680,400),0,24)
pygame.display.set_caption("Key Press Test")
global ser
connected = False
global bot
bot = 3 
if(len(sys.argv) > 1):
    bot = sys.argv[1]
print "using /dev/rfcomm" + str(bot)
#main loop which displays the pressed keys on the screen
while True:
    for i in pygame.event.get():
        screen.fill((0,0,0))
        if pygame.key.get_focused():
            press=pygame.key.get_pressed()
            for i in xrange(0,len(press)): 
                if press[i]==1:
                    name=pygame.key.name(i) 
                    print name
                    if name == 'q' or name == 'escape':
                        if(connected):
                            ser.write('\x00')
                            ser.close()
                        exit()
                    elif name == 'space':
                        if(not connected):
                            ser = serial.Serial("/dev/rfcomm"+str(bot), 9600, timeout=1)
                        connected = True
                    if(connected):
                        if name == 'w' or name == 'up':
                            ser.write('W')
                        elif name == 's' or name == 'down':
                            ser.write('B')
                        elif name == 'a' or name == 'left':
                            ser.write('L')
                        elif name == 'd' or name == 'right':
                            ser.write('R')
                        else:
                            ser.write('\x00')
    pygame.display.update()

