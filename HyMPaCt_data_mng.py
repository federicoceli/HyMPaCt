#import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import style
from pathlib import Path
import os.path

# Valid file to process, flag
valid_file = False

# Wait till user prompts a valid file
while valid_file == False :     
    # Prompt user for file to process
    user_file = input("What file should I work on?\n")

    # Clear the input from quotation marks
    user_file_cleared = user_file[1:-1]
    
    # Check if file is valid
    valid_file = os.path.isfile(user_file_cleared)

    # Just a friendly prompt
    if valid_file == True :
        print("Sure, let me work on that!\n")
    else:
        print("Sorry, doesen't look right to me!\n")

# path = "C:\\Users\\fedce\\Desktop\\Hympact Log\\"
# file = "2017-07-20 190400.xlsx"

# Import data from excel file
data = pd.read_excel(user_file_cleared)

# Create a variable for the single columns
time = data['Time']
temp1 = data['Temp1']
temp2 = data['Temp2']

# Use better looking style
style.use('seaborn-white')

# Create a figure
fig = plt.figure()

# Sub plot 1
ax1 = fig.add_subplot(211)

ax1.plot(time, temp1)

plt.ylabel('Temperature [°C]')
plt.title('Temperature1')

plt.grid(True)

# Sub plot 2
ax2 = fig.add_subplot(212)

ax2.plot(time, temp2)

plt.xlabel('Time')
plt.ylabel('Temperature [°C]')
plt.title('Temperature2')

for label in ax2.xaxis.get_ticklabels():
    label.set_rotation(45)

plt.grid(True)

# Show the plot
plt.tight_layout()
plt.show()

print("The end")