import os

os.system("cd /home/vascoffaria/Desktop/compiladores")

for filename in os.listdir("./auto-tests/"):    
    if filename.endswith(".og"): 
        print("echo " + filename)
        print("./src/og < ./auto-tests/" + filename + " -o ./auto-tests/results/" + filename + ".xml")