import serial
def decodeIt(sms):
	dec=(int(sms,2)+ord('a')-1)
	if(dec==27):
		return " "
	elif(dec==28):
		return "."
	else:
		return chr(dec)
arduino = serial.Serial('/dev/ttyACM0',9600)
sms = arduino.read()
while sms is not None:
	print sms
	sms=str(arduino.read())
    #print decodeIt(sms)
