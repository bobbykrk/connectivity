n = 90
puts n.to_s + " " + (n*(n-1)/2).to_s
for i in 1..n
  for j in (i+1)..n
    puts i.to_s + " " + j.to_s
  end
end

