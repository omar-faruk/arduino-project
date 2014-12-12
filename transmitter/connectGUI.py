from gi.repository import Gtk, GObject
import serial
import time
class MessengerGUI:
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("messenger.glade")
		self.window = builder.get_object("messengerWindow")
		self.messegeBox=builder.get_object("messege_box")
		self.showBox=builder.get_object("receive_box")
		self.send_button=builder.get_object("send")
		self.connect_button=builder.get_object("connect")
		builder.connect_signals(self)
	def on_send_clicked(self,button):
		sms=self.messegeBox.get_text()
		self.showBox.set_text(sms)
		self.messegeBox.set_text('')
	def on_connect_clicked(self,button):
		try:
			arduino = serial.Serial('/dev/ttyACM0', 9600)  
		except:
			self.showBox.set_text("Failed to connect on /dev/ttyUSB0\n check your usb connection")
	def mnu_quite_app(self, window):
		Gtk.main_quit()

if __name__ == '__main__':
    messenger = MessengerGUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
