# Key Code
# Left Analog X: Axis 0
# Left Analog Y: Axis 1
# Right Analog X: Axis 3
# Right Analog Y: Axis 2

# Button 1: 0
# Button 2: 1
# Button 3: 2
# Button 4: 3

# L1: 4
# R1: 5
# L2: 6
# R2: 7

# Hat X (L:R) : Axis 0 (-1, 1)
# Hat X (D:U) : Axis 1 (-1, 1)

import pygame
import socket

# MACROS
IP_ADDRESS = "192.168.1.69"
PORT = 8888

RIGHT = 0
LEFT = 1

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

pygame.init()
pygame.joystick.init()

class GamePad:
    def __init__(self, index):
        self.index = index
    
    def getButtons(self):
        z = 0
        for i in range(pygame.joystick.get_count()):
            x = pygame.joystick.Joystick(i)
            x.init()
            if i == self.index:
                z = x

        return [z.get_button(i) for i in range(z.get_numbuttons())]
    
    def getAnalog(self):
        z = 0
        for i in range(pygame.joystick.get_count()):
            x = pygame.joystick.Joystick(i)
            x.init()
            if i == self.index:
                z = x

        return [round(z.get_axis(x), 2) for x in range(z.get_numaxes())]
    
j = GamePad(1)

while True:
    analog_data = [chr(int((x+1)*50)) for x in j.getAnalog()]
    analog_data = "".join([analog_data[1], analog_data[2]])
    buttons_data = [str(x) for x in j.getButtons()]
    buttons_data = "".join([buttons_data[0], buttons_data[1], buttons_data[2], buttons_data[3], buttons_data[6], buttons_data[7]])

    data = analog_data + buttons_data

    s.sendto(data.encode(), (IP_ADDRESS, PORT))
    # print([int((x+1)*50) for x in j.getAnalog()])
    print(data)
