#!/usr/bin/ruby
#
# stty -F /dev/ttyUSB0; cat /dev/ttyUSB0 | ./checkImu.rb
#

$cntRecv  = [0, 0, 0, 0]
$cntError = [0, 0, 0, 0]
$tsPrev   = [-1, -1, -1, -1]
$cntPrev  = [-1, -1, -1, -1]

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


def ShowImu(pos, ts, no, cnt, accx, accy, accz, gyrox, gyroy, gyroz, temp, sum)
          printf("len: %2d, ts: %6x, no: %x, cnt: %4x, ", pos, ts, no, cnt)
          printf("acc: %4x %4x %4x ", accx, accy, accz)
          printf("gyro: %4x %4x %4x ", gyrox, gyroy, gyroz)
          printf("temp: %2x ", temp)
          printf("sum: %2x ", sum)
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

  $tStart = Time.now
  $tPrev = $tStart

  srand(Time.now.to_i)

  STDIN.each_byte do |c|
    if(c != nil) then

      if((c & 0x80) == 0x80 && pos >= 39) then
        sumcalc = 0;

        ### time stamp
        ts  = (buf[0]) <<  0
        ts |= (buf[1]) <<  4
        ts |= (buf[2]) <<  8
        ts |= (buf[3]) << 12
        ts |= (buf[4]) << 16
        ts |= (buf[5]) << 20
        sumcalc ^= ts >> 16;
        sumcalc ^= ts >>  8;
        sumcalc ^= ts >>  0;

        ### unit number
        no = buf[6];
        sumcalc ^= no;

        ### count
        cnt  = (buf[ 7] & 0xf) <<  0
        cnt |= (buf[ 8] & 0xf) <<  4
        cnt |= (buf[ 9] & 0xf) <<  8
        cnt |= (buf[10] & 0xf) << 12
        sumcalc ^= cnt >> 8;
        sumcalc ^= cnt >> 0;

        ### accel
        accx  = (buf[11] & 0xf) <<  0
        accx |= (buf[12] & 0xf) <<  4
        accx |= (buf[13] & 0xf) <<  8
        accx |= (buf[14] & 0xf) << 12
        sumcalc ^= accx >> 8;
        sumcalc ^= accx >> 0;
        accy  = (buf[15] & 0xf) <<  0
        accy |= (buf[16] & 0xf) <<  4
        accy |= (buf[17] & 0xf) <<  8
        accy |= (buf[18] & 0xf) << 12
        sumcalc ^= accy >> 8;
        sumcalc ^= accy >> 0;
        accz  = (buf[19] & 0xf) <<  0
        accz |= (buf[20] & 0xf) <<  4
        accz |= (buf[21] & 0xf) <<  8
        accz |= (buf[22] & 0xf) << 12
        sumcalc ^= accz >> 8;
        sumcalc ^= accz >> 0;

        ### gyro
        gyrox  = (buf[23] & 0xf) <<  0
        gyrox |= (buf[24] & 0xf) <<  4
        gyrox |= (buf[25] & 0xf) <<  8
        gyrox |= (buf[26] & 0xf) << 12
        sumcalc ^= gyrox >> 8;
        sumcalc ^= gyrox >> 0;
        gyroy  = (buf[27] & 0xf) <<  0
        gyroy |= (buf[28] & 0xf) <<  4
        gyroy |= (buf[29] & 0xf) <<  8
        gyroy |= (buf[30] & 0xf) << 12
        sumcalc ^= gyroy >> 8;
        sumcalc ^= gyroy >> 0;
        gyroz  = (buf[31] & 0xf) <<  0
        gyroz |= (buf[32] & 0xf) <<  4
        gyroz |= (buf[33] & 0xf) <<  8
        gyroz |= (buf[34] & 0xf) << 12
        sumcalc ^= gyroz >> 8;
        sumcalc ^= gyroz >> 0;

        temp   = (buf[35] & 0xf) <<  0
        temp  |= (buf[36] & 0xf) <<  4
        sumcalc ^= temp;

        sum    = (buf[37] & 0xf) <<  0
        sum   |= (buf[38] & 0xf) <<  4
        sumcalc ^= sum
        sumcalc &= 0xff

        if(sumcalc == 0) then
          if(no < 4) then
#            ShowImu(pos, ts, no, cnt, accx, accy, accz, gyrox, gyroy, gyroz, temp, sum)

            if($cntPrev[no] != -1 && (($cntPrev[no]+1)&mask) != cnt) then
              print "#{no} #{cnt} #{$cntPrev[no]}\n"
              $cntError[no] += 1
            else
              $cntRecv[no] += 1
            end
            $cntPrev[no] = cnt;

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


mainHamming
