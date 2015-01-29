import serial
def decodeIt(sms):
	dec=int(sms,2)+ord('a')-1
	
	return chr()
arduino = serial.Serial('/dev/ttyACM0',9600)
sms = arduino.read()
while sms is not None:
    sms=arduino.readline()
    print decodeIt(sms)
