from gi.repository import Gtk, GObject
import serial
import time
	
def decode_it(bi):
	dec=int(bi,2);
	if(dec<=26):
		return dec+ord('a')-1
	elif(dec==27):
		return " "
	elif(dec==28):
		return "\n"
		
class MessengerGUI:
	arduino=None
		
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("messenger.glade")
		self.window = builder.get_object("messengerWindow")
		self.showBox=builder.get_object("receive_box")
		self.connect_button=builder.get_object("connect")
		builder.connect_signals(self)

	def on_connect_clicked(self,button):
		try:
			global arduino
			arduino = serial.Serial('/dev/ttyACM0', 9600)
			sms = arduino.read()
			#while sms is not None:
			sms = arduino.readline()
			sms=str(sms)
			print sms
			self.showBox.set_text(sms)
		except:
			self.showBox.set_text("Failed to connect on /dev/ttyACM0\n check your usb connection")
			
			
	def mnu_quite_app(self, window):
		Gtk.main_quit()

if __name__ == '__main__':
    messenger = MessengerGUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
