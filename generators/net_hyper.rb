sizex = 10
sizey = 10

def nodenum(i,j,sx,sy)
  return sx*(i%sx)+(j%sy)+1
end
puts (sizex*sizey).to_s + " " + (2*sizey*sizex).to_s

for i in 0...sizex
  for j in 0...sizey
    puts (nodenum(i,j,sizex,sizey).to_s + " " + nodenum(i+1,j,sizex,sizey).to_s) 
    puts (nodenum(i,j,sizex,sizey).to_s + " " + nodenum(i,j+1,sizex,sizey).to_s) 
  end
end


