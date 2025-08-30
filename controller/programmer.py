from serial import Serial
import subprocess
import os
import pathlib
from parse import parse
import argparse

PROGRAMMER_PATH=os.getenv('STM32_PROGRAMMER')

BUFFER_SIZE=16
class DeviceSerial():
    
    def __init__(self, port, read_timeout_s=None, write_timeout_s=None):
        self.serial = Serial(port, 115200, timeout=read_timeout_s, write_timeout=write_timeout_s)

    def write(self, msg):
        buf = msg.encode()
        buf = buf.ljust(BUFFER_SIZE, b'\0')[:BUFFER_SIZE]
        self.serial.write(buf)

    def read(self):
        buf = self.serial.read(BUFFER_SIZE)
        buf = buf.decode().rstrip('\0')
        return buf.strip()


class Device():
    def __init__(self, swd_sn, serial_port):
        self.swd_sn = swd_sn
        self.serial = DeviceSerial(serial_port, 5, 5)


    def _programmer_prefix(self):
        return [PROGRAMMER_PATH, '-c', 'port=SWD', 'freq=8000', 'sn={sn}'.format(sn=self.swd_sn)]

    def erase(self):
        proc = subprocess.run(self._programmer_prefix() + ['-e', 'all'])
        return proc.returncode
    
    def reset(self):
        proc = subprocess.run(self._programmer_prefix() + ['-rst', '-run'])
        return proc.returncode
    
    def flash(self, filepath):
        proc = subprocess.run(self._programmer_prefix() + ['-w', filepath])
        return proc.returncode
    
    def check_ready(self):
        ret = self.serial.read()
        return ret == 'READY'
    
    def ping(self):
        self.serial.write("PING")
        ret = self.serial.read()
        return ret == 'PONG'
    
    def run_test(self, case_num):
        self.serial.write("RUN {}".format(case_num))
        ret = self.serial.read()

        if len(ret) == 0:
            # Empty return means we timed out, therefore fail
            return 2
        
        ret = parse('RESULT {case_num:d} {result:d}', ret)
        return ret['result']
    
    def last_result(self):
        self.serial.write("RESULT")
        ret = self.serial.read()
        ret = parse('RESULT {case_num:d} {result:d}', ret)
        return (ret['case_num'], ret['result'])

    def soft_reset(self):
        self.serial.write("RESTART")

def main():
    parser = argparse.ArgumentParser(prog='Programmer', description='')
    parser.add_argument('-s', '--serial_number', required=True)
    parser.add_argument('-p', '--port', required=True)

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-w', '--write', type=pathlib.Path)
    group.add_argument('-t', '--test', type=int)
    
    args = parser.parse_args()

    device = Device(args.serial_number, args.port)
    if args.write is not None:
        device.reset()
        device.erase()
        device.flash(args.write)
    
    if args.test is not None:
        device.reset()
        device.check_ready()
        result = device.run_test(args.test)
        if result == 0:
            print('PASS')
        elif result == 1:
            print('SKIP')
        elif result == 2:
            print('FAIL')
        else:
            print('UNKNOWN')


if __name__ == '__main__':
    main()