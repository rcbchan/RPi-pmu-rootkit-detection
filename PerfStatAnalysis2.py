import pandas as pd
from imblearn.under_sampling import RandomUnderSampler
import sklearn.model_selection as model_selection
import numpy as np
from pyod.models.knn import KNN
from pyod.utils import evaluate_print
data = pd.read_csv("PerfRootkit1.csv", nrows=1000)
#data = pd.read_csv("mixperfset.csv")
import matplotlib.pyplot as plt
import glob
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
#data = pd.concat([pd.read_csv(f, sep=",", nrows=1000) for f in glob.glob('PerfRootkit*.csv')], ignore_index = False)
print(data.head())

data = data[["Instructions", "Cache Misses", "Branch Misses", "Cache Reads", "Cache Writes", "LL1 Data Cache", "LL1 Instruction Cache", "Last Level Cache", "Data TLB", "Instruction Cache", "y"]]
#data1 = data[["Instruct", "Cache Miss", "Branch Miss", "Cache R", "Cache W", "LL1 Data C", "LL1 Instruct Cache", "LLC", "D TLB", "Instruct Cache", "Y"]]
str_conv1 = data["Instructions"].to_list()
list_conv1 = data["Cache Misses"].to_list()
#list_conv2 = data["Cache Miss"].to_list()

list_conv3 = data["Instructions"].to_list()
#list_conv4 = data["Instruct"].to_list()


#X = data["Instructions"].values.reshape(-1,1)
#y = data["y"].values


#clf = KNN(contamination=0.002, n_neighbors=5)
#clf.fit(X)
#y_train_pred = clf.labels_
#y_train_scores = clf.decision_scores_

#evaluate_print('KNN', y, y_train_scores)

SMA_Cache_Miss = data["Cache Misses"]
window_size = 200
windows = SMA_Cache_Miss.rolling(window_size)
moving_averages = windows.mean()
moving_averages_list = moving_averages.tolist()
no_nans = moving_averages_list[window_size -1:]
#print(no_nans)


#SMA_Cache_Miss1 = data["Cache Miss"]
#window_size1 = 200
#windows1 = SMA_Cache_Miss1.rolling(window_size1)
#moving_averages1 = windows1.mean()
#moving_averages_list1 = moving_averages1.tolist()
#no_nans1 = moving_averages_list1[window_size1 -1:]


legplot1 = list_conv1
plt.plot(legplot1)
plt.title("Before Smoothing Data with Rolling Average (with rootkit)")
plt.xlabel("Samples")
plt.ylabel("Cache Misses")
plt.savefig('data1.png')
plt.show()


#legplot2 = list_conv2
#plt.plot(legplot2)
#plt.title("Samples Before Smoothing Data with Rolling Average (without rootkit)")
#plt.xlabel("Samples")
#plt.ylabel("Cache Misses")
#plt.show()


legplot3 = no_nans
#legplot4 = no_nans1
plt.plot(legplot3, label= "With Rootkit")
#plt.plot(legplot4, label="No Rootkit")
plt.title("Smoothing Data with Moving Average (with Rookit)")
plt.xlabel("Samples")
plt.ylabel("Cache Misses")
#plt.plot(legplot2)
plt.legend()
plt.savefig('data2.png')
plt.show()


legplot5 = list_conv3
plt.plot(legplot5, color="green")
plt.title("Before Smoothing Data with Moving Average (with rootkit)")
plt.xlabel("Samples")
plt.ylabel("Instructions")
plt.savefig('data3.png')
plt.show()



SMA_Cache_Miss1 = data["Instructions"]
window_size1 = 200
windows = SMA_Cache_Miss1.rolling(window_size1)
moving_averages = windows.mean()
moving_averages_list1 = moving_averages.tolist()
no_nans1 = moving_averages_list1[window_size1 -1:]

legplot7 = no_nans1
plt.plot(legplot7, label="With Rootkit", color="green")
plt.title("Smoothing Data with Moving Average (with Rookit)")
plt.xlabel("Samples")
plt.ylabel("Instructions")
#plt.plot(legplot2)
plt.legend()
plt.savefig('data4.png')
plt.show()

data["y"].to_string(header=False, index=False).split("\n")

X = data.iloc[:, :-9].values
y = data.iloc[:, :12].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.20)

scaler = StandardScaler()
scaler.fit(X_train)

X_train = scaler.transform(X_train)
X_test = scaler.transform(X_test)

from sklearn.neighbors import KNeighborsClassifier
classifier = KNeighborsClassifier(n_neighbors=5)
classifier.fit(X_train, y_train)

y_pred = classifier.predict(X_test)

from sklearn.metrics import classification_report, confusion_matrix
print(confusion_matrix(y_test, y_pred))
print(classification_report(y_test, y_pred))