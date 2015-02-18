#!/usr/env ruby

require "serialport"
#
# serial comamnds
#
CMD_PRESS = 100
CMD_RELEASE = 101
CMD_PRESS_AND_RELESE = 102

ARDUIONO_COMMANDS = {
	"p" => CMD_PRESS,
	"r" => CMD_RELEASE,
	"pr" => CMD_PRESS_AND_RELESE
}


STDOUT.puts "Start listening #{ARGV[0]} arduino's serial port"


arduino = SerialPort.new(ARGV[0], 9600, 8, 1)

while true 
	STDOUT.print "enter command (p - press, r - release, pr - press and release): "
	cmd = STDIN.gets.chop
	
	STDOUT.puts "Unknown command: #{cmd}" unless ARDUIONO_COMMANDS.key?(cmd)
	
	STDOUT.puts "Sending command to arduino"
	
	arduino.binmode.write(ARDUIONO_COMMANDS[cmd].chr)
	
	STDOUT.puts "OK"
	
	STDOUT.print "From arduino: "

	STDOUT.puts arduino.gets unless arduino.eof?

end

arduino.close