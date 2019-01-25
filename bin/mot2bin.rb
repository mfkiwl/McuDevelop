#!/usr/bin/ruby
#
# Copyright (c) 2018,2019 zhtlab
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to permit
# persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#

filename = ARGV[0]

addr = 0x0000

$data = Array.new


def Read(filename)
  addrStart = 0xffffffff
  addrEnd   = 0x00000000
  File.open( filename, 'rb' ) do |fp|
    while str = fp.gets
      str.chomp!
      words = str.scan(/.{1,2}/)

      if(words[0] == "S3") then
        len   = words[1].hex
        addr  = words[2].hex << 24
        addr |= words[3].hex << 16
        addr |= words[4].hex <<  8
        addr |= words[5].hex

        (len-5).times do |i|
          $data[addr+i] = words[i+6].hex
        end

        addrStart = addr       if(addrStart > addr) 
        addrEnd   = addr+len-6 if(addrEnd   < (addr+len-6)) 
        #    else
        #      printf("%s\n", str)
      end
    end
  end

  [addrStart, addrEnd]
end

def Output(addrStart, addrEnd)
  i = addrStart
  while i < addrEnd+1
    c = $data[i]
    c = 0xff if(c == nil) 
    putc(c)
    i += 1
  end
end


re = Read(filename)
addrStart = re[0]
addrEnd   = re[1]

#printf("%08x %08x\n", addrStart, addrEnd)

Output(addrStart, addrEnd)
