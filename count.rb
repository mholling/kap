#! /usr/bin/ruby
 
count = 0;
File.open("main.hex").each do |line|
  match = /^:[0-9A-F]{8}([0-9A-F]*)\s*$/.match(line);
  count += match[1].length/2 if match;
end
 
puts "main.hex contains #{count} bytes."