sizex = 3
sizey = 3

def nodenum(i,j,sx)
  return sx*(i-1)+j
end
puts (sizex*sizey).to_s + " " + ((sizey-1)*sizex + (sizex-1)*sizey).to_s

for i in 1..sizex
  for j in 1..sizey
    puts (nodenum(i,j,sizex).to_s + " " + nodenum(i+1,j,sizex).to_s) unless i==sizex
    puts (nodenum(i,j,sizex).to_s + " " + nodenum(i,j+1,sizex).to_s) unless j==sizey
  end
end


