#!/usr/bin/ruby
#
# stty -F /dev/ttyUSB0; cat /dev/ttyUSB0 | ./checkImu.rb
#

require	"zlib"
require	"rubygems"
gem 'serialport','>=1.0.4'
require	"serialport"

$tty = "/dev/ttyUSB0"

$cntRecv  = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
$cntError = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
$tsPrev   = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
$cntPrev  = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]

$tStart = 0;
$tPrev = 0;

$tblCorrection = [0x00, 0x10, 0x20, 0x37, 0x30, 0x27, 0x17, 0x07, 0x40, 0x59, 0x6a, 0x7b, 0x7c, 0x6d, 0x5e, 0x47,
                  0x50, 0x49, 0x72, 0x63, 0x64, 0x75, 0x4e, 0x57, 0x19, 0x09, 0x3e, 0x29, 0x2e, 0x39, 0x0e, 0x1e,
                  0x60, 0x71, 0x4a, 0x53, 0x54, 0x4d, 0x76, 0x67, 0x2a, 0x3d, 0x0a, 0x1a, 0x1d, 0x0d, 0x3a, 0x2d,
                  0x34, 0x23, 0x13, 0x03, 0x04, 0x14, 0x24, 0x33, 0x78, 0x69, 0x5a, 0x43, 0x44, 0x5d, 0x6e, 0x7f,
                  0x70, 0x61, 0x52, 0x4b, 0x4c, 0x55, 0x66, 0x77, 0x3c, 0x2b, 0x1b, 0x0b, 0x0c, 0x1c, 0x2c, 0x3b,
                  0x22, 0x35, 0x02, 0x12, 0x15, 0x05, 0x32, 0x25, 0x68, 0x79, 0x42, 0x5b, 0x5c, 0x45, 0x7e, 0x6f,
                  0x11, 0x01, 0x36, 0x21, 0x26, 0x31, 0x06, 0x16, 0x58, 0x41, 0x7a, 0x6b, 0x6c, 0x7d, 0x46, 0x5f,
                  0x48, 0x51, 0x62, 0x73, 0x74, 0x65, 0x56, 0x4f, 0x08, 0x18, 0x28, 0x3f, 0x38, 0x2f, 0x1f, 0x0f
                 ]


def textout
  if(($tPrev+10) <= Time.now) then
    $tPrev = Time.now

    tDiff = Time.now - $tStart;

    printf("t: %6d cnt: %d(%d) %d(%d) %d(%d) %d(%d)  ",
           tDiff,
           $cntRecv[0], $cntError[0],
           $cntRecv[1], $cntError[1],
           $cntRecv[2], $cntError[2],
           $cntRecv[3], $cntError[3]);
    printf("fs: %5.2f %5.2f %5.2f %5.2f\n",
           $cntRecv[0]/tDiff, $cntRecv[1]/tDiff,
           $cntRecv[2]/tDiff, $cntRecv[3]/tDiff)
  end
end


def ShowImu(pos, ts, no, cnt, capability, accx, accy, accz, gyrox, gyroy, gyroz, temp, sum, crcrx)
  printf("len: %2d ts: %7x no: %x cnt: %2x capability: %4x ", pos, ts, no, cnt, capability)
  if((capability & (1<<0)) != 0) then
    printf("acc: %4x %4x %4x ", accx, accy, accz)
  end
  if((capability & (1<<1)) != 0) then
    printf("gyro: %4x %4x %4x ", gyrox, gyroy, gyroz)
  end
  if((capability & (1<<3)) != 0) then
    printf("temp: %2x ", temp)
  end
  printf("sum: %8x crcrx: %8x ", sum, crcrx)
  print "\n"
end

def mainText
  ts = 0
  no = 0
  mask = 0xffff

  $tStart = Time.now
  $tPrev = $tStart

  STDIN.each_line do |line|
    words = line.split
    no = words[1].to_i;
    ts = words[2].hex & mask;

    if($tsPrev[no] != -1 && (($tsPrev[no]+1)&mask) != ts) then
      print "#{no} #{ts} #{$tsPrev[no]}\n"
      $cntError[no] += 1
    else
      $cntRecv[no] += 1
    end
    $tsPrev[no] = ts;

    textout()

  end
end


def mainHamming
  ts = 0
  no = 0
  cnt = 0
  mask = 0xffff
  buf = Array.new
  pos = 0
  tIdle = 0;

  $tStart = Time.now
  $tPrev = $tStart

  srand(Time.now.to_i)

  #
  # 1.  gem install serialport
  # 2.  edit the higher baud rate code in
  #         /var/lib/gems/2.3.0/gems/serialport-1.3.1/ext/native/posix_serialport_impl.c
  #
  #       #ifdef B2000000
  #           case 2000000: data_rate = B2000000; break;
  #       #endif
  #       #ifdef B4000000
  #           case 4000000: data_rate = B4000000; break;
  #       #endif
  #
  # 3.  cd /var/lib/gems/2.3.0/gems/serialport-1.3.1/ext/native/posix_serialport_impl.c
  # 4.  sudo make
  # 5.  cp serialport.so  ../../lib/serialport/.
  #
  dSerial = SerialPort.open($tty, 4000000, 8, 1, 0)
  dSerial.binmode()

  dSerial.write("start\n")

  tIdle = Time.now
  fIdle = 0
  ret = 1

  pos = 0
  ts = 0
  no = 0
  cnt = 0
  capability = 0
  accx = 0
  accy = 0
  accz = 0
  gyrox = 0
  gyroy = 0
  gyroz = 0
  temp = 0
  sum = 0;
  crcrx = 0

  fStartStopEn = 0

#  dSerial.each_byte do |c|

  while true do
    ret = IO.select([dSerial], nil, nil, 0)

      if(fStartStopEn == 1 && (Time.now - tIdle) >= 2) then
        tIdle = Time.now
        if(fIdle == 1) then
          dSerial.write("start\n")
          fIdle = 0;
        else
          dSerial.write("stop\n")
          fIdle = 1;
        end
      end

    if(ret != nil) then
      c = dSerial.read(1).ord



      if(c != nil) then
        if((c & 0x80) == 0x80) then
          sumcalc = -1
          begin
            ### time stamp
            idx = 0
            tsunit  = (buf[idx+0]) <<  0
            tsunit |= (buf[idx+1]) <<  4
            tsunit |= (buf[idx+2]) <<  8
            tsunit |= (buf[idx+3]) << 12
            tsunit |= (buf[idx+4]) << 16
            tsunit |= (buf[idx+5]) << 20
            tsunit |= (buf[idx+6]) << 24
            tsunit |= (buf[idx+7]) << 28
            idx += 8
            #sumcalc ^= tsunit >> 24;
            sumcalc ^= tsunit >> 16;
            sumcalc ^= tsunit >>  8;
            sumcalc ^= tsunit >>  0;

            ### unit number
            no = (tsunit >> 28) & 0xf
            #          no = (buf[idx+0])
            #          idx += 1
            sumcalc ^= no;

            ### count
            cnt  = (buf[idx+0] & 0xf) <<  0
            cnt |= (buf[idx+1] & 0xf) <<  4
            #          cnt |= (buf[idx+2] & 0xf) <<  8
            #          cnt |= (buf[idx+3] & 0xf) << 12
            idx += 2
            sumcalc ^= cnt >> 8;
            sumcalc ^= cnt >> 0;

            ### capability
            ### opt, crc, -, -, -, mag, gyro, accel
            capability  = (buf[idx+0] & 0xf) <<  0
            capability |= (buf[idx+1] & 0xf) <<  4
            capability |= (buf[idx+2] & 0xf) <<  8
            capability |= (buf[idx+3] & 0xf) << 12
            idx += 4

            ### accel
            if((capability & (1<<0)) != 0) then
              accx  = (buf[idx+0] & 0xf) <<  0
              accx |= (buf[idx+1] & 0xf) <<  4
              accx |= (buf[idx+2] & 0xf) <<  8
              accx |= (buf[idx+3] & 0xf) << 12
              accy  = (buf[idx+4] & 0xf) <<  0
              accy |= (buf[idx+5] & 0xf) <<  4
              accy |= (buf[idx+6] & 0xf) <<  8
              accy |= (buf[idx+7] & 0xf) << 12
              accz  = (buf[idx+8] & 0xf) <<  0
              accz |= (buf[idx+9] & 0xf) <<  4
              accz |= (buf[idx+10] & 0xf) <<  8
              accz |= (buf[idx+11] & 0xf) << 12
              idx += 12
              sumcalc ^= accx >> 8;
              sumcalc ^= accx >> 0;
              sumcalc ^= accy >> 8;
              sumcalc ^= accy >> 0;
              sumcalc ^= accz >> 8;
              sumcalc ^= accz >> 0;
            else
              accx = 0
              accy = 0
              accz = 0
            end

            ### gyro
            if((capability & (1<<1)) != 0) then
              gyrox  = (buf[idx+0] & 0xf) <<  0
              gyrox |= (buf[idx+1] & 0xf) <<  4
              gyrox |= (buf[idx+2] & 0xf) <<  8
              gyrox |= (buf[idx+3] & 0xf) << 12
              gyroy  = (buf[idx+4] & 0xf) <<  0
              gyroy |= (buf[idx+5] & 0xf) <<  4
              gyroy |= (buf[idx+6] & 0xf) <<  8
              gyroy |= (buf[idx+7] & 0xf) << 12
              gyroz  = (buf[idx+8] & 0xf) <<  0
              gyroz |= (buf[idx+9] & 0xf) <<  4
              gyroz |= (buf[idx+10] & 0xf) <<  8
              gyroz |= (buf[idx+11] & 0xf) << 12
              idx += 12
              sumcalc ^= gyrox >> 8;
              sumcalc ^= gyrox >> 0;
              sumcalc ^= gyroy >> 8;
              sumcalc ^= gyroy >> 0;
              sumcalc ^= gyroz >> 8;
              sumcalc ^= gyroz >> 0;
            else
              gyrox = 0;
              gyroy = 0;
              gyroz = 0;
            end

            ### temp
            if((capability & (1<<3)) != 0) then
              temp   = (buf[idx+0] & 0xf) <<  0
              temp  |= (buf[idx+1] & 0xf) <<  4
              idx += 2
              sumcalc ^= temp;
            else
              temp = 0
            end

            ### sum, crc
            sum    = (buf[idx+0] & 0xf) <<  0
            sum   |= (buf[idx+1] & 0xf) <<  4
            sumcalc ^= sum
            sumcalc &= 0xff

            if((capability & (3<<13)) != 0) then # CRC32 least 1 bytes
              crcmask = 0xff
              sumcalc = -1
              crcrx    = (buf[idx+0] & 0xf) <<  0
              crcrx   |= (buf[idx+1] & 0xf) <<  4
              if((capability & (1<<13)) != 0) then # CRC32 all bits
                crcmask = 0xffffffff
                crcrx   |= (buf[idx+2] & 0xf) <<  8
                crcrx   |= (buf[idx+3] & 0xf) << 12
                crcrx   |= (buf[idx+4] & 0xf) << 16
                crcrx   |= (buf[idx+5] & 0xf) << 20
                crcrx   |= (buf[idx+6] & 0xf) << 24
                crcrx   |= (buf[idx+7] & 0xf) << 28
              end

              str = [(tsunit >>  0)&0xff, (tsunit >>  8)&0xff,
                     (tsunit >> 16)&0xff, (tsunit >> 24)&0xff].pack('c4')
              str << [(cnt >> 0)&0xff].pack("c1")
              str << [(capability>>0)&0xff, (capability>>8)&0xff].pack("c2")
              if((capability & (1<<0)) != 0) then
                str << [(accx>>0)&0xff, (accx>>8)&0xff].pack("c2")
                str << [(accy>>0)&0xff, (accy>>8)&0xff].pack("c2")
                str << [(accz>>0)&0xff, (accz>>8)&0xff].pack("c2")
              end
              if((capability & (1<<1)) != 0) then
                str << [(gyrox>>0)&0xff,  (gyrox>>8)&0xff].pack("c2")
                str << [(gyroy>>0)&0xff,  (gyroy>>8)&0xff].pack("c2")
                str << [(gyroz>>0)&0xff,  (gyroz>>8)&0xff].pack("c2")
              end
              if((capability & (1<<3)) != 0) then
                str << [temp&0xff].pack("c1")
              end

              crc = 0
              crc = Zlib.crc32(str, crc);
              if(((crcrx ^ crc) & crcmask) == 0) then
                sumcalc = 0
              end
            end
          rescue
          end

          if(sumcalc == 0) then
            if(no < 16) then
              ts = tsunit & 0xfffffff
              #ShowImu(pos, ts, no, cnt, capability, accx, accy, accz, gyrox, gyroy, gyroz, temp, crcrx, crc)

              ### check count
              fError = 0
              if($cntPrev[no] != -1 && (($cntPrev[no]+1)&0xff) != cnt) then
                printf("# error cnt unit:%d cnt:%2x cntPrev:%x\n", no, cnt, $cntPrev[no])
                $cntError[no] += 1
                fError |= 1
              end
              $cntPrev[no] = cnt;

              ### check timestamp
              if($tsPrev[no] != -1 && $tsPrev[no] >= ts &&
                 !(($tsPrev[no] | 0x000ffff) == 0xfffffff && (ts & 0xfff0000) == 0)) then
                printf("# error ts unit:%d ts:%x tsPrev:%x\n", no, ts, $tsPrev[no])
                $cntError[no] += 1
                fError |= 1
              end
              $tsPrev[no] = ts;

              if(fError != 1) then
                $cntRecv[no] += 1
              end

              textout()
            end
          else
            textout()
            printf("checksum error %x\n", sumcalc)
          end

          pos = 0;
        end

        ### error generation
        error = c ^ (1 << rand(7))

        ### error correction
        correction = $tblCorrection[error & 0x7f]

        #      printf("xx %x %x %x\n", c, error, correction)

        buf[pos] = correction & 0xf;
        pos += 1

      end
    end
  end
end


mainHamming
