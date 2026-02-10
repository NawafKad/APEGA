from picozero import pico_led
from time import sleep

pico_led.off()

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

def dot():
    pico_led.on()
    sleep(0.3)
    pico_led.off()
    sleep(0.3)

def dash():
    pico_led.on()
    sleep(1)
    pico_led.off()
    sleep(0.3)

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

def translateString(str):
    for i in str:
        translateLetter(i)        
#translateString('hello my name is nawaf')