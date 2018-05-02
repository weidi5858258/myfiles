#coding=utf-8

from flask import Flask

app=Flask(__name__)
app.config['DEBUG'] = True

@app.route('/hello')
def hello_world():
	return 'hello_world'

@app.route('/hello2')
def hello_world2():
	return 'hello_world2'

@app.route('/hello3/<name>')
def hello_world3(name):
	return 'hello_world3: ' + name

if __name__ == '__main__':
	# app.debug = True
	app.run(debug=True, host='0.0.0.0', port=9000)
