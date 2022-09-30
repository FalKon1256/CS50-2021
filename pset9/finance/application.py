import os
import time

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# export API_KEY=pk_db90297150a74d499a8d8866e5e46ea4
# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# TODO
@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    
    # Query database for user's current stock assets
    assets = db.execute(
        "SELECT symbol, name, SUM(shares), price, SUM(total) FROM records WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
    
    # Query database for user's remaining fund
    cashRemain = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    
    # Calculate for user's total assets (remaining fund + stock assets)
    total = cashRemain[0]["cash"]
    for stock in assets:
        total += stock["SUM(total)"]
    
    # User reached route via GET (as by clicking a link or via redirect)
    return render_template("index.html", assets=assets, cash=cashRemain[0]["cash"], total=total)


# TODO
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Get input for company symbol (convert to uppercase) & Query database for stock info
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol)
        
        # Get input for share number that user wants to buy (datatype of shareNum is str, values from "input tag" are always "str" type)
        shareNum = request.form.get("shares")
        
        # Query database for user's remaining fund
        cashRemain = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        
        # Ensure symbol was submitted
        if not symbol:
            return apology("missing symbol", 400)
        
        # Getting invalid symbol input
        elif stock == None:
            return apology("invalid symbol", 400)
        
        # Ensure share number was submitted
        elif not shareNum:
            return apology("missing shares", 400)
            
        # Getting invalid share number input
        if not shareNum.isnumeric():
            return apology("invalid shares", 400)
        
        # Convert shareNum to int
        shareNum = int(shareNum)
        
        # Ensure user can afford (must have an shareNum input before converting to int to avoid error)
        if (stock["price"] * shareNum) > cashRemain[0]["cash"]:
            return apology("can't afford", 400)
        
        # If user can afford, add purchase record to database
        db.execute("INSERT INTO records (user_id, symbol, name, price, shares, total, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], stock["symbol"], stock["name"], stock["price"], shareNum, stock["price"] * shareNum, time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
        
        # Update user's fund (subtraction)
        db.execute("UPDATE users SET cash= ? WHERE id = ?", cashRemain[0]["cash"] - stock["price"] * shareNum, session["user_id"])
        
        # Redirect user to home page
        return redirect("/")
    
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


# TODO
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    # Query database for user's buying/selling records
    records = db.execute("SELECT symbol, shares, price, date FROM records WHERE user_id = ?", session["user_id"])
    
    return render_template("history.html", records=records)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


# TODO
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Get input for company symbol (convert to uppercase) & Query database for stock info
        symbol = request.form.get("symbol").upper()
        stock = lookup(symbol)
        
        # Getting invalid symbol input
        if stock == None:
            return apology("invalid symbol", 400)
        
        # Render webpage for stock info
        return render_template("quoted.html", company=stock["name"], symbol=stock["symbol"], price=stock["price"])
    
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


# TODO
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get input for username/password & Query database for username
        inputUsername = request.form.get("username")
        inputPassword = request.form.get("password")
        rows = db.execute("SELECT username FROM users WHERE username = ?", inputUsername)
        
        # Ensure username was submitted
        if not inputUsername or rows:
            return apology("username inncorrect", 400)
        
        # Ensure password was submitted
        elif not inputPassword:
            return apology("missing password", 400)

        # Ensure password was confirmed
        elif not request.form.get("confirmation") == inputPassword:
            return apology("password don't match", 400)
        
        # Add user data to database for registration
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", inputUsername, generate_password_hash(inputPassword))
        
        # Remember which user has logged in
        rows = db.execute("SELECT * FROM users WHERE username = ?", inputUsername)
        session["user_id"] = rows[0]["id"]
        
        # Redirect user to home page
        return redirect("/")
        
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        inputUsername = request.form.get("username")
        return render_template("register.html", inputUsername=inputUsername)


# TODO
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Query database for user's current stock assets
    assets = db.execute(
        "SELECT symbol, name, SUM(shares), price, date FROM records WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", session["user_id"])
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Get input for company symbol (convert to uppercase) & Query database for stock info
        symbol = request.form.get("symbol")
        
        # User's remaining shares (datatype of shareNumSell is str)
        shareNumSell = request.form.get("shares")
        
        # Ensure symbol was submitted
        if not symbol:
            return apology("missing symbol", 400)
        
        # Ensure share number was submitted
        elif not shareNumSell:
            return apology("missing shares", 400)
        
        # Select the stock symbol/shares corresponds to input
        for stock in assets:
            if symbol == stock["symbol"]:
                stockRow = stock
        
        # Convert shareNumSell to int
        shareNumSell = int(shareNumSell)
        
        # Ensure enough share number for selling (stockRow is no list anymore, it becomes a dictionary after "for loop")
        if int(shareNumSell) > stockRow["SUM(shares)"]:
            return apology("Too many shares", 400)
        
        # If user has enough share number for selling, add selling record to database
        db.execute("INSERT INTO records (user_id, symbol, name, price, shares, total, date) VALUES(?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], stockRow["symbol"], stockRow["name"], stockRow["price"], -shareNumSell, -stockRow["price"] * shareNumSell, time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()))
        
        # Query database for user's remaining fund
        cashRemain = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        
        # Update user's fund (addition)
        db.execute("UPDATE users SET cash= ? WHERE id = ?", cashRemain[0]
                   ["cash"] + stockRow["price"] * shareNumSell, session["user_id"])
        
        # Redirect user to home page
        return redirect("/")
    
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        symbols = []
        for stock in assets:
            symbols.append(stock["symbol"])
        
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
