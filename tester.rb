abort("tester.rb nazwa_pliku.exe [katalog_z_testami]") if ARGV.length < 1 
prog = ARGV[0]
dir_name = ARGV[1] || Dir.pwd

dir = Dir.new(dir_name)

dir.each do |file_name|
  full_file_name = dir_name + "/" + file_name
  check_file_name = fn = full_file_name.slice(0, full_file_name.length - 3) + '.out'
  if(File.extname(file_name) == '.in')
    print file_name
    beg = Time.now
    res = IO.popen("\"#{prog}\" < #{full_file_name}","r")
    time = Time.now - beg
    check_res = File.open(check_file_name,'r')
    res_line = res.readline
    check_res_line = check_res.readline
    if(res_line == check_res_line)
      print "\t" + "ok"
    else
      print "\t" + "error! jest: " + res_line + " powinno byc: " + check_res_line 
    end 
    puts " czas: " + time.to_s
    res.close
    check_res.close
  end
end

