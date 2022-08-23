from distutils.log import debug
from operator import methodcaller
from re import T
from unittest import result
from flask import Flask, render_template, request
from backend import extended_boolean

app = Flask(__name__)
is_matrix_used = False
data_source = './static/data'
ebm = extended_boolean.ExtendedBooleanModel(is_matrix_used, data_source)


@app.route("/")
def home():
    return render_template("index.html")


@app.route('/get_query', methods=['POST'])
def get_query():
    query = request.form['queryInput']
    time_spent, results = ebm.get_query_results(query)
    return render_template("result.html", query=query, time=time_spent, value=results)


@app.route('/query_results')
def query_results():
    return render_template("result.html")


if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=4928)
