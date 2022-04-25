from bluepy.btle import Scanner, Peripheral, DefaultDelegate
import struct, time

STRUCT_FORMAT="i"
rcv_dict={"moisture":0}

class ScanDelegate(DefaultDelegate):
	def __init__(self):
		DefaultDelegate.__init__(self)
	def handleDiscovery(self, dev, isNewDev, isNewData):
		if isNewDev:
			print( "Discovered device", dev.addr)
		elif isNewData:
			print("Received new data from", dev.addr)
	def handleNotification(self,cHandle,data):
		print("handling notification...")
		print(self)
		print(cHandle)
		print(struct.unpack("b",data))


scanner = Scanner().withDelegate(ScanDelegate())
devices = scanner.scan(10.0)
periph = Peripheral("84:CC:A8:30:50:8E")
periph.setDelegate(ScanDelegate())
for dev in devices:
	print("Device %s (%s), RSSI=%d dB" % (dev.addr, dev.addrType, dev.rssi)) #received signal strength indicator
	for (adtype, desc, value) in dev.getScanData():
		print( " %s = %s" % (desc,value))
while True:
	if periph.waitForNotifications(1.0):
		continue
	print("waiting...")
	print(periph.getServiceByUUID("1101"))
	byte_rcv=periph.getCharacteristics()
	rcv_dict["moisture"]=struct.unpack(STRUCT_FORMAT,byte_rcv)
	print(rcv_dict)	

