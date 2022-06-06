from flask import Flask, render_template, request
import pickle
import numpy as np

model = pickle.load(open('classifier.pkl','rb'))

app = Flask(__name__)

@app.route('/')
def man():
    return render_template('home.html')

@app.route('/predict',methods=['POST'])
def home():
    data1 = request.form['a']
    data2 = request.form['b']
    data3 = request.form['c']
    data4 = request.form['d']
    data5 = request.form['e']
    data6 = request.form['f']
    data7 = request.form['g']
    data8 = request.form['h']

    arr = np.array([[data1,data2,data3,data4,data5,data6,data7,data8]])
    ans = model.predict(arr)

    # if ans[0] == 0:
    #     pred =  "10-26-26"
    # elif ans[0] ==1:
    #     pred = "14-35-14"
    # elif ans[0] == 2:
    #     pred = "17-17-17"
    # elif ans[0] == 3:
    #     pred = "20-20"
    # elif ans[0] == 4:
    #     pred = "28-28"
    # elif ans[0] == 5:
    #     pred = "DAP"
    # else:
    #     pred = "Urea"

    return render_template('after.html',data=ans[0])

if __name__ == "__main__":
    app.run(debug=True)