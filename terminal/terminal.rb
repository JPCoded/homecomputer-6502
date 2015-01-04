#!/bin/env ruby
# encoding: UTF-8

require 'serialport'

serial = SerialPort.open('/dev/ttyUSB0', 9600)

trap 'SIGINT' do
  serial.close
end

def cmd_save serial, filename
  File.open(filename, 'w') do |file|
    while line = serial.gets.chomp
      break if line =~ /\*EOF/
      file.puts line
      print '.'
    end
  end
  puts "\nSaved program to file #{filename}"
end

def cmd_load serial, filename
  File.open(filename).each do |line|
    serial.gets
    serial.puts line
  end
  serial.gets
  serial.puts '*EOF'
  puts "Loaded program from file #{filename}"
end

while true do
  begin
    line = serial.gets.chomp
    puts line
    begin
      case line
        when /\*SAVE (\w+)/
          cmd_save serial, "programs/#{$1}.bas"
        when /\*LOAD (\w+)/
          cmd_load serial, "programs/#{$1}.bas"
      end
    rescue ArgumentError
    end
  rescue IOError => x
    break
  end
end