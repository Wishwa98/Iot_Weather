from datetime import datetime
from dateutil.relativedelta import relativedelta
import pandas as pd
import numpy
from flask import Flask, request, jsonify
import pickle

app = Flask(__name__)
model = pickle.load(open('ArimaModelNew.pkl', 'rb'))


@app.route('/', methods=['GET'])
def predict():
    previous_year = datetime.today() - relativedelta(years=1)
    previous_year = previous_year.strftime("%Y-%m-%d")

    next_year = datetime.today() + relativedelta(years=1)
    next_year = next_year.strftime("%Y-%m-%d")
    
    prediction = model.predict(start=previous_year, end=next_year)
    
    return pd.Series.tolist(prediction)


if __name__ == '__main__':
    app.run(debug=True)
