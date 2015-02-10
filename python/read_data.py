import emotiv
import gevent

headset = emotiv.Emotiv()
gevent.spawn(headset.setup)
gevent.sleep(0)
print "Starting"
try:
    while True:
        packet = headset.dequeue()
        if packet != None:
            for key in packet.sensors.keys():
                print key + ": " + str(packet.sensors[key]['value']),
            print "\n"
                    
        gevent.sleep(0)
except KeyboardInterrupt:
    headset.close()
finally:
    headset.close()
