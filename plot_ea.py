import numpy as np
import matplotlib.pyplot as plt
plt.rcParams["font.family"] = "Times New Roman"
plt.rcParams['axes.facecolor'] = 'lightgray'
Ea = np.loadtxt('events_clean.list',usecols=(-1))
fig = plt.figure(facecolor='gray')
plt.hist(Ea,bins=20)
plt.xlabel('Activation energy (eV)',fontsize=20)
plt.ylabel('Frequency',fontsize=20)
