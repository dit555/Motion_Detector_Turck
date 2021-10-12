f = open("enc.csv", "w")

i = 0
while (i < 100):
    w = ""
    w = str(i) +",0," + str(-i) + "\n"
    f.write(w)
    i += 1

f.close()
