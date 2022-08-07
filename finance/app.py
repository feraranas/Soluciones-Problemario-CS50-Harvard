import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""

    # Select the table
    stocks = db.execute("SELECT * FROM total_owned WHERE person_id = ?", session["user_id"])

    # Calculate the total cost of stocks
    cashTotal = 0
    for i in range(len(stocks)):
        cashTotal += stocks[i]["total"]

    # Sum total cost of stocks and remaining cash available
    cashrem = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])

    total = cashrem[0]["cash"] + cashTotal

    # Render the html table in index.html
    return render_template("index.html", stocks = stocks, cash = cashrem[0]["cash"], total = total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Save the stock information: name, price, symbol
        stock = lookup(request.form.get("symbol"))

        # If the stock search box is blank or the stock doesn't exist
        if not request.form.get("symbol") or not stock:
            return apology("Error obtaining stock symbol", 400)

        if not request.form.get("symbol").isalpha():
            return apology("Invalid stock symbol", 400)

        # If the number box is empty
        if not request.form.get("shares"):
            return apology("No amount of stock selected", 400)

        # Number of stocks
        shares = int(request.form.get("shares"))

        # Total Cost of stocks
        totalCost = shares * stock["price"]

        # Check for the available cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Update purchases if there's cash available
        if cash[0]["cash"] >= totalCost:
            # Calculate actual time
            time = datetime.datetime.now()

            # CREATE TABLE purchases_sales (person_id INTEGER NOT NULL, symbol TEXT NOT NULL, status TEXT NOT NULL, shares INTEGER NOT NULL, price INTEGER NOT NULL, time TIMESTAMP, FOREIGN KEY(person_id) REFERENCES users(id));

            # Insert the purchase into purchases db
            db.execute("INSERT INTO purchases_sales (person_id, symbol, status, shares, price, time) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], stock["symbol"], "buy", shares, stock["price"], time)

            # CREATE TABLE total_owned (person_id INTEGER NOT NULL, symbol_id TEXT NOT NULL, name TEXT NOT NULL, shares INTEGER NOT NULL, price INTEGER NOT NULL, total INTEGER NOT NULL DEFAULT 0, FOREIGN KEY(person_id) REFERENCES users(id));

            # Check whether or not is the first buy of that stock
            currentStock = db.execute("SELECT symbol_id FROM total_owned WHERE person_id = ? AND symbol_id = ?", session["user_id"], stock["symbol"])
            if not currentStock:
                # Update total_owned table
                db.execute("INSERT INTO total_owned (person_id, symbol_id, name, shares, price, total) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], stock["symbol"], stock["name"], shares, stock["price"], totalCost)
                # Update cash available
                updatedCash = cash[0]["cash"] - totalCost
                db.execute("UPDATE users SET cash = ? WHERE id = ?", updatedCash, session["user_id"])
                # Redirect to index
                return redirect("/")

            if currentStock[0]["symbol_id"] == stock["symbol"]:

                # Calculate the total shares already bought
                listaCompra = db.execute("SELECT shares FROM purchases_sales WHERE person_id = ? AND symbol = ? AND status = ?", session["user_id"], stock["symbol"], "buy")
                if listaCompra:
                    sharesCompra = 0
                    for i in range(len(listaCompra)):
                        sharesCompra += listaCompra[i]["shares"]
                else:
                    sharesCompra = 0

                # Calculate the total shares already sold
                listaVenta = db.execute("SELECT shares FROM purchases_sales WHERE person_id = ? AND symbol = ? AND status = ?", session["user_id"], stock["symbol"], "sell")
                if listaVenta:
                    sharesVenta = 0
                    for i in range(len(listaVenta)):
                        sharesVenta += listaVenta[i]["shares"]
                else:
                    sharesVenta = 0

                # Do the arithmetics
                if sharesCompra == sharesVenta:
                    # Se elimina registro alguno de ese stock
                    db.execute("DELETE FROM total_owned WHERE symbol_id = ? AND person_id = ?", stock["symbol"], session["user_id"])
                    # Se agregan a total_owned las recien compradas
                    db.execute("INSERT INTO total_owned (person_id, symbol, name, shares, price, total) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], stock["symbol"], stock["name"], shares, stock["price"], totalCost)
                    # Update cash available
                    updatedCash = cash[0]["cash"] - totalCost
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", updatedCash, session["user_id"])
                    # Redirect to index
                    return redirect("/")
                elif sharesCompra > sharesVenta:
                    # Se calculan las shares existentes
                    sharesExistentes = sharesCompra - sharesVenta
                    # Calculate the total value of stocks owned
                    totalValue = sharesExistentes * stock["price"]
                    # Update the table
                    db.execute("UPDATE total_owned SET shares = ?, price = ?, total = ? WHERE person_id = ? AND symbol_id = ?", sharesExistentes, stock["price"], totalValue, session["user_id"], stock["symbol"])
                    # Update cash available
                    updatedCash = cash[0]["cash"] - totalCost
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", updatedCash, session["user_id"])
                    # Redirect to index
                    return redirect("/")
                else:
                    # Hay un error, no pueden haber más acciones vendidas que compradas
                    return apology("There's been an error", 404)
        # In case the user can't afford the purchase
        else:
            return apology("Cannot Afford", 333)

        # Redirect to index
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    stocks = db.execute("SELECT * FROM purchases_sales WHERE person_id = ?", session["user_id"])

    # Render the html table in index.html
    return render_template("history.html", stocks = stocks)


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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Check for errors
        if not request.form.get("symbol"):
            return apology("Missing symbol", 400)

        if not request.form.get("symbol").isalpha():
            return apology("Invalid symbol", 400)

        # Parsing the symbol submitted in the API
        stock = lookup(request.form.get("symbol"))

        if not stock:
            return apology("Error ocurred", 400)

        # Rendering html file with results
        return render_template("quoted.html", stock=stock)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username is submitted username doesn't already exist
        if not request.form.get("username"):
            return apology("Must enter username", 400)

        username = db.execute("SELECT username FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username doesn't already exists
        if username:
            return apology("Username already exists", 400)

        # Ensure password is submitted
        if not request.form.get("password"):
            return apology("Must enter a password", 400)

        # Ensure password confirmation
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("Password confirmation error", 400)

        # Create user in database
        db.execute("INSERT INTO users (username, hash) VALUES(?,?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form)
    if request.method == "POST":

        # Get from the html the selected input by user
        symbol = lookup(request.form.get("symbol"))

        # Check for errors — User didn't select an input
        if not symbol:
            return apology("Error selecting symbol", 400)

        # Check for errors — User doesn't own any stocks
        errStock = db.execute("SELECT shares FROM total_owned WHERE symbol_id = ?", symbol["symbol"])
        if errStock[0]["shares"] == 0:
            return apology("An error has ocurred", 400)

        # Get from the html the inputted number of shares
        numberStocks = int(request.form.get("shares"))

        # Check for errors — User didn't enter an input
        if not numberStocks:
            return apology("Error selecting shares", 400)

        # Grab the amount of shares owned
        sharesOwned = db.execute("SELECT shares FROM total_owned WHERE symbol_id = ? AND person_id = ?", symbol["symbol"], session["user_id"])

        # Update the number of stocks & the total value after sale
        if numberStocks > sharesOwned[0]["shares"]:

            return apology("You don't own that much shares to sell", 400)

        elif numberStocks == sharesOwned[0]["shares"]:

            time = datetime.datetime.now()

            currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            totalCash = currentCash[0]["cash"] + numberStocks * symbol["price"]

            db.execute("UPDATE users SET cash = ? WHERE id = ?", totalCash, session["user_id"])

            db.execute("DELETE FROM total_owned WHERE symbol_id = ? AND person_id = ?", symbol["symbol"], session["user_id"])

            db.execute("INSERT INTO purchases_sales (person_id, symbol, status, shares, price, time) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], symbol["symbol"], "sell", numberStocks, symbol["price"], time)

        else:

            time = datetime.datetime.now()

            newSharesOwned = sharesOwned[0]["shares"] - numberStocks

            currentCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

            totalCash = currentCash[0]["cash"] + numberStocks * symbol["price"]

            totalValue = newSharesOwned * symbol["price"]

            db.execute("UPDATE users SET cash = ? WHERE id = ?", totalCash, session["user_id"])

            db.execute("UPDATE total_owned SET shares = ?, total = ? WHERE symbol_id = ? AND person_id = ?", newSharesOwned, totalValue, symbol["symbol"], session["user_id"])

            db.execute("INSERT INTO purchases_sales (person_id, symbol, status, shares, price, time) VALUES(?, ?, ?, ?, ?, ?)", session["user_id"], symbol["symbol"], "sell", numberStocks, symbol["price"], time)

        # Redirect to "/"
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        # Retrieve all the symbol stocks the user owns
        stocks = db.execute("SELECT DISTINCT(symbol_id) FROM total_owned WHERE person_id = ?", session["user_id"])

        # Render a template sell.html with stocks being send as input
        return render_template("sell.html", stocks = stocks)
