import serial
import time
try:
	arduino = serial.Serial('/dev/ttyACM0', 9600)  
except:
	print "Failed to connect on /dev/ttyUSB0"

try:
	for i in range (1,10):
		print "Trying to send"
		arduino.write(str(i*10))
		time.sleep(4)
except:
	print "Failed to connect/send..."
