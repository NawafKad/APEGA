from picozero import pico_led
from time import sleep
#picozero is the library we are using, we are importing specifically the pico built in led
#----keep in mind that when we connect external parts, it won't be as easy as importing the object------

#we are also importing the sleep method from the microprocessors built in time libraries

#making sure the picos built in LED is off.
pico_led.off()

#defines each letter an it's coresponding morse code
morse_code = {
    'a': ".-",
    'b': "-...",
    'c': "-.-.",
    'd': "-..",
    'e': ".",
    'f': "..-.",
    'g': "--.",
    'h': "....",
    'i': "..",
    'j': ".---",
    'k': "-.-",
    'l': ".-..",
    'm': ".--",
    'n': "-.",
    'o': "---",
    'p': ".--.",
    'q': "--.-",
    'r': ".-.",
    's': "...",
    't': "-",
    'u': "..-",
    'v': "...-",
    'w': ".--",
    'x': "-..-",
    'y': "-.--",
    'z': "--..",
    ' ': ' '
}

#defines what the dot flash looks like and how long it is sustained for
def dot():
    pico_led.on()
    sleep(0.3)
    pico_led.off()
    sleep(0.3)
    
#defines what the dash flash looks like and how long it is sustained for
def dash():
    pico_led.on()
    sleep(1)
    pico_led.off()
    sleep(0.3)

#translates the letter to the morse code
def translateLetter(char):
    str = morse_code[char]
    print(char + ' ' + str)
    for i in str:
        if i == '.':
            dot()
        elif i == '-':
            dash()
        elif i == ' ':
            sleep(1)

#goes through a given string translating each letter with it's given code
def translateString(str):
    for i in str:
        translateLetter(i)        

