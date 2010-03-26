#! /usr/bin/ruby

begin
  file = File.open("count.txt", "r+")
  begin
    count = file.readline.to_i + 1
  rescue EOFError
    count = 1
  end
rescue Errno::ENOENT
  file = File.open("count.txt", "w+")
  count = 1
end
file.rewind
file.puts count
file.close
 
puts "Flash erase cycle: #{count}"
