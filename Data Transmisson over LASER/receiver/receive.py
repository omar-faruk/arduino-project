from gi.repository import Gtk, GObject
import serial
import time
class MessengerGUI:
	arduino=None
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("messenger.glade")
		self.window = builder.get_object("messengerWindow")
		self.showBox=builder.get_object("receive_box")
		builder.connect_signals(self)
		GObject.timeout_add(1000,self.run)
		
	def run(self):
		#global arduino
		#arduino = serial.Serial('/dev/ttyACM0', 9600)
		#sms = str(arduino.read())
		#sms=raw_input()
		#print sms
		self.showBox.set_text(sms)
		return True

	def mnu_quite_app(self, window):
		Gtk.main_quit()

if __name__ == '__main__':
    messenger = MessengerGUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
