from gi.repository import Gtk, GObject
import serial
import time
import os

class MessengerGUI:
	arduino=None
		
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("messenger.glade")
		self.window = builder.get_object("messengerWindow")
		self.dump_button=builder.get_object("dump")
		self.showBox=builder.get_object("receive_box")
		self.connect_button=builder.get_object("connect")
		builder.connect_signals(self)
		
	def on_connect_clicked(self,button):
		connect_arduino=False
		try:
			global arduino
			arduino = serial.Serial('/dev/ttyACM0', 9600)
			connect_arduino=True
		except:
			self.showBox.set_text("Failed to connect on /dev/ttyACM0\n check your usb connection")
		if(connect_arduino==True):
			self.showBox.set_text("Connection Successfull!!!")

	def on_dump_clicked(self,button):
		global arduino
		count=0
		f=open('db.txt','w')
		try:
			arduino.write("dump")
			while(arduino.inWaiting()>=11):
				sms=arduino.readline()
				f.write(sms)
				print (sms)
				count=count+1
			if(count<2):
					self.showBox.set_text("Missed timing, try again")
			else:
				f.close()
				self.showBox.set_text("Dumping Complete")
		except:
			self.showBox.set_text("Error !!! Dumping Failed")
			
	def mnu_quite_app(self, window):
		Gtk.main_quit()

if __name__ == '__main__':
    messenger = MessengerGUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
