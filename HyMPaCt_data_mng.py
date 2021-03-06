""" 
    Created by Federico Celi 
    August 2017 for HyMPaCt Project 

"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import style
from pathlib import Path
import os.path

# Window height and width
W_HEIGHT = 1200
W_WIDTH = 850  

# More colors!
orange = '#ff8000'

# Formats the plots in the same fashion
def format_plot(plot, title, ylable) :
    plot.set_ylabel(ylable)
    plot.set_title(title)

    for label in plot.xaxis.get_ticklabels():
        label.set_rotation(45)

    plot.grid(True)

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
        print("Going to use dummy file\n")
        user_file_cleared = path + file
        valid_file = True
    else:
        print("Sorry, does not look right to me!\n")

# Import data from excel file
data = pd.read_excel(user_file_cleared)

# Create a variable for every single columns
# PT100
time    = data['Time']
temp1   = data['Temp1']
temp2   = data['Temp2']
temp3   = data['Temp3']

# Accelerometer
acc_x   = data['ACC_X']
acc_y   = data['ACC_Y']
acc_z   = data['ACC_Z']

# TEC DC
volt    = data['Voltage']
current = data['Current']

# TEC Temperatures
sink_temp = data['SinkT']
obj_temp  = data['ObjT']


# Use better looking style
style.use('seaborn-white')

# Create a figure
f, ((plot_temp1, plot_temp2, plot_temp3), (plot_voltage, plot_power, plot_acc)) = plt.subplots(2, 3)

# Sub plot 11
temp1 = temp1/100
plot_temp1.plot(time, temp1)

temp2 = temp2/100
plot_temp1.plot(time, temp2)
format_plot(plot_temp1, 'Temperature 1', 'Temperature [°C]')

# Sub plot 12
temp3 = temp3/100
plot_temp2.plot(time, temp3)
format_plot(plot_temp2, 'Temperature 2', 'Temperature [°C]')

# Sub plot 13
plot_temp3.plot(time, sink_temp, 'r')
plot_temp3.plot(time, obj_temp, 'c')
format_plot(plot_temp3, 'TEC Object (blue) and Sink (red) temperatures', 'Temperature [°C]')

# Sub plot 21
plot_voltage.plot(time, volt, orange)
plot_voltage.plot(time, current, 'g')   
format_plot(plot_voltage, 'TEC Voltage (Orange) and Current (Green)', 'Tension [V] / Current [A]')

# Sub plot 22
power = current*volt
plot_power.plot(time, power)   
format_plot(plot_power, 'TEC Power', 'Eletrical Power [W]')

# Sub plot 23
plot_acc.plot(time, acc_x, 'y')
plot_acc.plot(time, acc_y, 'g')
plot_acc.plot(time, acc_z, 'r')
format_plot(plot_acc, '3-Axis Acceleration XYZ = YGR', 'Accelerartion [g]')

# Resize figure 
mng = plt.get_current_fig_manager()
mng.resize(W_HEIGHT, W_WIDTH)

# TODO adjust wspace and hspace

# Make a better formatting and show the plot
plt.tight_layout()
f.subplots_adjust(left = 0.06, right = 0.98, bottom = 0.09, top = 0.93, wspace = 0.26, hspace = 0.44)
plt.show()

print("The end\n")