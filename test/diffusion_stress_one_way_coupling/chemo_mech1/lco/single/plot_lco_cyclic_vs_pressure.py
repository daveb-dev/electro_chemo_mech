
# coding: utf-8

# In[159]:


import pandas as pd
import os
import sys
import matplotlib
import matplotlib.pyplot as plt


# In[160]:


pressure = ['0.0','10e-6','50e-6','100e-6']
pressure_dir = ['pres_' + x for x in pressure]
pressure_dir


# In[161]:


df_list = []


# In[162]:


homedir = "/home/srinath/Documents/workspace_dev/electro_chemo_mech/test/diffusion_stress_one_way_coupling/chemo_mech1/lco/single/1C"
os.chdir(homedir)
os.getcwd()


# In[163]:


for i in pressure_dir:
    os.chdir(i)
    df_list.append(pd.read_csv('lco_circle_bc_out.csv',skiprows=[1,2]))
    os.chdir('../')


# In[164]:


for d,pr in zip(df_list, pressure):
    p = str(float(pr)*1e6) + "MPa"
    print pr, p
    plt.plot(d["time"]/7200.0, (d["ave_stress_22_top"] + float(pr))*1e12/1e5, label=p)
plt.legend()


# In[165]:


for d,pr in zip(df_list, pressure):
    p = str(float(pr)*1e6) + "MPa"
    print pr, p
    plt.plot(d["ave_conc_inner"]/7200.0, (d["ave_stress_22_top"] + float(pr))*1e12/1e5, label=p)
plt.legend()


# In[172]:


d["ave_stress_22_top"] + float(pr)


# In[173]:


os.getcwd()

