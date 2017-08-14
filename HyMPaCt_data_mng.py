# Createb by Federico Celi 
# August 2017 for HyMPaCt Project

#import numpy as np

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib import style
from pathlib import Path
import os.path
from pylab import rcParams

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
time    = data['Time']
temp1   = data['Temp1']
temp2   = data['Temp2']
temp3   = data['Temp3']
volt    = data['Voltage']
current = data['Current']

# Use better looking style
style.use('seaborn-white')

# Create a figure
f, ((plot_temp1, plot_temp2, plot_temp3), (plot_voltage, plot_current, plot_heatFlux)) = plt.subplots(2, 3)

# Sub plot 11
plot_temp1.plot(time, temp1)
format_plot(plot_temp1, 'Temperature 1', 'Temperature [°C]')

# Sub plot 12
plot_temp2.plot(time, temp2)
format_plot(plot_temp2, 'Temperature 2', 'Temperature [°C]')

# Sub plot 13
plot_temp3.plot(time, temp3)
format_plot(plot_temp3, 'Temperature 3', 'Temperature [°C]')

# Sub plot 21
plot_voltage.plot(time, volt)
format_plot(plot_voltage, 'TEC Voltage', 'Tension [V]')

# Sub plot 22
plot_current.plot(time, current)   
format_plot(plot_current, 'TEC Current', 'Current [A]')

# Sub plot 23
plot_heatFlux.plot(time, current) # TODO just a place holder, change when data available  
format_plot(plot_heatFlux, 'Heat Flux', 'Heat Flux [W/m**2]')

# Resize figure 
mng = plt.get_current_fig_manager()
mng.resize(*mng.window.maxsize())

# Better spacing between graphs
f.wspace : 0.24
f.hspace : 0.30

# Show the plot
plt.tight_layout()
plt.show()

print("The end\n")