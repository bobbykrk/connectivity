n = 4
r = 3

puts n.to_s + " " + (n*r).to_s

for i in 0...n
  for j in 1..r
    puts (i+1).to_s + " " + (((i+j)%n)+1).to_s
  end
end

