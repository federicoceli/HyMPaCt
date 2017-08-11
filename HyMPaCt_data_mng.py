# Createb by Federico Celi 
# August 2017 for HyMPaCt Project

#import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import style
from pathlib import Path
import os.path

# Valid file to process, flag
valid_file = False

# Change to local know folder
path = "C:\\Users\\fedce\\Desktop\\"
file = "HyMPaCt dummy file.xlsx"

# Wait till user prompts a valid file
while valid_file == False :     
    # Prompt user for file to process
    user_file = input("What file should I work on? (Leave blanc for DEMO)\n")

    # Clear the input from quotation marks
    user_file_cleared = user_file[1:-1]
    
    # Check if file is valid
    valid_file = os.path.isfile(user_file_cleared)

    # Just a friendly prompt
    if valid_file == True :
        print("Sure, let me work on that!\n")
    elif user_file == '' :
        print("Going to use dummy file")
        user_file_cleared = path + file
        valid_file = True
    else:
        print("Sorry, does not look right to me!\n")

# Import data from excel file
data = pd.read_excel(user_file_cleared)

# Create a variable for the single columns
time    = data['Time']
temp1   = data['Temp1']
temp2   = data['Temp2']
temp3   = data['Temp3']
volt    = data['Voltage']
current = data['Current']

# Use better looking style
style.use('seaborn-white')

# Create a figure
f, ((plot_temp1, plot_temp2), (plot_voltage, plot_current)) = plt.subplots(2, 2)

# Sub plot 1
plot_temp1.plot(time, temp1)

plot_temp1.set_xlabel('Time')
plot_temp1.set_ylabel('Temperature [°C]')
plot_temp1.set_title('Temperature #1')

for label in plot_temp1.xaxis.get_ticklabels():
    label.set_rotation(45)

plot_temp1.grid(True)

# Sub plot 2
plot_temp2.plot(time, temp2)

plot_temp2.set_xlabel('Time')
plot_temp2.set_ylabel('Temperature [°C]')
plot_temp2.set_title('Temperature #2')

for label in plot_temp2.xaxis.get_ticklabels():
    label.set_rotation(45)

plot_temp2.grid(True)

# Sub plot 3
plot_voltage.plot(time, volt)

plot_voltage.set_xlabel('Time')
plot_voltage.set_ylabel('Tension [°V]')
plot_voltage.set_title('TEC Voltage')

for label in plot_voltage.xaxis.get_ticklabels():
    label.set_rotation(45)

plot_voltage.grid(True)

# Sub plot 4
plot_current.plot(time, current)

plot_current.set_xlabel('Time')
plot_current.set_ylabel('Current [A]')
plot_current.set_title('TEC Current')

for label in plot_current.xaxis.get_ticklabels():
    label.set_rotation(45)

plot_current.grid(True)

# Show the plot
plt.tight_layout()
plt.show()

print("The end")
