from gi.repository import Gtk, GObject
import serial
import datetime
from time import sleep
import os

class MessengerGUI:
	arduino=None
		
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("gui.glade")
		self.window = builder.get_object("mainWindow")
		
		self.dump_button=builder.get_object("dump")
		self.registration_button=builder.get_object("registration")
		self.coursereg_button=builder.get_object("coursereg")
		self.entry_box=builder.get_object("entry_box")
		self.date_button=builder.get_object("date")
		self.clear_button=builder.get_object("cleardb")
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
			
	def on_coursereg_clicked(self,buttton):
		#edit the contents here
		global arduino
		courses=self.entry_box.get_text()
		courses='cr'+courses+'#'
		print courses
		try:
			arduino.write(courses)
			self.showBox.set_text("Course Code Initialized, ReStart Device")
		except:
			self.showBox.set_text("Failed to initialize Courses, Retry")
	
	def on_date_clicked(self,button):
		global arduino
		try:
			date=str(datetime.date.today())
			date=date.replace('-','')
			formated_date="dt"+date
			arduino.write(formated_date)
			print(formated_date)
			self.showBox.set_text("Date Updated")
		except:
			self.showBox.set_text("Failed to set Date")
	
	def on_cleardb_clicked(self,button):
		global arduino
		try:
			arduino.write("cleardb")
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
		#edit the contents here
		global arduino
		count=0
		f=open('temp.txt','w')
		try:
			arduino.write("dump")
			sleep(2.5)
			while(arduino.inWaiting()>=2):
				sms=arduino.readline()
				if(len(sms)>5):
					f.write(sms)
					print sms
					count=count+1
			if(count<1):
					self.showBox.set_text("Data File may not exists,\n check device display? ")
			else:
				f.close()
				self.showBox.set_text("Reading Complete")
		except:
			self.showBox.set_text("Error !!! Read Failed")
		f.close()
		processFile()
		
	def mnu_quite_app(self, window):
		Gtk.main_quit()

def processFile():
	tempFile=open('temp.txt','r')
	courseName=""
	flag=0;
	while(True):
		c=tempFile.read(1)
		if not c:
			break
		if(c!='#' and flag==0):
			courseName=courseName+c;
		else:
			flag=1
			courseFile=open(courseName,'a')
			if(c!='#' and c!="\n"):
				courseFile.write(c)
	courseFile.close()
	
	
if __name__ == '__main__':
    messenger = MessengerGUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
