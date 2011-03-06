# usr/bin/env python
import pexpect, sys, os, time

TIMEOUT=10

# copy our stuff to cfserver
if os.system('cfcp %s > /dev/null' % sys.argv[1]) != 0:
	print 'Error copying firmware'
else:   
	print 'Successfully copied %s to cf-server' % sys.argv[1]

cfserver = pexpect.spawn('telnet cf-server')

# grab the 2nd serial port info
cfserver.expect('Second Serial Port On Port: ([0-9]*\n)')
PORT = int(cfserver.match.group(1))

# select "continue executing" in cf-server
cfserver.expect('Please Pick', timeout=TIMEOUT)
cfserver.expect(': ', timeout=TIMEOUT)
cfserver.send('1')
print "Started JanusROM"
time.sleep(1)

# wait for the janusROM prompt to init tftp
cfserver.expect('janusROM> ', timeout=TIMEOUT)
cfserver.send('tftp -f %s\r\n' % sys.argv[1])
cfserver.expect('Finished. Start Address is [0-9a-fA-Fx]+\.', timeout=TIMEOUT)
print "Successfully loaded %s" % sys.argv[1]

# make your life easier
print "Second serial port on port: %i" % PORT
print "telnet cf-server %i" % PORT
cfserver.interact()
