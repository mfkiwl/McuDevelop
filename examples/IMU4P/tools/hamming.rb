#!/usr/bin/ruby


G = [[1, 0, 0, 0,  0, 1, 1],
     [0, 1, 0, 0,  1, 0, 1],
     [0, 0, 1, 0,  1, 1, 0],
     [0, 0, 0, 1,  1, 1, 1]]


H = [[1, 0, 1, 0,  1, 0, 1],
     [0, 1, 1, 0,  0, 1, 1],
     [0, 0, 0, 1,  1, 1, 1]]


$outCode = [-1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1]

P = [0, 1, 1, 0, 1, 0, 0, 1,
     1, 0, 0, 1, 0, 1, 1, 0]

$tblSyndorome = Array.new
$tblCorrection = Array.new

def main

  print "\n/* output code */\n"
  16.times do |code|

    byte = 0

    7.times do |j|
      bit = 0
      tmp = code
      4.times do |i|
        bit ^= tmp & G[i][j] & 1
        tmp >>= 1
      end
      byte |= bit << j
    end

    if(code == 8) then
      print "\n"
    end
    $outCode[code] = byte
    printf("0x%02x, ", byte)

  end
  print "\n\n"


  print "\n/*output code with odd parity*/\n"
  16.times do |code|
    byte = $outCode[code]
    parity = (P[(byte>>4)&0xf] + P[byte&0xf]) & 1
    if(code == 8) then
      print "\n"
    end
    printf("0x%02x, ", byte | ((parity == 1)? 0x00: 0x80));
  end
  print "\n\n"


  print "\n/* decode */\n"
  128.times do |recv|

    byte = 0

    3.times do |j|
      bit = 0
      tmp = recv
      7.times do |i|
        bit ^= tmp & H[j][i] & 1
        tmp >>= 1
      end
      byte |= bit << j
    end

    $tblSyndorome[recv] = byte

    if((recv & 0xf) == 0) then
      print "\n"
    end
    if(byte == 0) then
      printf("[0x%02x, ", byte)
    else
      printf(" 0x%02x, ", byte)
    end

  end
  print "\n\n"


  print "xxxxxxxxxxxxxxxxxx\n"
  128.times do |val|
    if((val & 0xf) == 0) then
      print "\n"
    end

    syn = $tblSyndorome[val]
    if(syn != 0) then
      val ^= (1 << (syn-1))
    end
    val &= 0xf
    val |= syn << 4
    $tblCorrection = val
    printf("0x%02x, ", val);
  end
  print "\n"

end

main
