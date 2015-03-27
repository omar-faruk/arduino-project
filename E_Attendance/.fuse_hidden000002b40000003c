from gi.repository import Gtk, GObject
import serial
from time import sleep
import os

class MessengerGUI:
	arduino=None
		
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("messenger.glade")
		self.window = builder.get_object("messengerWindow")
		self.dump_button=builder.get_object("dump")
		self.registration_button=builder.get_object("registration")
		self.deregistration_button=builder.get_object("deregistration")
		self.dump_button=builder.get_object("attendance")
		self.dump_button=builder.get_object("cleardb")
		self.showBox=builder.get_object("receive_box")
		self.connect_button=builder.get_object("connect")
		builder.connect_signals(self)
	
	def on_registration_clicked(self,button):
		global arduino
		try:
			arduino.write("registration-mode")
			self.showBox.set_text("Device set to Registration Mode")
		except:
			self.showBox.set_text("Failed to set Registration Mode")
			
	def on_deregistration_clicked(self,buttton):
		global arduino
		try:
			arduino.write("deregistration-mode")
			self.showBox.set_text("Device set to De-Registration Mode")
		except:
			self.showBox.set_text("Failed to set De-Registration Mode")
	
	def on_attendance_clicked(self,button):
		global arduino
		try:
			arduino.write("attendance-mode")
			self.showBox.set_text("Device set to Attendance Mode")
		except:
			self.showBox.set_text("Failed to set Attendance Mode")
	
	def on_cleardb_clicked(self,button):
		global arduino
		try:
			arduino.write("clear-db")
			self.showBox.set_text("SD database cleared")
		except:
			self.showBox.set_text("Failed to clear SD database")
	
	def on_connect_clicked(self,button):
		connect_arduino=False
		deviceList=['/dev/ttyACM0','/dev/ttyACM1','/dev/ttyACM2','/dev/ttyACM3']
		for device in deviceList:
			try:
				global arduino
				arduino = serial.Serial(device,9600,timeout=10)
				connect_arduino=True
			except:
				self.showBox.set_text("Failed to connect on,\n check your usb connection")
			if(connect_arduino==True):
				self.showBox.set_text("Connection Successfull!!!")
				break

	def on_dump_clicked(self,button):
		global arduino
		count=0
		f=open('db.txt','w')
		try:
			arduino.write("dump-atd")
			sleep(2.5)
			while(arduino.inWaiting()>=2):
				sms=arduino.readline()
				if(len(sms)>5):
					f.write(sms)
					print sms
					count=count+1
			if(count<1):
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
