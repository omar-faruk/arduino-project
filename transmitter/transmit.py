from gi.repository import Gtk, GObject
import serial
import time

def encode_to_bin(num):
	return bin(num)[2:].zfill(5)
	
def encode_it(char):
	if(ord(char)>=ord('a') and ord(char)<=ord('z')):
		return int(ord(char)-ord('a')+1)
	elif(char==" "):
		return 27
	elif (char=="\n"):
		return 28

class MessengerGUI:
	arduino=None
		
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("messenger.glade")
		self.window = builder.get_object("messengerWindow")
		self.messegeBox=builder.get_object("messege_box")
		self.showBox=builder.get_object("receive_box")
		self.send_button=builder.get_object("send")
		self.connect_button=builder.get_object("connect")
		builder.connect_signals(self)
		
	def on_connect_clicked(self,button):
		connect_arduino=False
		try:
			global arduino
			arduino = serial.Serial('/dev/ttyACM1', 9600)
			connect_arduino=True
		except:
			self.showBox.set_text("Failed to connect on /dev/ttyACM0\n check your usb connection")
		if(connect_arduino==True):
			self.showBox.set_text("Connection Successfull!!!")

	def on_send_clicked(self,button):
		global arduino
		sms=self.messegeBox.get_text()
		sms=str(sms)
		self.showBox.set_text(sms)
		try:
			self.messegeBox.set_text('')
			for i in range (0,len(sms)):
				ascii=encode_it(sms[i])
				char=encode_to_bin(ascii)
				print char
				arduino.write(str(char))
		except:
			self.showBox.set_text("Error !!! Failed to send")
			
	def mnu_quite_app(self, window):
		Gtk.main_quit()

if __name__ == '__main__':
    messenger = MessengerGUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
