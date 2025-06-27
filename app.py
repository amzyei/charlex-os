from flask import Flask, render_template
from window_manager import window_manager_bp

app = Flask(__name__)
app.register_blueprint(window_manager_bp)

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True)
