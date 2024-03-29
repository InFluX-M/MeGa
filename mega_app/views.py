# arduino_panel/views.py
from django.http import JsonResponse
from django.shortcuts import render
import serial
import time
from django.views.decorators.csrf import csrf_exempt

import logging


logger = logging.Logger(__name__)
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
blink_state = 0
T = 1000

@csrf_exempt 
def index(request):
    return render(request, 'index.html')

@csrf_exempt 
def send_command(request):
    global logger, ser, blink_state, T
    
    if request.method == 'POST':
        command = request.POST.get('command', '')
        ser.write((command + '\r' + '\n').encode())
        
        if blink_state == 1:
            time.sleep((len(command) + 2) * T / 1000)
    
        if command == "BLINKING=ON":
            blink_state = 1
    
        if command == "BLINKING=OFF":
            blink_state = 0

        if "PERIOD" in command:
            if command[6] == '0':
                T = int(command[8:])
            else:
                T = int(command[7:])
    
        time.sleep(0.5)

        res = ser.readline().decode().strip()
        response_from_arduino = f"{res}"
        return JsonResponse({'response': response_from_arduino})

    return JsonResponse({'error': 'Invalid request method'})
