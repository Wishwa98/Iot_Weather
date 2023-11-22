# -*- coding: utf-8 -*-
"""IOTArimaModel

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1Bx_c_mfPmB6LCkxgQFPVGyoGfHJ7DoQ_
"""

import pandas as pd
import matplotlib.pyplot as plt

from google.colab import drive
drive.mount('/content/drive')

# read in the dataset from your Drive and setting Date as index column
df = pd.read_csv('/content/drive/My Drive/SLIIT/Y4S1/ColomboRHPrePorcessed.csv',index_col = 'Date', parse_dates = True)
print('Shape of Data', df.shape)
df.head()

df.tail()

df['obs_val'].plot(figsize=(12,5))

#perfoming ADF(Augmented Dickey Fuller Test) in order clarify the if data is stationary or not. [if p<0.05, then data is set to be stationary]

from statsmodels.tsa.stattools import adfuller
def ad_test(dataset):
     dftest = adfuller(dataset, autolag = 'AIC')
     print("1. ADF : ",dftest[0])
     print("2. P-Value : ", dftest[1])
     print("3. Num Of Lags : ", dftest[2])
     print("4. Num Of Observations Used For ADF Regression:",      dftest[3])
     print("5. Critical Values :")
     for key, val in dftest[4].items():
         print("\t",key, ": ", val)
ad_test(df['obs_val'])

pip install pmdarima

"""Identifying best ARIMA Model:
The code is pretty self explanatory. We simple supply our data to the auto_arima function. The function basically uses something called as the AIC score to judge how good a particular order model is. It simply tries to minimize the AIC score, and here’s how the output looks like
"""

from pmdarima import auto_arima
stepwise_fit = auto_arima(df['obs_val'], trace=True,
suppress_warnings=True)

"""Before we actually train the model, we have to split the data set into a training and testing section. We do this because we first train the model on the data and keep the testing section hidden from the model. Once model is ready, we ask it to make predictions on the test data and see how well it performs."""

print(df.shape)
train=df.iloc[:-30]
test=df.iloc[-30:]
print(train.shape,test.shape)

"""Surprisingly, creating the ARIMA model is actually one of the easiest steps once you have done all the prerequisite steps. It’s as simple as shown in the code snippet below:


As you can see we simply call the ARIMA function, supply it our data set and mention the order of the ARIMA model we want. You will be able to see the summary of the model in your output as well.

```
# This is formatted as code
```


"""

#from statsmodels.tsa.arima_model import ARIMA
import statsmodels.api as sm
model=sm.tsa.arima.ARIMA(train['obs_val'],order=(3,1,3))
model=model.fit()
model.summary()

"""**Check How Good Your Model Is**
Here’s where our test data comes in. We first make prediction for temperature on the test data. Then we plot out to see how our predictions compared to the actual data.
"""

start=len(train)
end=len(train)+len(test)-1
pred1=model.predict(start=start,end=end,typ='levels').rename('ARIMA Predictions')
print(pred1)
pred1.index=df.index[start:end+1]
print(pred1)

pred1.plot(legend=True)
test['obs_val'].plot(legend=True)

"""**Check your Accuracy Metric**
To actually ascertain how good or bad your model is we find the root mean squared error for it. The following code snippet shows that
"""

from sklearn.metrics import mean_squared_error
from math import sqrt
test['obs_val'].mean()
rmse=sqrt(mean_squared_error(pred1,test['obs_val']))
print(rmse)

FutureDates = pd.date_range(start ='2018-12-31 ',end='2019-12-31 ')
#start=len(train)
#end=len(train)+len(test)-1
pred2=model.predict(start=len(df),end=len(df)+30,typ='levels').rename('ARIMA Predictions')

#pred2.index=FutureDates
print(pred2)

pred2.plot(legend=True)