n = 10

puts n.to_s + " " + n.to_s 
for i in 0...n
  puts (i+1).to_s + " " + ((i+1)%n+1).to_s
end
