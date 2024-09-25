data = open("data.txt", "r")
form = open("Jerk_2_thinned.dat", "w")

for line in data:
    out = line[0:len(line)-2]
    out = out.replace(';', ' ')
    form.write(out)
    form.write("\n")

data.close()
form.close()

print("End program")
