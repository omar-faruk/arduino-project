from gi.repository import Gtk, GObject
import MySQLdb as my
import serial
import datetime
from time import sleep
import os

class GUI:
	arduino=None
		
	def __init__(self):
		builder = Gtk.Builder()
		builder.add_from_file("gui.glade")
		self.window = builder.get_object("mainWindow")
		
		self.dump_button=builder.get_object("dump")
		self.registration_button=builder.get_object("registration")
		self.coursereg_button=builder.get_object("coursereg")
		
		self.entry_box=builder.get_object("entry_box")
		self.entry_box1=builder.get_object("entry_box1")
		
		self.date_button=builder.get_object("date")
		self.count_button=builder.get_object("countatd")
		
		self.showBox=builder.get_object("receive_box")
		self.showBox2=builder.get_object("receive_box2")
		self.showBox3=builder.get_object("receive_box3")
		
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
			self.showBox3.set_text("Date Updated")
		except:
			self.showBox3.set_text("Failed to set Date")



	def on_countatd_clicked(self,button):
		print ('attendance counting')
		courses=self.entry_box1.get_text();
		course_list=courses.split()
		success=0
		for course_code in course_list:
			#print (course_code)
			count_atd(course_code)
			self.entry_box1.set_placeholder_text("done")
			



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
					self.showBox2.set_text("Data File may not exists,\n check device display? ")
			else:
				f.close()
				self.showBox2.set_text("Reading Complete")
		except:
			self.showBox2.set_text("Error !!! Read Failed")
		f.close()
		processFile()
		

	def mnu_quite_app(self, window):
		Gtk.main_quit()
#class definition ends here




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



def count_atd(course_code):
	with open(course_code,'r+') as text:
		while(True):
			line=text.readline()
			if not line:
				break
			line=line.replace(',',' ')
			raw_list=line.split(' ')
			
			student_id=[]
			atd_count=[]
			counter=0
			date_count=0
			
			for i in range (0,len(raw_list)):
				if(len(raw_list[i])>8):
					student_id.append(raw_list[i])
					if(counter>0):
						atd_count.append(date_count)
						date_count=0
					counter=counter+1
				elif(len(raw_list[i])==8):
					date_count=date_count+1
				if(i==len(raw_list)-1):
					atd_count.append(date_count)
			
			atd_count_file=course_code+"_final.txt"
			course_atd_file=open(atd_count_file,'a+')
			for i in range (0,len(student_id)):
				if(i==0):
					student_id[0]=student_id[0][1:]
				course_atd_file.write(student_id[i]+" "+str(atd_count[i])+"\n")
				insert_into_db(student_id[i],course_code,atd_count[i])
				#print(student_id[i])
				#print(atd_count[i])
				#print("\n")
			course_atd_file.close()
		text.close()


def insert_into_db(id_card,course_code,atd_count):
	db = my.connect(host="127.0.0.1",user="root",passwd="cse22@JU",db="test")
 
	cursor = db.cursor()
	
	sql = "insert into attendance VALUES('%s', '%s', %d)" % (id_card,course_code,atd_count)
	
	number_of_rows = cursor.execute(sql)
	db.commit()   # you need to call commit() method to save 
	db.close()
	
	
if __name__ == '__main__':
    messenger = GUI()
    messenger.window.connect("delete-event", Gtk.main_quit)
    messenger.window.show_all()
    Gtk.main()
