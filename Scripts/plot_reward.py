import numpy as np
import matplotlib.pyplot as plt


agent = "MK_1"
agent_2 = "MK_2"
with open("/Users/maxikrahschutz/Desktop/Studium/Master_EI/3.Semester/ARL/Project/Group-F/Storage/" + agent + "/training_reward.info") as f:
    data = f.read()

data_txt = data.split('\n')
data_y = []

for txt in data_txt:
    try:
        data_y.append(float(txt))
    except:
        print("An exception occurred")        

with open("/Users/maxikrahschutz/Desktop/Studium/Master_EI/3.Semester/ARL/Project/Group-F/Storage/" + agent_2 + "/training_reward.info") as f:
    data = f.read()

data_txt = data.split('\n')
data_y2 = []

for txt in data_txt:
    try:
        data_y2.append(float(txt))
    except:
        print("An exception occurred")  


#x = [row.split(' ')[0] for row in data]
#y = [row.split(' ')[1] for row in data]

plt.plot(data_y, color="orange", label="single_step")
plt.plot(data_y2, color="blue", label="multi_step")
plt.xlabel('Epochs x 1000')
plt.ylabel('Reward per 1000 trajectories')
plt.savefig('/Users/maxikrahschutz/Desktop/multistep2.pdf')  


exit()


fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Reward")    
#ax1.set_xlabel('Epoch in e3')
ax1.set_ylabel('Reward per 1000 Epochs')


ax1.plot(data_y)

#leg = ax1.legend()

plt.show()
