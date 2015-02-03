import serial
arduino = serial.Serial('/dev/ttyACM0', 9600)
sms = arduino.read()
while sms is not None:
	print sms
	sms=arduino.read()
